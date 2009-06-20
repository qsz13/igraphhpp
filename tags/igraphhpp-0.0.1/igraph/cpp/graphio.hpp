/*

graphio.hpp ... Graph I/O.

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

#ifndef IGRAPH_GRAPHWRITER_HPP
#define IGRAPH_GRAPHWRITER_HPP

#include <igraph/cpp/common.hpp>
#include <igraph/cpp/exception.hpp>
#include <cstdio>

namespace igraph {
	enum GraphFormat {
		GraphFormat_auto,
		
		GraphFormat_edgelist,
		GraphFormat_ncol,
		GraphFormat_lgl,
		GraphFormat_dimacs,
		GraphFormat_graphml,
		GraphFormat_gml,
		GraphFormat_pajek,
		GraphFormat_dot,
		GraphFormat_graphdb,
		GraphFormat_adjlist,
	};
	
	GraphFormat identify_file_format(const char* filename, const bool open_file_to_check = false);
	
	class GraphReader {
	private:
		std::FILE* fptr;
		
	public:
		MEMORY_MANAGER_INTERFACE_NO_COPYING(GraphReader);
		
		enum lglNames {
			lglNames_Ignore,
			lglNames_Read,
		};
		
		enum lglWeights {
			lglWeights_Ignore,
			lglWeights_Read,
		};
		
		enum EdgelistReadEngine {
			EdgelistReadEngine_igraph,
			EdgelistReadEngine_igraphhpp
		};
		
		GraphReader(const char* filename);
		GraphReader(std::FILE* filestream) throw();
		
		::tempobj::temporary_class<Graph>::type edgelist(const Directedness directedness = Undirected, const EdgelistReadEngine engine = EdgelistReadEngine_igraph) MAY_THROW_EXCEPTION;
		::tempobj::temporary_class<Graph>::type adjlist(const Directedness directedness = Undirected, const EdgeMultiplicity multiplicity = EdgeMultiplicity_Simple, const char* line_separator = "\n") MAY_THROW_EXCEPTION;
		// TODO: ncol, after StringVector is implemented.
		::tempobj::temporary_class<Graph>::type lgl(const lglNames names = lglNames_Ignore, const lglWeights weights = lglWeights_Ignore) MAY_THROW_EXCEPTION;
		// TODO: dimacs, after StringVector is implemented.
		::tempobj::temporary_class<Graph>::type graphml(const int index = 0) MAY_THROW_EXCEPTION;
		::tempobj::temporary_class<Graph>::type gml() MAY_THROW_EXCEPTION;
		::tempobj::temporary_class<Graph>::type pajek() MAY_THROW_EXCEPTION;
		::tempobj::temporary_class<Graph>::type graphdb(const Directedness directedness = Undirected) MAY_THROW_EXCEPTION;
	};

	class GraphWriter {
	private:
		const igraph_t* _;
		std::FILE* fptr;
	
		GraphWriter(const igraph_t* graph, const char* filename);
		GraphWriter(const igraph_t* graph, std::FILE* filestream) throw();
		
	public:
		MEMORY_MANAGER_INTERFACE_NO_COPYING(GraphWriter);
		
		enum lglIsolatedVertices {
			lglIsolatedVertices_Ignore,
			lglIsolatedVertices_Write,
		};
		
		void edgelist(const char* separator = NULL, const char* line_separator = NULL) MAY_THROW_EXCEPTION;
		void adjlist(const char* first_separator = ", ", const char* separator = ", ", const char* line_separator = "\n") MAY_THROW_EXCEPTION;
		void ncol(const char* names = NULL, const char* weights = NULL) MAY_THROW_EXCEPTION;
		void lgl(const char* names = NULL, const char* weights = NULL, const lglIsolatedVertices isolates = lglIsolatedVertices_Ignore) MAY_THROW_EXCEPTION;
		void dimacs(const Vertex source, const Vertex target, const Vector& capacity_of_each_edge) MAY_THROW_EXCEPTION;
		void graphml() MAY_THROW_EXCEPTION;
		void gml(const char* creator = NULL) MAY_THROW_EXCEPTION;
		void gml(const Vector& new_vertex_ids, const char* creator = NULL) MAY_THROW_EXCEPTION;
		void pajek() MAY_THROW_EXCEPTION;
		void dot() MAY_THROW_EXCEPTION;
			
		friend class Graph;
	};
	
	MEMORY_MANAGER_INTERFACE_EX_NO_COPYING(GraphReader);
	MEMORY_MANAGER_INTERFACE_EX_NO_COPYING(GraphWriter);
}

#endif
