/*

edgeselector.hpp ... Edge selector

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

#ifndef IGRAPH_EDGESELECTOR_HPP
#define IGRAPH_EDGESELECTOR_HPP

#include <igraph/igraph.h>
#include <igraph/cpp/common.hpp>
#include <igraph/cpp/exception.hpp>
#include <igraph/cpp/vector.hpp>

namespace igraph {
	class Graph;
	
	class EdgeSelector {
	private:
		igraph_es_t _;
		EdgeVector retained_vector;
		
	public:
		MEMORY_MANAGER_INTERFACE(EdgeSelector);
		XXINTRNL_WRAPPER_CONSTRUCTOR_INTERFACE(EdgeSelector, igraph_es_t);
		
		/// Return an EdgeSelector of all edges.
		static ::tempobj::temporary_class<EdgeSelector>::type all(EdgeOrderType ordering = OrderByID) MAY_THROW_EXCEPTION;
		
		/// Return a EdgeSelector of the neighbors of a vertex.
		static ::tempobj::temporary_class<EdgeSelector>::type adj(const Vertex which, const NeighboringMode mode = OutNeighbors) MAY_THROW_EXCEPTION;
				
		/// Return a EdgeSelector of nothing.
		static ::tempobj::temporary_class<EdgeSelector>::type none() MAY_THROW_EXCEPTION;
		
		/// Return a EdgeSelector a single edge.
		static ::tempobj::temporary_class<EdgeSelector>::type single(const Edge which) MAY_THROW_EXCEPTION;
		
		/// Return a EdgeSelector with edges identified as content of the vector.
		static ::tempobj::temporary_class<EdgeSelector>::type vector(const EdgeVector& vec, const ::tempobj::OwnershipTransfer transfer = ::tempobj::OwnershipTransferMove) MAY_THROW_EXCEPTION;
		
		// TODO: Implement igraph_es_vector_small()
		
		/// Return an EdgeSelector with edges inside the specified range.
		static ::tempobj::temporary_class<EdgeSelector>::type seq(const Edge fromID, const Edge toID) MAY_THROW_EXCEPTION;
		
		/// Return an EdgeSelector between two vertex sets
		static ::tempobj::temporary_class<EdgeSelector>::type fromto(const VertexSelector& from, const VertexSelector& to) MAY_THROW_EXCEPTION;
		
		/// Return an EdgeSelector of the single edge between the vertices.
		static ::tempobj::temporary_class<EdgeSelector>::type fromto(const Vertex from, const Vertex to) MAY_THROW_EXCEPTION;
		
		/// Return an EdgeSelector defined by the endpoints
		static ::tempobj::temporary_class<EdgeSelector>::type pairs(const VertexVector& vec, const Directedness directedness = Undirected) MAY_THROW_EXCEPTION;
		
		/// ?
		static ::tempobj::temporary_class<EdgeSelector>::type multipairs(const VertexVector& vec, const Directedness directedness = Undirected) MAY_THROW_EXCEPTION;
		
		/// Return an EdgeSelector on a path of vertices.
		static ::tempobj::temporary_class<EdgeSelector>::type path(const VertexVector& vec, const Directedness directedness = Undirected) MAY_THROW_EXCEPTION;
		
		int type() const throw() { return igraph_es_type(&_); }
		bool is_all() throw() { return igraph_es_is_all(&_); }
		
		::tempobj::temporary_class<EdgeVector>::type as_vector(const Graph& g) const MAY_THROW_EXCEPTION;
		Integer size(const Graph& g) const MAY_THROW_EXCEPTION;
		
		friend class Graph;
		friend class EdgeIterator;
	};
	
	MEMORY_MANAGER_INTERFACE_EX(EdgeSelector);
}

#endif