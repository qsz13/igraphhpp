/*

mincut_test.cpp ... test of mincut function

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
#include <igraph/cpp/impl/mincut.cpp>
#include <cstdio>
using namespace std;
using namespace igraph;


int main () {

	Graph g = (Graph::full(5) + Graph::full(5)).add_edge(0,7).add_edge(1,8).add_edge(2,6);
	g.writer(stdout).edgelist();

	Vector p1(1), p2(1), cut(1), capacity(1);
	Integer numcut;
	numcut = g.mincut().get_cut(cut).get_partition1(p1).compute();
	//igraph_mincut(g.get(), &numcut, &p1._, NULL, NULL, NULL);

	assert(numcut == 3);
	assert(p1.sort() == Vector("5 6 7 8 9"));
	assert(cut.sort() == Vector("20 21 22"));

	printf("\nnumcut=%f\n",numcut);
	p1.sort().print();
	cut.sort().print();

	return 0;
}
