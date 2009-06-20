/*

vertexselector.hpp ... Vertex selector

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

/**
 \file vertexselector.hpp
 \brief Vertex selector
 \author KennyTM~
 \date June 12th, 2009
 */

#ifndef IGRAPH_VERTEXSELECTOR_HPP
#define IGRAPH_VERTEXSELECTOR_HPP

#include <igraph/igraph.h>
#include <igraph/cpp/common.hpp>
#include <igraph/cpp/exception.hpp>
#include <igraph/cpp/vector.hpp>

namespace igraph {
	class EdgeSelector;
	class Graph;
	
	class VertexSelector {
	private:
		igraph_vs_t _;
		VertexVector retained_vector;
				
	public:
		MEMORY_MANAGER_INTERFACE(VertexSelector);
		XXINTRNL_WRAPPER_CONSTRUCTOR_INTERFACE(VertexSelector, igraph_vs_t);
		
		/// Return a VertexSelector of all vertices.
		static ::tempobj::force_temporary_class<VertexSelector>::type all() MAY_THROW_EXCEPTION;
		
		/// Return a VertexSelector of the neighbors of a vertex.
		static ::tempobj::force_temporary_class<VertexSelector>::type adj(const Vertex which, const NeighboringMode mode = OutNeighbors) MAY_THROW_EXCEPTION;
		
		/// Return a VertexSelector of the non-neighbors of a vertex.  
		static ::tempobj::force_temporary_class<VertexSelector>::type nonadj(const Vertex which, const NeighboringMode mode = OutNeighbors) MAY_THROW_EXCEPTION;
		
		/// Return a VertexSelector of nothing.
		static ::tempobj::force_temporary_class<VertexSelector>::type none() MAY_THROW_EXCEPTION;
		
		/// Return a VertexSelector a single vertex.
		static ::tempobj::force_temporary_class<VertexSelector>::type single(const Vertex which) MAY_THROW_EXCEPTION;
		
		/// Return a VertexSelector with vertices identified as content of the vector.
		static ::tempobj::force_temporary_class<VertexSelector>::type vector(const VertexVector& vec, const ::tempobj::OwnershipTransfer transfer = ::tempobj::OwnershipTransferMove) MAY_THROW_EXCEPTION;
				
		// TODO: Implement igraph_vs_vector_small()
		
		/// Return a VertexSelector with vertices inside the specified range.
		static ::tempobj::force_temporary_class<VertexSelector>::type seq(const Vertex fromID, const Vertex toID) MAY_THROW_EXCEPTION;
		
		int type() const throw();
		bool is_all() MAY_THROW_EXCEPTION;
				
		::tempobj::force_temporary_class<VertexVector>::type as_vector(const Graph& g) const MAY_THROW_EXCEPTION;
		Integer size(const Graph& g) const MAY_THROW_EXCEPTION;
		
		friend class Graph;
		friend class EdgeSelector;
		friend class VertexIterator;
	};
	MEMORY_MANAGER_INTERFACE_EX(VertexSelector);
}

#endif
