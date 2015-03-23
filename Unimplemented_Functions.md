Items in <font color='red'>red</font> are more urgent ones that should be done first.

Remove the section if you have done that part. Once everything is completed, this page shall disappear :).

Please mark down those items by <font color='green'>your name</font> if they are under development that take more than one day to avoid duplicating effort.

# 4 About igraph graphs, the basic interface #
## 4.2 The basic interface ##
### 4.2.1 Graph Constructors and Destructors ###
  * `igraph_empty_attr`

# 5 Error Handling #
All unimplemented

# 6 Data structure library #

## 6.4 Sparse matrices ##
All unimplemented.

## 6.5 Stacks ##
All unimplemented.

## 6.6 Double-ended queues ##
All unimplemented.

## 6.7 Maximum and minimum heaps ##
All unimplemented.

## 6.8 String vectors ##
<font color='red'>All unimplemented.</font>

## 6.9 Adjacency lists ##
### 6.9.2 Adjacent edges ###
All unimplemented.
### 6.9.3 Lazy adjacency list for vertices ###
All unimplemented.
### 6.9.4 Lazy adjacency list for edges ###
All unimplemented.

# 7 Graph Generators #

Besides wrapping the original ones, we also need to create the corresponding ones that give simple graphs. See [issue 1](https://code.google.com/p/igraphhpp/issues/detail?id=1). The undone parts are<font color='red'>
<ul><li>Rewire (preserve degree distribution) <font color='green'>(<i>to be implemented by</i> Kenny.)</font>
</font></li></ul>

# 9 Graph, Vertex and Edge Attributes #
All unimplemented.

# 12 Graph Isomorphism #
All unimplemented.

# 14 Generating Layouts for Graph Drawing #
  * `igraph_layout_graphopt`
  * `10.1.4. The DrL layout generator`
  * `igraph_layout_fruchterman_reingold`
  * `igraph_layout_kamada_kawai`
  * `igraph_layout_grid_fruchterman_reingold`
  * `igraph_layout_lgl`
  * `igraph_layout_fruchterman_reingold_3d`
  * `igraph_layout_kamada_kawai_3d`
  * `igraph_layout_merge_dla`

# 15 Reading and Writing Graphs from and to Files #
## 15.1 Simple edge list and similar formats ##
  * `igraph_read_graph_ncol` _requires_ <font color='red'>StringVector</font>
  * `igraph_read_graph_dimacs` _requires_ <font color='red'>StringVector</font>

# 17 Detecting Community Structure #
  * `igraph_community_spinglass_single`
  * `igraph_community_leading_eigenvector_*`

# 20 Bipartite, i.e. two-mode graphs #
All unimplemented.

# 21 Not Graph Related Functions #
All unimplemented (again).