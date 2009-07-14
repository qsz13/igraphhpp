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

	Graph g = Graph::erdos_renyi_game(1000, 5.0/1000);
	Real diameter = g.diameter();
	printf("Diameter of a random graph with average degree 5: %f\n", (double) diameter);

	return 0;
}
