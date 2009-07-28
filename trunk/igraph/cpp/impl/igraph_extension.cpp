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


#ifndef IGRAPH_EXTENSION_CPP
#define IGRAPH_EXTENSION_CPP

#include <igraph/cpp/graph.hpp>

/**
  Return the maximum component in a graph.

  \param graph The original graph.
  \param res The subgraph with maximum size of the original graph.
  \param mode Either \c IGRAPH_WEAK or \c IGRAPH_STRONG for weakly and strongly connected components respectively. Right now only the former is implemented.
  \return error code

  Time complexity: O(|V|+|E|), the number of vertices plus the number of edges.
*/
int igraph_max_component(const igraph_t* graph, igraph_t* res, igraph_connectedness_t mode) {
	igraph_vector_ptr_t tmp;
	long i, sz, max_index=0;

	IGRAPH_CHECK( igraph_vector_ptr_init(&tmp, 0) );
	IGRAPH_CHECK( igraph_decompose(graph, &tmp, mode, -1, -1) );
	sz = igraph_vector_ptr_size(&tmp);
	if(sz <= 0)  {
		IGRAPH_CHECK( igraph_empty(res, 0, 0) );
		return 0;
	}

	for(i=0; i<sz; ++i)  if(igraph_vcount((igraph_t*)VECTOR(tmp)[i]) > igraph_vcount((igraph_t*)VECTOR(tmp)[max_index]))
		max_index = i;
	*res = *((igraph_t*)(VECTOR(tmp)[max_index]));	// use shallow copy of igraph
	for(i=0; i<sz; ++i)  if(i != max_index)
		igraph_destroy((igraph_t*)&VECTOR(tmp)[i]);
	igraph_vector_ptr_destroy(&tmp);

	return 0;
}

#endif
