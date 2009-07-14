/*

arpack.cpp ... wrapper for igraph arpack

Copyright (C) 2009  Hon Wai, LAU <lau65536@gmail.com>

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

#ifndef IGRAPH_ARPACK_CPP
#define IGRAPH_ARPACK_CPP

#include <igraph/igraph.h>
#include <igraph/cpp/exception.hpp>

namespace igraph {

	ArpackOptions::ArpackOptions() {
		igraph_arpack_options_init(&_);
	}


}

#endif
