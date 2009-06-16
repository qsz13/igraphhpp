/*

vector.cpp ... Wrapper implementation of igraph vector.

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

#ifndef IGRAPH_VECTOR_CPP
#define IGRAPH_VECTOR_CPP

#include <igraph/cpp/vector.hpp>
#include <cstring>

namespace igraph {
	
	MEMORY_MANAGER_IMPLEMENTATION_WITH_TEMPLATE(template<typename T>, BasicVector, <T>);
	
#define BASE Real
#define FUNC(x) igraph_vector_##x
#define TYPE FUNC(t)
#include <igraph/cpp/impl/vector.4.cpp>
#undef TYPE
#undef FUNC
#undef BASE

#define BASE long
#define FUNC(x) igraph_vector_long_##x
#define TYPE FUNC(t)
#include <igraph/cpp/impl/vector.4.cpp>
#undef TYPE
#undef FUNC
#undef BASE
	
#define BASE char
#define FUNC(x) igraph_vector_char_##x
#define TYPE FUNC(t)
#include <igraph/cpp/impl/vector.4.cpp>
#undef TYPE
#undef FUNC
#undef BASE
	
#define BASE igraph_bool_t
#define FUNC(x) igraph_vector_bool_##x
#define TYPE FUNC(t)
#include <igraph/cpp/impl/vector.4.cpp>
#undef TYPE
#undef FUNC
#undef BASE	
			
}

#endif