/*

graphwriter.cpp ... Graph writer implementation.

Copyright (C) 2009  KennyTM~

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef IGRAPH_GRAPHWRITER_CPP
#define IGRAPH_GRAPHWRITER_CPP

#include <igraph/cpp/graph.hpp>
#include <igraph/cpp/graphio.hpp>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#if __MSVC__
#define strcasecmp _stricmp
#endif

namespace igraph {
	
	GraphFormat identify_file_format(const char* filename, const bool open_file_to_check) {
		// (1) find the dot.
		const char* theDot = strrchr(filename, '.');
		if (theDot != NULL) {
			++ theDot;
			if (strcasecmp("ncol", theDot) == 0) return GraphFormat_ncol;
			else if (strcasecmp("lgl", theDot) == 0) return GraphFormat_lgl;
			else if (strcasecmp("graphml", theDot) == 0) return GraphFormat_graphml;
			else if (strcasecmp("gml", theDot) == 0) return GraphFormat_gml;
			else if (strcasecmp("dot", theDot) == 0) return GraphFormat_dot;
			else if (strcasecmp("graphviz", theDot) == 0) return GraphFormat_dot;
			else if (strcasecmp("net", theDot) == 0) return GraphFormat_pajek;
			else if (strcasecmp("pajek", theDot) == 0) return GraphFormat_pajek;
			else if (strcasecmp("dimacs", theDot) == 0) return GraphFormat_dimacs;
			else if (strcasecmp("edge", theDot) == 0) return GraphFormat_edgelist;
			else if (strcasecmp("edges", theDot) == 0) return GraphFormat_edgelist;
			else if (strcasecmp("edgelist", theDot) == 0) return GraphFormat_edgelist;
			else if (strcasecmp("adj", theDot) == 0) return GraphFormat_adjlist;
			else if (strcasecmp("adjlist", theDot) == 0) return GraphFormat_adjlist;
			else if (strcasecmp("graphdb", theDot) == 0) return GraphFormat_graphdb;
			else if (!open_file_to_check) {
				if (strcasecmp("txt", theDot) == 0) return GraphFormat_edgelist;
				else if (strcasecmp("dat", theDot) == 0) return GraphFormat_edgelist;
			}
		}
		
		GraphFormat retval = GraphFormat_auto;
		if (open_file_to_check) {
			/// TODO: Fill in this part.
		}
		return retval;
	}
	
#pragma mark -
#pragma mark GraphReader
	
	MEMORY_MANAGER_IMPLEMENTATION_NO_COPYING(GraphReader);
	
	IMPLEMENT_MOVE_METHOD(GraphReader) {
		fptr = ::std::move(other.fptr);
	}
	IMPLEMENT_DEALLOC_METHOD(GraphReader) {
		::std::fclose(fptr);
	}
	
	GraphReader::GraphReader(const char* filename) : fptr(::std::fopen(filename, "r")) { XXINTRNL_DEBUG_CALL_INITIALIZER(GraphReader); }
	GraphReader::GraphReader(std::FILE* filestream) throw() : fptr(filestream), COMMON_INIT_WITH(::tempobj::OwnershipTransferNoOwnership) { XXINTRNL_DEBUG_CALL_INITIALIZER(GraphReader); }
	
	::tempobj::temporary_class<Graph>::type GraphReader::edgelist(const Directedness directedness, EdgelistReadEngine engine) MAY_THROW_EXCEPTION {
		igraph_t _;
		
		if (engine == EdgelistReadEngine_igraph) {
			TRY(igraph_read_graph_edgelist(&_, fptr, 0, directedness));
		} else {
			igraph_vector_t resvec;
			igraph_vector_init(&resvec, 0);
			
			while (!feof(fptr)) {
				int num;
				if (fscanf(fptr, "%d", &num) != 0)
					igraph_vector_push_back(&resvec, num);
				else
					fseek(fptr, 1, SEEK_CUR);
			}
			
			if (igraph_vector_size(&resvec) % 2 != 0)
				igraph_vector_pop_back(&resvec);
			
			igraph_create(&_, &resvec, 0, directedness);
			igraph_vector_destroy(&resvec);
		}
		
		return ::std::move(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	
	::tempobj::temporary_class<Graph>::type GraphReader::adjlist(const Directedness directedness, const EdgeMultiplicity multiplicity, const char* line_separator) MAY_THROW_EXCEPTION {
		int linesep_len = strlen(line_separator);
		long linesep_rewind = 0;
		for (int i = 1; i < linesep_len; ++ i)
			if (line_separator[i] == line_separator[0]) {
				linesep_rewind = i - linesep_len;
				break;
			}
		
		char* linesep_checker = reinterpret_cast<char*>(alloca(linesep_len+1));
		linesep_checker[linesep_len] = '\0';
		
		// Problem: We're managing the adjlist ourselves. Is it OK to do so?
		int capacity = 16;
		int length = 0;
		igraph_adjlist_t adjlist;
		adjlist.length = length;
		adjlist.adjs = reinterpret_cast<igraph_vector_t*>(::std::calloc(capacity, sizeof(igraph_vector_t)));
		
		igraph_vector_t resvec;
		igraph_vector_init(&resvec, 0);
		int num;
		int head_vertex = -1;
		
		while (!feof(fptr)) {
			if (fscanf(fptr, "%d", &num) != 0) {
				if (num > length) {
					length = num;
					if (num > capacity) {
						unsigned old_capacity = capacity;
						capacity *= 2;	// TODO: Nonlinear scaling.
						adjlist.adjs = reinterpret_cast<igraph_vector_t*>(::std::realloc(adjlist.adjs, sizeof(igraph_vector_t) * capacity));
						::std::memset(adjlist.adjs - capacity + old_capacity, 0, sizeof(igraph_vector_t) * (capacity - old_capacity));
					}
				}
				if (head_vertex == -1)
					head_vertex = num;
				else 
					igraph_vector_push_back(&resvec, num);
			} else {
				fread(linesep_checker, linesep_len, 1, fptr);
				if (strcmp(linesep_checker, line_separator) == 0) {
					if (head_vertex != -1) {
						igraph_vector_t* target_vector = adjlist.adjs + head_vertex;
						if (target_vector->stor_begin == NULL && target_vector->stor_end == NULL && target_vector->end == NULL) {
							*target_vector = ::std::move(resvec);
							igraph_vector_init(&resvec, 0);
						} else {
							igraph_vector_append(target_vector, &resvec);
							igraph_vector_clear(&resvec);
						}
						head_vertex = -1;
					}
				} else if (linesep_rewind != 0)
					fseek(fptr, SEEK_CUR, linesep_rewind);
			}
		}
		
		igraph_vector_destroy(&resvec);
		
		igraph_t _;
		TRY(igraph_adjlist(&_, &adjlist, directedness, multiplicity));
		
		for (unsigned i = 0; i < adjlist.length; ++ i)
			igraph_vector_destroy(adjlist.adjs + i);
		::std::free(adjlist.adjs);
		
		return ::std::move(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	
	// TODO: ncol, after StringVector is implemented.
	
	::tempobj::temporary_class<Graph>::type GraphReader::lgl(const lglNames names, const lglWeights weights) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_read_graph_lgl(&_, fptr, names, weights));
		return ::std::move(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	
	// TODO: dimacs, after StringVector is implemented.
	
	::tempobj::temporary_class<Graph>::type GraphReader::graphml(const int index) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_read_graph_graphml(&_, fptr, index));
		return ::std::move(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	
	::tempobj::temporary_class<Graph>::type GraphReader::gml() MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_read_graph_gml(&_, fptr));
		return ::std::move(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	
	::tempobj::temporary_class<Graph>::type GraphReader::pajek() MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_read_graph_pajek(&_, fptr));
		return ::std::move(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	
	::tempobj::temporary_class<Graph>::type GraphReader::graphdb(const Directedness directedness) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_read_graph_graphdb(&_, fptr, directedness));
		return ::std::move(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	
#pragma mark -
#pragma mark GraphWriter
	
	MEMORY_MANAGER_IMPLEMENTATION_NO_COPYING(GraphWriter);
	
	IMPLEMENT_MOVE_METHOD(GraphWriter) {
		_ = ::std::move(other._);
		fptr = ::std::move(other.fptr);
	}
	IMPLEMENT_DEALLOC_METHOD(GraphWriter) {
		::std::fclose(fptr);
	}
	
	GraphWriter::GraphWriter(const igraph_t* graph, const char* filename)
		: _(graph), fptr(::std::fopen(filename, "w")), COMMON_INIT_WITH(::tempobj::OwnershipTransferMove) { XXINTRNL_DEBUG_CALL_INITIALIZER(GraphWriter); }
	GraphWriter::GraphWriter(const igraph_t* graph, std::FILE* filestream) throw()
		: _(graph), fptr(filestream), COMMON_INIT_WITH(::tempobj::OwnershipTransferNoOwnership) { XXINTRNL_DEBUG_CALL_INITIALIZER(GraphWriter); }

	void GraphWriter::edgelist(const char* separator, const char* line_separator) MAY_THROW_EXCEPTION {
		if (separator == NULL && line_separator == NULL) {
			TRY(igraph_write_graph_edgelist(_, fptr));
		} else {
			if (separator == NULL) separator = " ";
			if (line_separator == NULL) line_separator = "\n";
			
			Integer ecount = igraph_ecount(_);
			
			for (Edge i = 0; i < ecount; ++ i) {
				Vertex from, to;
				TRY(igraph_edge(_, i, &from, &to));
				fprintf(fptr, "%lg%s%lg%s", from, separator, to, line_separator);
			}
		}
	}
	
	void GraphWriter::adjlist(const char* first_separator, const char* separator, const char* line_separator) MAY_THROW_EXCEPTION {
		igraph_adjlist_t al;
		TRY(igraph_adjlist_init(_, &al, IGRAPH_OUT));
		long vcount = (long)igraph_vcount(_);
		for (long i = 0; i < vcount; ++ i) {
			igraph_vector_t* pList = igraph_adjlist_get(&al, i);
			fprintf(fptr, "%ld%s", i, first_separator);
			long deg = igraph_vector_size(pList);
			for (long j = 0; j < deg; ++ j) {
				if (j != 0)
					fprintf(fptr, "%s", separator);
				fprintf(fptr, "%lg", VECTOR(*pList)[j]);
			}
			fprintf(fptr, "%s", line_separator);
		}
		igraph_adjlist_destroy(&al);
	}
	
	void GraphWriter::ncol(const char* names, const char* weights) MAY_THROW_EXCEPTION {
		TRY(igraph_write_graph_ncol(_, fptr, names, weights));
	}
	
	void GraphWriter::lgl(const char* names, const char* weights, const lglIsolatedVertices isolates) MAY_THROW_EXCEPTION {
		TRY(igraph_write_graph_lgl(_, fptr, names, weights, isolates));
	}
	
	void GraphWriter::dimacs(const Vertex source, const Vertex target, const Vector& capacity_of_each_edge) MAY_THROW_EXCEPTION {
		TRY(igraph_write_graph_dimacs(_, fptr, (long)source, (long)target, &capacity_of_each_edge._));
	}
	
	void GraphWriter::graphml() MAY_THROW_EXCEPTION {
		TRY(igraph_write_graph_graphml(_, fptr));
	}
	
	void GraphWriter::gml(const char* creator) MAY_THROW_EXCEPTION {
		TRY(igraph_write_graph_gml(_, fptr, NULL, creator));
	}
	
	void GraphWriter::gml(const Vector& new_vertex_ids, const char* creator) MAY_THROW_EXCEPTION {
		TRY(igraph_write_graph_gml(_, fptr, &new_vertex_ids._, creator));
	}
	
	void GraphWriter::pajek() MAY_THROW_EXCEPTION { 
		TRY(igraph_write_graph_pajek(_, fptr));
	}
	
	void GraphWriter::dot() MAY_THROW_EXCEPTION {
		TRY(igraph_write_graph_dot(_, fptr));
	}
}

#endif