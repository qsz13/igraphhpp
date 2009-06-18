/*

iterators.cpp ... Implementations for vertex/edge iterators.

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

#ifndef IGRAPH_ITERATORS_CPP
#define IGRAPH_ITERATORS_CPP

#include <igraph/cpp/graph.hpp>
#include <igraph/cpp/vertexiterator.hpp>
#include <igraph/cpp/edgeiterator.hpp>

namespace igraph {
	RETRIEVE_TEMPORARY_CLASS(VertexVector) VertexIterator::as_vector() const MAY_THROW_EXCEPTION {
		igraph_vector_t res;
		TRY(igraph_vit_as_vector(&_, &res));
		return FORCE_STD_MOVE(VertexVector)(VertexVector(&res, ::tempobj::OwnershipTransferMove));
	}
	
	RETRIEVE_TEMPORARY_CLASS(EdgeVector) EdgeIterator::as_vector() const MAY_THROW_EXCEPTION {
		igraph_vector_t res;
		TRY(igraph_eit_as_vector(&_, &res));
		return FORCE_STD_MOVE(EdgeVector)(EdgeVector(&res, ::tempobj::OwnershipTransferMove));
	}
}

#endif