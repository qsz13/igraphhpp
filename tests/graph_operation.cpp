/*

igraph_cliques.cpp ... Test if Graph::cliques return correct results

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

// Converted from igraph's examples/simple/igraph_cliques.c

#include <igraph/igraph.hpp>
#include <cstdio>
using namespace std;
using namespace igraph;

int main () {    
	Graph F = Graph::full(5);
	Graph G = Graph::full(4);
	Graph H = Graph::full(3);
	//ReferenceVector<Graph> glist(3l);

	printf("\nGraph operation fucntion test\n----------------\n");
	Graph::disjoint_union(F,H).write("test1.txt",GraphFormat_adjlist);
	//Graph::disjoint_union(glist).write("test2.txt",GraphFormat_adjlist);
	Graph::merge(F,H).write("test3.txt",GraphFormat_adjlist);
	//Graph::merge(glist).write("test4.txt",GraphFormat_adjlist);
	Graph::intersection(F,H).write("test5.txt",GraphFormat_adjlist);
	//Graph::intersection(glist).write("test6.txt",GraphFormat_adjlist);
	Graph::difference(F,H).write("test7.txt",GraphFormat_adjlist);
	Graph::complementer(F).write("test8.txt",GraphFormat_adjlist);
	Graph::compose(F,H).write("test9.txt",GraphFormat_adjlist);

	printf("\nGraph operator test\n----------------\n");
	(F ^ H).write("test11.txt",GraphFormat_adjlist);
	(F | H).write("test12.txt",GraphFormat_adjlist);
	(F & H).write("test13.txt",GraphFormat_adjlist);
	(F - H).write("test14.txt",GraphFormat_adjlist);

	printf("\nGraph operators combination test\n----------------\n");
	Graph t1 = F - G;
	t1.write("test21.txt",GraphFormat_adjlist);
	Graph t2 = (F - G) | H;
	t2.write("test22.txt",GraphFormat_adjlist);
	Graph t3 = (F - G) ^ (H & F);
	t3.write("test23.txt",GraphFormat_adjlist);

	printf("\nTest end\n----------------\n");
	
	return 0;
}
