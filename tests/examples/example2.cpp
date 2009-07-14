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
#include <cstdio>
using namespace std;
using namespace igraph;


int main () {

	Vector dimvector("30 30");
	Graph g = Graph::lattice(dimvector);

	printf("Average path length (lattice):            %f\n", (double)g.average_path_length() );

	Vector edges(20);
	srand(100);
	for(int i=0; i<edges.size(); i++)
		edges[i] = rand() % (int)g.size();

	g.add_edges(edges);
	printf("Average path length (randomized lattice): %f\n", (double)g.average_path_length() );

	return 0;
}
