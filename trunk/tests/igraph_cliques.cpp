/*

igraph_cliques.cpp ... Test if Graph::cliques return correct results

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

// Converted from igraph's examples/simple/igraph_cliques.c

#include <igraph/igraph.hpp>
#include <cstdio>
using namespace std;
using namespace igraph;

struct Interval {
	Integer min, max;
};

int main () {
	const Interval params[] = {{4, -1}, {2, 2}, {0, 0}, {-1, -1}};
	
	Graph g = Graph::full(6).delete_edges(EdgeSelector::pairs(EdgeVector("0, 1; 0, 2; 3, 5")));
	
	for (unsigned j = 0; j < sizeof(params)/sizeof(Interval); ++ j) {
		ReferenceVector<Vector> result;
		if (params[j].max != 0)
			result = g.cliques(params[j].min, params[j].max);
		else
			result = g.largest_cliques();
		
		long n = result.size();
		printf("%ld cliques found\n", n);
		for (ReferenceVector<Vector>::const_iterator cit = result.begin(); cit != result.end(); ++ cit) {
			printf(" ");
			cit->print();
		}
	}
	
	printf("omega=%lg\n", g.clique_number());
	
	return 0;
}