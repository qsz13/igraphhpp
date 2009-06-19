/*

edgeselector.cpp ... Edge selector implementation

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

#ifndef IGRAPH_EDGESELECTOR_CPP
#define IGRAPH_EDGESELECTOR_CPP

#include <igraph/cpp/graph.hpp>
#include <igraph/cpp/vertexselector.hpp>
#include <igraph/cpp/edgeselector.hpp>

namespace igraph {
	MEMORY_MANAGER_IMPLEMENTATION(EdgeSelector);
	XXINTRNL_WRAPPER_CONSTRUCTOR_IMPLEMENTATION(EdgeSelector, igraph_es_t, igraph_es_copy);
	
	IMPLEMENT_COPY_METHOD(EdgeSelector) {
		TRY(igraph_es_copy(&_, &other._));
		retained_vector = other.retained_vector;
	}
	
	IMPLEMENT_MOVE_METHOD(EdgeSelector) {
		_ = ::std::move(other._);
		retained_vector = ::tempobj::force_move(other.retained_vector);
	}
	
	IMPLEMENT_DEALLOC_METHOD(EdgeSelector) {
		igraph_es_destroy(&_);
	}
	
	
	/// Return an EdgeSelector of all edges.
	::tempobj::force_temporary_class<EdgeSelector>::type EdgeSelector::all(EdgeOrderType ordering) MAY_THROW_EXCEPTION {
		igraph_es_t _ = igraph_ess_all((igraph_edgeorder_type_t)ordering);
		return ::tempobj::force_move(EdgeSelector(&_, ::tempobj::OwnershipTransferNoOwnership));
	}
	
	/// Return a EdgeSelector of the neighbors of a vertex.
	::tempobj::force_temporary_class<EdgeSelector>::type EdgeSelector::adj(const Vertex which, const NeighboringMode mode) MAY_THROW_EXCEPTION {
		igraph_es_t _;
		TRY(igraph_es_adj(&_, which, (igraph_neimode_t)mode));
		return ::tempobj::force_move(EdgeSelector(&_, ::tempobj::OwnershipTransferMove));
	}
	
	/// Return a EdgeSelector of nothing.
	::tempobj::force_temporary_class<EdgeSelector>::type EdgeSelector::none() MAY_THROW_EXCEPTION {
		igraph_es_t _ = igraph_ess_none();
		return ::tempobj::force_move(EdgeSelector(&_, ::tempobj::OwnershipTransferNoOwnership));
	}
	
	/// Return a EdgeSelector a single edge.
	::tempobj::force_temporary_class<EdgeSelector>::type EdgeSelector::single(const Edge which) MAY_THROW_EXCEPTION {
		igraph_es_t _ = igraph_ess_1(which);
		return ::tempobj::force_move(EdgeSelector(&_, ::tempobj::OwnershipTransferNoOwnership));
	}
	
	/// Return a EdgeSelector with edges identified as content of the vector.
	::tempobj::force_temporary_class<EdgeSelector>::type EdgeSelector::vector(const EdgeVector& vec, const ::tempobj::OwnershipTransfer transfer) MAY_THROW_EXCEPTION {
		igraph_es_t _;
		switch (transfer) {
			case ::tempobj::OwnershipTransferCopy: {
				TRY(igraph_es_vector_copy(&_, &vec._));
				return ::tempobj::force_move(EdgeSelector(&_, ::tempobj::OwnershipTransferMove));
			}
			case ::tempobj::OwnershipTransferMove: {
				_ = igraph_ess_vector(&vec._);
				EdgeSelector es = EdgeSelector(&_, ::tempobj::OwnershipTransferNoOwnership);
				es.retained_vector = ::tempobj::force_move(vec);
				return ::std::move(es);
			}
			case ::tempobj::OwnershipTransferKeepOriginal:
				_ = igraph_ess_vector(&vec._);
				return ::tempobj::force_move(EdgeSelector(&_, ::tempobj::OwnershipTransferNoOwnership));
		}
		std::unexpected();
	}
		
	/// Return an EdgeSelector with edges inside the specified range.
	::tempobj::force_temporary_class<EdgeSelector>::type EdgeSelector::seq(const Edge fromID, const Edge toID) MAY_THROW_EXCEPTION {
		igraph_es_t _ = igraph_ess_seq(fromID, toID);
		return ::tempobj::force_move(EdgeSelector(&_, ::tempobj::OwnershipTransferNoOwnership));
	}
	
	/// Return an EdgeSelector between two vertex sets
	::tempobj::force_temporary_class<EdgeSelector>::type EdgeSelector::fromto(const VertexSelector& from, const VertexSelector& to) MAY_THROW_EXCEPTION {
		igraph_es_t _;
		TRY(igraph_es_fromto(&_, from._, to._));
		return ::tempobj::force_move(EdgeSelector(&_, ::tempobj::OwnershipTransferMove));
	}
	
	/// Return an EdgeSelector of the single edge between the vertices.
	::tempobj::force_temporary_class<EdgeSelector>::type EdgeSelector::fromto(const Vertex from, const Vertex to) MAY_THROW_EXCEPTION {
		return ::std::move(EdgeSelector::fromto(VertexSelector::single(from), VertexSelector::single(to)));
	}
	
	/// Return an EdgeSelector defined by the endpoints
	::tempobj::force_temporary_class<EdgeSelector>::type EdgeSelector::pairs(const VertexVector& vec, const Directedness directedness) MAY_THROW_EXCEPTION {
		igraph_es_t _;
		TRY(igraph_es_pairs(&_, &vec._, directedness));
		return ::tempobj::force_move(EdgeSelector(&_, ::tempobj::OwnershipTransferMove));
	}
	
	/// ?
	::tempobj::force_temporary_class<EdgeSelector>::type EdgeSelector::multipairs(const VertexVector& vec, const Directedness directedness) MAY_THROW_EXCEPTION {
		igraph_es_t _;
		TRY(igraph_es_multipairs(&_, &vec._, directedness));
		return ::tempobj::force_move(EdgeSelector(&_, ::tempobj::OwnershipTransferMove));
	}
	
	/// Return an EdgeSelector on a path of vertices.
	::tempobj::force_temporary_class<EdgeSelector>::type EdgeSelector::path(const VertexVector& vec, const Directedness directedness) MAY_THROW_EXCEPTION {
		igraph_es_t _;
		TRY(igraph_es_path(&_, &vec._, directedness));
		return ::tempobj::force_move(EdgeSelector(&_, ::tempobj::OwnershipTransferMove));
	}
		
	::tempobj::force_temporary_class<EdgeVector>::type EdgeSelector::as_vector(const Graph& g) const MAY_THROW_EXCEPTION {
		igraph_vector_t res;
		igraph_vector_init(&res, 0);
		TRY(igraph_es_as_vector(&g._, _, &res));
		return ::tempobj::force_move(EdgeVector(&res, ::tempobj::OwnershipTransferMove));
	}
	
	Integer EdgeSelector::size(const Graph& g) const MAY_THROW_EXCEPTION {
		Integer s = 0;
		TRY(igraph_es_size(&g._, &_, &s));
		return s;
	}
}

#endif