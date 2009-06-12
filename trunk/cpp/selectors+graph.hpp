/*

selectors+graph.hpp ... Additional implementations for selectors after Graph has been defined.

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

#ifndef IGRAPH_SELECTORS_GRAPH_HPP
#define IGRAPH_SELECTORS_GRAPH_HPP

#include <igraph/cpp/graph.hpp>

namespace igraph {
	Vector VertexSelector::as_vector(const Graph& g) throw(Exception) {
		igraph_vector_t res;
		TRY(igraph_vs_as_vector(&g._, _, &res));
		return Vector(res, OwnershipTransferMove);
	}

	Integer VertexSelector::size(const Graph& g) throw(Exception) {
		Integer s = 0;
		TRY(igraph_vs_size(&g._, &_, &s));
		return s;
	}
	
	Vector EdgeSelector::as_vector(const Graph& g) throw(Exception) {
		igraph_vector_t res;
		TRY(igraph_es_as_vector(&g._, _, &res));
		return Vector(res, OwnershipTransferMove);
	}
	
	Integer EdgeSelector::size(const Graph& g) throw(Exception) {
		Integer s = 0;
		TRY(igraph_es_size(&g._, &_, &s));
		return s;
	}
}

#endif