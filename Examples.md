

# Introduction #

A prior to use the igraph, the igraph library must be installed (http://igraph.sf.net/). The header file of the C++ wrapper is igraph.hpp. All related components such as `Graph`, `Vector`, `Matrix`, etc. are available in the namespace `igraph` by simply including the igraph.hpp. In the following examples, we assume the following is used

```
#include <cstdio>
#include <igraph/igraph.hpp>
using namespace std;
using namespace igraph;
```

The three examples below are translated from the examples in the original igraph documentation.

## Example 1 ##

```
int main () {

	Graph g = Graph::erdos_renyi_game(1000, 5.0/1000);
	Real diameter = g.diameter();
	printf("Diameter of a random graph with average degree 5: %lf\n", diameter);

	return 0;
}
```

## Example 2 ##
```
int main () {

	Vector dimvector("30 30");
	Graph g = Graph::lattice(dimvector);

	printf("Average path length (lattice):            %lf\n", g.average_path_length() );

	Vector edges(20);
	srand(100);
	for(int i=0; i<edges.size(); i++)
		edges[i] = rand() % (int)g.size();

	g.add_edges(edges);
	printf("Average path length (randomized lattice): %lf\n", g.average_path_length() );

	return 0;
}
```

## Example 3 ##
```
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
```