/*
igraph examples

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


#include <igraph/igraph.hpp>
#include <algorithm>
#include <cstdio>
using namespace std;
using namespace igraph;


int main () {

	Vertex edges[] = { 0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 8,
					   0,10, 0,11, 0,12, 0,13, 0,17, 0,19, 0,21, 0,31,
					   1, 2, 1, 3, 1, 7, 1,13, 1,17, 1,19, 1,21, 1,30,
					   2, 3, 2, 7, 2,27, 2,28, 2,32, 2, 9, 2, 8, 2,13,
					   3, 7, 3,12, 3,13, 4, 6, 4,10, 5, 6, 5,10, 5,16,
					   6,16, 8,30, 8,32, 8,33, 9,33,13,33,14,32,14,33,
					  15,32,15,33,18,32,18,33,19,33,20,32,20,33,
					  22,32,22,33,23,25,23,27,23,32,23,33,23,29,
					  24,25,24,27,24,31,25,31,26,29,26,33,27,33,
					  28,31,28,33,29,32,29,33,30,32,30,33,31,32,31,33,
					  32,33
	};

	VertexVector v = VertexVector::view(edges, sizeof(edges)/sizeof(double));
	Graph g = Graph::create(v);

	Vector res;
	res = g.degree();
	printf("Maximum degree is      %10i, vertex %2i.\n", (int)res.max(), (int)res.which_min());

	res = g.closeness(VertexSelector::all());
	printf("Maximum closeness is   %10lf, vertex %2i.\n", res.max(), (int)res.which_min());

	// the support of C++ standard template library
	res = g.betweenness(VertexSelector::all());
	VertexVector::const_iterator it = max_element(res.begin(), res.end());
	printf("Maximum betweenness is %10lf, vertex %2i.\n", *it, (int)(it-res.begin()));

	return 0;
}
