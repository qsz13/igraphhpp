/*

Graph.cpp ... Implementation for Graphs

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

#ifndef IGRAPH_GRAPH_CPP
#define IGRAPH_GRAPH_CPP

#include <igraph/cpp/graph.hpp>

namespace igraph {
	
	MEMORY_MANAGER_IMPLEMENTATION(Graph);
	XXINTRNL_WRAPPER_CONSTRUCTOR_IMPLEMENTATION(Graph, igraph_t, igraph_copy);
	
	IMPLEMENT_COPY_METHOD(Graph) {
		igraph_copy(&_, &other._);
	}
	IMPLEMENT_MOVE_METHOD(Graph) {
		_ = ::std::move(other._);
	}
	IMPLEMENT_DEALLOC_METHOD(Graph) {
		igraph_destroy(&_);
	}
	
	
	Graph::Graph(Integer size, Directedness directedness) MAY_THROW_EXCEPTION {
		XXINTRNL_DEBUG_CALL_INITIALIZER(Graph);
		TRY(igraph_empty(&_, size, directedness));
	}
	
	temporary_class<Graph>::type Graph::empty(Integer size, Directedness directedness) MAY_THROW_EXCEPTION {
		return ::std::move(Graph(size, directedness));
	}
	
#pragma mark -
#pragma mark Basic Query Operations

	void Graph::edge(const Edge edge_id, Vertex& from, Vertex& to) const MAY_THROW_EXCEPTION { 
		TRY(igraph_edge(&_, edge_id, &from, &to)); 
	}
	Edge Graph::get_eid(const Vertex from, const Vertex to, const Directedness arc) const MAY_THROW_EXCEPTION {
		Edge e;
		TRY(igraph_get_eid(&_, &e, from, to, arc));
		return e;
	}
	
	temporary_class<VertexVector>::type Graph::neighbors(Vertex vid, NeighboringMode neimode) const MAY_THROW_EXCEPTION {
		VertexVector res = VertexVector::zeros(0);
		TRY(igraph_neighbors(&_, &res._, vid, (igraph_neimode_t)neimode));
		return ::std::move(res);
	}
	
	temporary_class<EdgeVector>::type Graph::adjacent(Vertex vid, NeighboringMode neimode) const MAY_THROW_EXCEPTION {
		EdgeVector res = EdgeVector::zeros(0);
		TRY(igraph_adjacent(&_, &res._, vid, (igraph_neimode_t)neimode));
		return ::std::move(res);
	}
	
	temporary_class<Vector>::type Graph::degree(const VertexSelector& vids, NeighboringMode neimode, SelfLoops countLoops) const MAY_THROW_EXCEPTION {
		Vector res = Vector::zeros(vids.size(*this));
		TRY(igraph_degree(&_, &res._, vids._, (igraph_neimode_t)neimode, countLoops));
		return ::std::move(res);
	}
	
#pragma mark -
#pragma mark Adding and Deleting Vertices and Edges

	Graph& Graph::add_edge(const Vertex from, const Vertex to) MAY_THROW_EXCEPTION {
		TRY(igraph_add_edge(&_, from, to));
		return *this;
	}
	
	Graph& Graph::add_edges(const VertexVector& edges, void* const attr) MAY_THROW_EXCEPTION {
		TRY(igraph_add_edges(&_, &edges._, attr));
		return *this;
	}
	
	Graph& Graph::add_vertices(const Integer nv, void* const attr) MAY_THROW_EXCEPTION {
		TRY(igraph_add_vertices(&_, nv, attr));
		return *this;
	}
	
	Graph& Graph::delete_edges(const EdgeSelector& es) MAY_THROW_EXCEPTION {
		TRY(igraph_delete_edges(&_, es._));
		return *this;
	}
	
	Graph& Graph::delete_edge(const Vertex from, const Vertex to) MAY_THROW_EXCEPTION { return delete_edges(EdgeSelector::fromto(from, to)); }
	Graph& Graph::delete_edge(const Edge eid) MAY_THROW_EXCEPTION { return delete_edges(EdgeSelector::single(eid)); }
	
	Graph& Graph::delete_vertices(const VertexSelector& vs) MAY_THROW_EXCEPTION {
		TRY(igraph_delete_vertices(&_, vs._));
		return *this;
	}
	
#pragma mark -
#pragma mark Basic Properties

	bool Graph::are_connected(const Vertex from, const Vertex to) MAY_THROW_EXCEPTION {
		igraph_bool_t res;
		TRY(igraph_are_connected(&_, from, to, &res));
		return res;
	}
	
#pragma mark -
#pragma mark Reading and Writing Graphs from and to Files (GraphWriter)
	
	temporary_class<GraphWriter>::type Graph::writer(const char* filename) const {
		return ::std::move(GraphWriter(&_, filename));
	}
	temporary_class<GraphWriter>::type Graph::writer(std::FILE* filestream) const throw() {
		return ::std::move(GraphWriter(&_, filestream));
	}

	void write(const char* filename, GraphFormat format = GraphFormat_auto) const MAY_THROW_EXCEPTION {
		if (format == GraphFormat_auto)
			format = identify_file_format(filename, false);
		if (format != GraphFormat_auto) {
			GraphWriter writer = get_writer(filename);
			switch (format) {
				case GraphFormat_adjacency:
					writer.adjacency();
					break;
				case GraphFormat_dot:
					writer.dot();
					break;
				case GraphFormat_edgelist:
					writer.edgelist();
					break;
				case GraphFormat_gml:
					writer.gml();
					break;
				case GraphFormat_graphml:
					writer.graphml();
					break;
				case GraphFormat_lgl:
					writer.lgl();
					break;
				case GraphFormat_ncol:
					writer.ncol();
					break;
				case GraphFormat_pajek:
					writer.pajek();
					break;
				default:
					return false;
			}
			return true;
		}
		return false;
	}
	
}

#endif