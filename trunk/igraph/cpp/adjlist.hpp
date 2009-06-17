/*

adjlist.hpp ... Adjacency lists.

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

#ifndef IGRAPH_ADJLIST_HPP
#define IGRAPH_ADJLIST_HPP

#include <igraph/cpp/common.hpp>
#include <igraph/cpp/vector.hpp>

namespace igraph {
	class Graph;
	
	class AdjacencyList {
		igraph_adjlist_t _;
	public:
		MEMORY_MANAGER_INTERFACE_NO_COPYING(AdjacencyList);
		XXINTRNL_WRAPPER_CONSTRUCTOR_INTERFACE(AdjacencyList, igraph_adjlist_t);
		
		AdjacencyList(const Graph& g, NeighboringMode mode = OutNeighbors) MAY_THROW_EXCEPTION;
		static RETRIEVE_TEMPORARY_CLASS(AdjacencyList) complementer(const Graph& g, NeighboringMode mode = OutNeighbors, SelfLoops loop = NoSelfLoops) MAY_THROW_EXCEPTION;
		RETRIEVE_TEMPORARY_CLASS(Vector) operator[] (const Vertex v) const throw();
		Integer size() const throw();
		AdjacencyList& sort() throw();
		AdjacencyList& simplify() MAY_THROW_EXCEPTION;
		
		friend class Graph;
	};
	MEMORY_MANAGER_INTERFACE_EX(AdjacencyList);
}

#endif