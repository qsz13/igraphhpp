/*

graph.hpp ... Basic graph functions

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

#ifndef IGRAPH_GRAPH_HPP
#define IGRAPH_GRAPH_HPP

#include <igraph/igraph.h>
#include <igraph/cpp/exception.hpp>
#include <igraph/cpp/edgeselector.hpp>

namespace igraph {
	
	class Graph {
	private:
		igraph_t _;
		
	public:
		MEMORY_MANAGER_INTERFACE(Graph);
		
		Graph(Integer size, Directedness directedness = Undirected) throw(Exception) {
			TRY(igraph_empty(&_, size, directedness));
		}
		static Graph empty(Integer size, Directedness directedness = Undirected) throw(Exception) {
			return Graph(size, directedness);
		}
		
#pragma mark -
#pragma mark Basic Query Operations
		long size() const throw() { return (long)igraph_vcount(&_); }
		long edges() const throw() { return (long)igraph_ecount(&_); }
		Integer vcount() const throw() { return igraph_vcount(&_); }
		Integer ecount() const throw() { return igraph_ecount(&_); }
		
		void edge(const Edge edge_id, Vertex& from, Vertex& to) const throw(Exception) { 
			TRY(igraph_edge(&_, edge_id, &from, &to)); 
		}
		Edge get_eid(const Vertex from, const Vertex to, const Directedness arc = Undirected) const throw(Exception) {
			Edge e;
			TRY(igraph_get_eid(&_, &e, from, to, arc));
			return e;
		}
		__attribute__((deprecated,warning("Graph::get is deprecated. Use Graph::get_eid instead.")))
		Edge get(const Vertex from, const Vertex to, const Directedness arc = Undirected) const throw(Exception) { 
			return get_eid(from, to, arc);
		}
		__attribute__((deprecated,warning("Graph::end_points is deprecated. Use Graph::edge instead.")))
		void end_points(const Edge edge_id, Vertex& from, Vertex& to) const throw(Exception) { 
			edge(edge_id, from, to);
		}
		
#pragma mark -
#pragma mark Adding and Deleting Vertices and Edges
		void add_edge(const Vertex from, const Vertex to) throw(Exception) {
			TRY(igraph_add_edge(&_, from, to));
		}
		
		void add_edges(const Vector& edges, void* const attr = NULL) throw(Exception) {
			TRY(igraph_add_edges(&_, &edges._, attr));
		}
		
		void add_vertices(const Integer nv, void* const attr = NULL) throw(Exception) {
			TRY(igraph_add_vertices(&_, nv, attr));
		}
		
		void delete_edges(const EdgeSelector& es) throw(Exception) {
			TRY(igraph_delete_edges(&_, es._));
		}
		
		void delete_vertices(const VertexSelector& vs) throw(Exception) {
			TRY(igraph_delete_vertices(&_, vs._));
		}
		
#pragma mark -
#pragma mark Basic Properties
		bool connected(const Vertex from, const Vertex to) throw(Exception) {
			igraph_bool_t res;
			TRY(igraph_are_connected(&_, from, to, &res));
			return res;
		}
		
	};
	
	MEMORY_MANAGER_IMPLEMENTATION(Graph);
	IMPLEMENT_COPY_METHOD(Graph) {
		igraph_copy(&_, &other._);
	}
	IMPLEMENT_MOVE_METHOD(Graph) {
		_ = ::std::move(other._);
	}
	IMPLEMENT_DEALLOC_METHOD(Graph) {
		igraph_destroy(&_);
	}
}

#endif