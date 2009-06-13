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

#include <igraph/cpp/graphio.hpp>
#include <cstring>
#include <cstdio>
#if __unix__ || __MACH__
#include <sys/stat.h>
#include <sys/mman.h>
#else if __WINDOWS__
#include <windows.h>
#endif
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
			else if (strcasecmp("adj", theDot) == 0) return GraphFormat_adjacency;
			else if (strcasecmp("adjacency", theDot) == 0) return GraphFormat_adjacency;
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
	
	MEMORY_MANAGER_IMPLEMENTATION(GraphReader);
	
	IMPLEMENT_COPY_METHOD(GraphReader) throw(std::logic_error) {
		throw std::logic_error("GraphReader::mm_raw_copy() called. GraphReader cannot be copied. Use std::move() for assignment.");
	}
	IMPLEMENT_MOVE_METHOD(GraphReader) throw() {
		fptr = ::std::move(fptr);
	}
	IMPLEMENT_DEALLOC_METHOD(GraphReader) {
		::std::fclose(fptr);
	}
	
	GraphReader(const char* filename) : fptr(::std::fopen(filename, "r")) { XXINTRNL_DEBUG_CALL_INITIALIZER(GraphReader); }
	GraphReader(std::FILE* filestream) throw() : fptr(filestream), COMMON_INIT_WITH(OwnershipTransferNoOwnership) { XXINTRNL_DEBUG_CALL_INITIALIZER(GraphReader); }
	
	temporary_class<Graph>::type edgelist(const Directedness directedness, EdgelistReadEngine engine) MAY_THROW_EXCEPTION {
		igraph_t _;
		
		if (engine == EdgelistReadEngine_igraph) {
			TRY(igraph_read_graph_edgelist(&_, fptr, 0, directedness));
		} else {
			igraph_vector_t resvec;
			igraph_vector_init(&resvec, 0);
			
			while (!feof(fptr)) {
				int num;
				if (fscanf("%d", &num, fptr) != 0)
					igraph_vector_push_back(&resvec, num);
				else
					fseek(fptr, 1, SEEK_CUR);
			}
			
			if (igraph_vector_size(&resvec) % 2 != 0)
				igraph_vector_pop_back(&resvec);
			
			igraph_create(&_, &resvec, 0, directedness);
			igraph_vector_destroy(&resvec);
		}
		
		return ::std::move(Graph(&_, OwnershipTransferMove));
	}
	
	/*
	temporary_class<Graph>::type adjacency(const char* line_separator = NULL, const char* comment_seperator = NULL) MAY_THROW_EXCEPTION;
	// TODO: ncol, after StringVector is implemented.
	temporary_class<Graph>::type lgl(const lglNames names = lglNames_Ignore, const lglWeights weights = lglWeights_Ignore) MAY_THROW_EXCEPTION;
	// TODO: dimacs, after StringVector is implemented.
	temporary_class<Graph>::type graphml(const int index = 0) MAY_THROW_EXCEPTION;
	temporary_class<Graph>::type gml() MAY_THROW_EXCEPTION;
	temporary_class<Graph>::type pajek() MAY_THROW_EXCEPTION;
	temporary_class<Graph>::type graphdb(const Directedness directedness = Undirected) MAY_THROW_EXCEPTION;
	*/
	
	
#pragma mark -
#pragma mark GraphWriter
	
	MEMORY_MANAGER_IMPLEMENTATION(GraphWriter);
	
	IMPLEMENT_COPY_METHOD(GraphWriter) throw(std::logic_error) {
		throw std::logic_error("GraphWriter::mm_raw_copy() called. GraphWriter cannot be copied. Use std::move() for assignment.");
	}
	IMPLEMENT_MOVE_METHOD(GraphWriter) throw() {
		_ = ::std::move(other._);
		fptr = ::std::move(fptr);
	}
	IMPLEMENT_DEALLOC_METHOD(GraphWriter) {
		::std::fclose(fptr);
	}
	
	GraphWriter::GraphWriter(const igraph_t* graph, const char* filename)
		: _(graph), fptr(std::fopen(filename, "w")), COMMON_INIT_WITH(OwnershipTransferMove) { XXINTRNL_DEBUG_CALL_INITIALIZER(GraphWriter); }
	GraphWriter::GraphWriter(const igraph_t* graph, std::FILE* filestream) throw()
		: _(graph), fptr(filestream), COMMON_INIT_WITH(OwnershipTransferNoOwnership) { XXINTRNL_DEBUG_CALL_INITIALIZER(GraphWriter); }

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
	
	void GraphWriter::adjacency(const char* first_separator, const char* separator, const char* line_separator) MAY_THROW_EXCEPTION {
		igraph_adjlist_t al;
		TRY(igraph_adjlist_init(_, &al, IGRAPH_OUT));
		long vcount = (long)igraph_vcount(_);
		for (long i = 0; i < vcount; ++ i) {
			igraph_vector_t* pList = igraph_adjlist_get(&al, i);
			fprintf(fptr, "%d%s", i, first_separator);
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