/*
igraph_extension ... collect the set of function that is not in the original igraph

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


#ifndef IGRAPH_EXTENSION_HPP
#define IGRAPH_EXTENSION_HPP

#include <igraph/igraph.h>

int igraph_max_component(const igraph_t* graph, igraph_t* res, igraph_connectedness_t mode);

#endif
