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
#include <igraph/cpp/exception.hpp>
#include <igraph/cpp/vertexselector.hpp>

namespace igraph {
	class Graph;
	class Vector;
	
	class EdgeSelector {
	private:
		igraph_es_t _;
		Vector retained_vector;
		
		EdgeSelector() throw() {
			retained_vector.mm_dont_dealloc = true;
		};
		
	public:
		MEMORY_MANAGER_INTERFACE(EdgeSelector);
		
		/// Return an EdgeSelector of all edges.
		static EdgeSelector all(EdgeOrderType ordering = OrderByID) throw(Exception) {
			EdgeSelector es;
			TRY(igraph_es_all(&es._, (igraph_edgeorder_type_t)ordering));
			return es;
		}
		
		/// Return a EdgeSelector of the neighbors of a vertex.
		static EdgeSelector adj(const Vertex which, const NeighboringMode mode = AllNeighbors) throw(Exception) {
			EdgeSelector es;
			TRY(igraph_es_adj(&es._, which, (igraph_neimode_t)mode));
			return es;
		}
				
		/// Return a EdgeSelector of nothing.
		static EdgeSelector none() throw(Exception) {
			EdgeSelector es;
			TRY(igraph_es_none(&es._));
			return es;
		}
		
		/// Return a EdgeSelector a single edge.
		static EdgeSelector single(const Edge which) throw(Exception) {
			EdgeSelector es;
			TRY(igraph_es_1(&es._, which));
			return es;
		}
		
		/// Return a EdgeSelector with edges identified as content of the vector.
		static EdgeSelector vector(const Vector& vec, const OwnershipTransfer transfer = OwnershipTransferKeepOriginal) throw(Exception) {
			EdgeSelector es;
			switch (transfer) {
				case OwnershipTransferCopy:
					TRY(igraph_es_vector_copy(&es._, &vec._));
					break;
				case OwnershipTransferMove:
					es.retained_vector = std::move(vec);
					TRY(igraph_es_vector(&es._, &es.retained_vector._));
					break;
				case OwnershipTransferKeepOriginal:
					TRY(igraph_es_vector(&es._, &vec._));
					break;
			}
			return es;
		}
		
		// TODO: Implement igraph_es_vector_small()
		
		/// Return an EdgeSelector with edges inside the specified range.
		static EdgeSelector seq(const Edge fromID, const Edge toID) throw(Exception) {
			EdgeSelector es;
			TRY(igraph_es_seq(&es._, fromID, toID));
			return es;
		}
		
		/// Return an EdgeSelector between two vertex sets
		static EdgeSelector fromto(const VertexSelector& from, const VertexSelector& to) throw(Exception) {
			EdgeSelector es;
			TRY(igraph_es_fromto(&es._, from._, to._));
			return es;
		}
		
		/// Return an EdgeSelector defined by the endpoints
		static EdgeSelector pairs(const Vector& vec, const Directedness directedness = Undirected) {
			EdgeSelector es;
			TRY(igraph_es_pairs(&es._, &vec._, directedness));
			return es;
		}
		
		/// ?
		static EdgeSelector multipairs(const Vector& vec, const Directedness directedness = Undirected) {
			EdgeSelector es;
			TRY(igraph_es_multipairs(&es._, &vec._, directedness));
			return es;
		}
		
		/// Return an EdgeSelector on a path of vertices.
		static EdgeSelector path(const Vector& vec, const Directedness directedness = Undirected) {
			EdgeSelector es;
			TRY(igraph_es_path(&es._, &vec._, directedness));
			return es;
		}
		
		bool is_all() throw(Exception) { return igraph_es_is_all(&_); }
		
		Vector as_vector(const Graph& g) throw(Exception);
		Integer size(const Graph& g) throw(Exception);
		
		friend class Graph;
		friend class EdgeIterator;
	};
	
	MEMORY_MANAGER_IMPLEMENTATION(EdgeSelector);
	
	IMPLEMENT_COPY_METHOD(EdgeSelector) {
		igraph_es_copy(&_, &other._);
		retained_vector = other.retained_vector;
	}
	
	IMPLEMENT_MOVE_METHOD(EdgeSelector) {
		_ = std::move(other._);
		retained_vector = std::move(other.retained_vector);
	}
	
	IMPLEMENT_DEALLOC_METHOD(EdgeSelector) {
		igraph_es_destroy(&_);
	}

}

#endif