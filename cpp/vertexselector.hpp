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
#include <igraph/cpp/vector.hpp>

namespace igraph {
	class EdgeSelector;
	class Graph;
	
	class VertexSelector {
	private:
		igraph_vs_t _;
		Vector retained_vector;
		
		VertexSelector() throw() {
			retained_vector.mm_dont_dealloc = true;
		};
		
	public:
		MEMORY_MANAGER_INTERFACE(VertexSelector);
		
		/// Return a VertexSelector of all vertices.
		static VertexSelector all() throw(Exception) {
			VertexSelector vs;
			TRY(igraph_vs_all(&vs._));
			return vs;
		}
		
		/// Return a VertexSelector of the neighbors of a vertex.
		static VertexSelector adj(const Vertex which, const NeighboringMode mode = AllNeighbors) throw(Exception) {
			VertexSelector vs;
			TRY(igraph_vs_adj(&vs._, which, (igraph_neimode_t)mode));
			return vs;
		}
		
		/// Return a VertexSelector of the non-neighbors of a vertex.  
		static VertexSelector nonadj(const Vertex which, const NeighboringMode mode = AllNeighbors) throw(Exception) {
			VertexSelector vs;
			TRY(igraph_vs_nonadj(&vs._, which, (igraph_neimode_t)mode));
			return vs;
		}
		
		/// Return a VertexSelector of nothing.
		static VertexSelector none() throw(Exception) {
			VertexSelector vs;
			TRY(igraph_vs_none(&vs._));
			return vs;
		}
		
		/// Return a VertexSelector a single vertex.
		static VertexSelector single(const Vertex which) throw(Exception) {
			VertexSelector vs;
			TRY(igraph_vs_1(&vs._, which));
			return vs;
		}
		
		/// Return a VertexSelector with vertices identified as content of the vector.
		static VertexSelector vector(const Vector& vec, const OwnershipTransfer transfer = OwnershipTransferKeepOriginal) throw(Exception) {
			VertexSelector vs;
			switch (transfer) {
				case OwnershipTransferCopy:
					TRY(igraph_vs_vector_copy(&vs._, &vec._));
					break;
				case OwnershipTransferMove:
					vs.retained_vector = std::move(vec);
					TRY(igraph_vs_vector(&vs._, &vs.retained_vector._));
					break;
				case OwnershipTransferKeepOriginal:
					TRY(igraph_vs_vector(&vs._, &vec._));
					break;
			}
			return vs;
		}
				
		// TODO: Implement igraph_vs_vector_small()
		
		/// Return a VertexSelector with vertices inside the specified range.
		static VertexSelector seq(const Vertex fromID, const Vertex toID) throw(Exception) {
			VertexSelector vs;
			TRY(igraph_vs_seq(&vs._, fromID, toID));
			return vs;
		}
		
		bool is_all() throw(Exception) { return igraph_vs_is_all(&_); }
				
		Vector as_vector(const Graph& g) throw(Exception);
		Integer size(const Graph& g) throw(Exception);	
		friend class Graph;
		friend class EdgeSelector;
		friend class VertexIterator;
	};
	
	MEMORY_MANAGER_IMPLEMENTATION(VertexSelector);
	
	IMPLEMENT_COPY_METHOD(VertexSelector) {
		igraph_vs_copy(&_, &other._);
		retained_vector = other.retained_vector;
	}
	
	IMPLEMENT_MOVE_METHOD(VertexSelector) {
		_ = std::move(other._);
		retained_vector = std::move(other.retained_vector);
	}
	
	IMPLEMENT_DEALLOC_METHOD(VertexSelector) {
		igraph_vs_destroy(&_);
	}
}

#endif