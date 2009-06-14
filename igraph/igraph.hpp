/*

igraph.hpp ... C++ wrapper for igraph

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

#ifndef IGRAPH_HPP
#define IGRAPH_HPP


#include <igraph/cpp/common.hpp>
#include <igraph/cpp/exception.hpp>

#include <igraph/cpp/vector.hpp>
#include <igraph/cpp/pointervector.hpp>

#include <igraph/cpp/graph.hpp>
#include <igraph/cpp/graphio.hpp>

#include <igraph/cpp/vertexselector.hpp>
#include <igraph/cpp/vertexiterator.hpp>
#include <igraph/cpp/edgeselector.hpp>
#include <igraph/cpp/edgeiterator.hpp>

//------------------------------------------------------------------------------

#include <igraph/cpp/impl/vector.cpp>
#include <igraph/cpp/impl/pointervector.cpp>

#include <igraph/cpp/impl/vertexselector.cpp>
#include <igraph/cpp/impl/edgeselector.cpp>

#include <igraph/cpp/impl/graphio.cpp>
#include <igraph/cpp/impl/graph.cpp>

#include <igraph/cpp/impl/iterators.cpp>

#endif