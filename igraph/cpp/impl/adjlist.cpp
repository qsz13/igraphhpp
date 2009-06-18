/*

adjlist.cpp ... Adjacency lists.

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

#include <igraph/cpp/adjlist.hpp>
#include <igraph/cpp/graph.hpp>
#include <stdexcept>

namespace igraph {
	MEMORY_MANAGER_IMPLEMENTATION_NO_COPYING(AdjacencyList);
	XXINTRNL_WRAPPER_CONSTRUCTOR_IMPLEMENTATION(AdjacencyList, igraph_adjlist_t, XXINTRNL_nop);
	
	IMPLEMENT_MOVE_METHOD(AdjacencyList) {
		_ = ::std::move(other._);
	}
	IMPLEMENT_DEALLOC_METHOD(AdjacencyList) {
		igraph_adjlist_destroy(&_);
	}
	
	AdjacencyList::AdjacencyList(const Graph& g, NeighboringMode mode) MAY_THROW_EXCEPTION {
		TRY(igraph_adjlist_init(&g._, &_, (igraph_neimode_t)mode));
	}
	
	RETRIEVE_TEMPORARY_CLASS(AdjacencyList) AdjacencyList::complementer(const Graph& g, NeighboringMode mode, SelfLoops loop) MAY_THROW_EXCEPTION {
		igraph_adjlist_t _;
		TRY(igraph_adjlist_init_complementer(&g._, &_, (igraph_neimode_t)mode, loop));
		return FORCE_STD_MOVE(AdjacencyList)(AdjacencyList(&_, ::tempobj::OwnershipTransferMove));
	}
	
	RETRIEVE_TEMPORARY_CLASS(Vector) AdjacencyList::operator[](const Vertex v) const throw() {
		return FORCE_STD_MOVE(Vector)(Vector(igraph_adjlist_get(&_, v), ::tempobj::OwnershipTransferKeepOriginal));
	}
	
	AdjacencyList& AdjacencyList::sort() throw() {
		igraph_adjlist_sort(&_);
		return *this;
	}
	
	AdjacencyList& AdjacencyList::simplify() MAY_THROW_EXCEPTION {
		TRY(igraph_adjlist_simplify(&_));
		return *this;
	}
	
	Integer AdjacencyList::size() const throw() {
		return igraph_adjlist_size(&_);
	}
}