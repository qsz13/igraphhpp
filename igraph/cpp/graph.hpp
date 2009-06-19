/*

graph.hpp ... Basic graph functions

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

#ifndef IGRAPH_GRAPH_HPP
#define IGRAPH_GRAPH_HPP

#include <igraph/igraph.h>
#include <igraph/cpp/exception.hpp>
#include <igraph/cpp/vertexselector.hpp>
#include <igraph/cpp/edgeselector.hpp>
#include <igraph/cpp/graphio.hpp>
#include <igraph/cpp/vector.hpp>
#include <igraph/cpp/referencevector.hpp>

namespace gsl {
	class Random;
};

namespace igraph {
	class AdjacencyList;
	
	class Graph {
	private:
		igraph_t _;
		
	public:
		enum ToUndirectedMode {
			ToUndirectedMode_Each = IGRAPH_TO_UNDIRECTED_EACH,
			ToUndirectedMode_Collapse = IGRAPH_TO_UNDIRECTED_COLLAPSE,
			// Backward compatibility.
			EachArcToEdge = IGRAPH_TO_UNDIRECTED_EACH,
			CollapseArcs = IGRAPH_TO_UNDIRECTED_COLLAPSE
		};
		
		MEMORY_MANAGER_INTERFACE(Graph);
		XXINTRNL_WRAPPER_CONSTRUCTOR_INTERFACE(Graph, igraph_t);
		
		Graph(Integer size, Directedness directedness = Undirected) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type empty(Integer size, Directedness directedness = Undirected) MAY_THROW_EXCEPTION;
		
#pragma mark -
#pragma mark Basic Query Operations
		long size() const throw() { return (long)igraph_vcount(&_); }
		long edges() const throw() { return (long)igraph_ecount(&_); }
		Integer vcount() const throw() { return igraph_vcount(&_); }
		Integer ecount() const throw() { return igraph_ecount(&_); }
		
		void edge(const Edge edge_id, Vertex& from, Vertex& to) const MAY_THROW_EXCEPTION;
		Edge get_eid(const Vertex from, const Vertex to, const Directedness arc = Undirected) const MAY_THROW_EXCEPTION;
		
		__attribute__((deprecated,warning("Graph::get is deprecated. Use Graph::get_eid instead.")))
		Edge get(const Vertex from, const Vertex to, const Directedness arc = Undirected) const MAY_THROW_EXCEPTION { return get_eid(from, to, arc); }
		__attribute__((deprecated,warning("Graph::end_points is deprecated. Use Graph::edge instead.")))
		void end_points(const Edge edge_id, Vertex& from, Vertex& to) const MAY_THROW_EXCEPTION { edge(edge_id, from, to); }
		
		::tempobj::force_temporary_class<VertexVector>::type neighbors(const Vertex vid, NeighboringMode neimode = OutNeighbors) const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<EdgeVector>::type adjacent(const Vertex vid, NeighboringMode neimode = OutNeighbors) const MAY_THROW_EXCEPTION;
		
		Directedness is_directed() const throw() { return igraph_is_directed(&_) ? Directed : Undirected; }
		
		Integer degree(Vertex i, NeighboringMode neimode = OutNeighbors, SelfLoops countLoops = ContainSelfLoops) const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<Vector>::type degree(const VertexSelector& vids, NeighboringMode neimode = OutNeighbors, SelfLoops countLoops = ContainSelfLoops) const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<Vector>::type degree(NeighboringMode neimode = OutNeighbors, SelfLoops countLoops = ContainSelfLoops) const MAY_THROW_EXCEPTION;
		
#pragma mark -
#pragma mark Adding and Deleting Vertices and Edges
		Graph& add_edge(const Vertex from, const Vertex to) MAY_THROW_EXCEPTION;
		Graph& add_edges(const VertexVector& edges, void* const attr = NULL) MAY_THROW_EXCEPTION;
		Graph& add_vertices(const Integer nv, void* const attr = NULL) MAY_THROW_EXCEPTION;
		Graph& delete_edges(const EdgeSelector& es) MAY_THROW_EXCEPTION;
		
		Graph& delete_edge(const Vertex from, const Vertex to) MAY_THROW_EXCEPTION;
		Graph& delete_edge(const Edge eid) MAY_THROW_EXCEPTION;
		
		Graph& delete_vertices(const VertexSelector& vs) MAY_THROW_EXCEPTION;
		
		__attribute__((deprecated,warning("Graph::connect is deprecated. Use Graph::add_edge instead.")))
		Graph& connect(const Vertex from, const Vertex to) MAY_THROW_EXCEPTION { return add_edge(from, to); }
		__attribute__((deprecated,warning("Graph::disconnect is deprecated. Use Graph::delete_edge instead.")))
		Graph& disconnect(const Vertex from, const Vertex to) MAY_THROW_EXCEPTION { return delete_edge(from, to); }
		__attribute__((deprecated,warning("Graph::disconnect is deprecated. Use Graph::delete_edge instead.")))
		Graph& disconnect(const Edge eid) { return delete_edge(eid); }

#pragma mark -
#pragma mark Deterministic Graph Generators

		enum StarMode {
			StarMode_Out = IGRAPH_STAR_OUT,
			StarMode_In = IGRAPH_STAR_IN,
			StarMode_Undirected = IGRAPH_STAR_UNDIRECTED,
		};
		
		enum TreeMode {
			TreeMode_Out = IGRAPH_TREE_OUT,
			TreeMode_In = IGRAPH_TREE_IN,
			TreeMode_Undirected = IGRAPH_TREE_UNDIRECTED,
		};
	
		enum PeriodicLattice {
			PeriodicLattice_NotPeriodic,
			PeriodicLattice_Periodic,
		};
		
		static ::tempobj::force_temporary_class<Graph>::type create(const VertexVector& edges, const Integer min_size = 0, const Directedness directedness = Undirected) MAY_THROW_EXCEPTION;
		// TODO: igraph_adjacency when Matrix is implemented.
		// TODO: igraph_weighted_adjacency when Matrix is implemented.
		// TODO: igraph_adjlist when AdjacencyList is implemented.
		static ::tempobj::force_temporary_class<Graph>::type adjlist(const AdjacencyList& lst, const Directedness directedness = Undirected, const ToUndirectedMode duplicate_edges = ToUndirectedMode_Collapse) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type star(const Integer n, const StarMode mode = StarMode_Undirected, const Vertex center = 0) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type lattice(const Vector& dimensions, const PeriodicLattice periodic = PeriodicLattice_Periodic, const Integer step = 1, const Directedness directedness = Undirected, const MutualConnections mutual = MutualConnections_NotMutual) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type lattice_2d(const Integer width, const Integer length, const PeriodicLattice periodic = PeriodicLattice_Periodic, const Integer step = 1, const Directedness directedness = Undirected, const MutualConnections mutual = MutualConnections_NotMutual) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type lattice_3d(const Integer width, const Integer length, const Integer height, const PeriodicLattice periodic = PeriodicLattice_Periodic, const Integer step = 1, const Directedness directedness = Undirected, const MutualConnections mutual = MutualConnections_NotMutual) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type ring(const Integer size, const Directedness directedness = Undirected, const MutualConnections mutual = MutualConnections_NotMutual, const PeriodicLattice periodic = PeriodicLattice_Periodic) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type tree(const Integer n, const Integer children, const TreeMode type = TreeMode_Undirected) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type full(const Integer n, const Directedness directedness = Undirected, const SelfLoops loops = NoSelfLoops) MAY_THROW_EXCEPTION;
		__attribute__((deprecated,warning("Graph::complete is deprecated. Use Graph::full instead.")))
		static ::tempobj::force_temporary_class<Graph>::type complete(const Integer n, const Directedness directedness = Undirected, const SelfLoops loops = NoSelfLoops) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type full_citation(const Integer n, const Directedness directedness = Directed) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type famous(const char* name) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type lcf_vector(const Integer n, const Vector& shifts, const Integer repeats) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type atlas(const int number) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type de_bruijn(const Integer m, const Integer n) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type kautz(const Integer m, const Integer n) MAY_THROW_EXCEPTION;
		// TODO: igraph_extended_chordal_ring when Matrix is implemented.
		
		Graph& connect_neighborhood(const Integer order = 1, const NeighboringMode neimode = OutNeighbors) MAY_THROW_EXCEPTION; 
		
#pragma mark -
#pragma mark Games: Randomized Graph Generators
		enum BarabasiOutPref {
			BarabasiOutPref_InDegreeOnly,
			BarabasiOutPref_TotalDegree,
		};		
		
		static ::tempobj::force_temporary_class<Graph>::type grg_game(const Integer size, const Real radius, const PeriodicLattice periodic = PeriodicLattice_Periodic) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type grg_game(const Integer size, const Real radius, const PeriodicLattice periodic, Vector& x_coords, Vector& y_coords) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type barabasi_game(const Integer size, const Integer m, const Directedness directed = Undirected, const BarabasiOutPref outpref = BarabasiOutPref_InDegreeOnly) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type barabasi_game(const Integer size, const Vector& outseq, const Directedness directed = Undirected, const BarabasiOutPref outpref = BarabasiOutPref_InDegreeOnly) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type barabasi_game_simple(const Integer size, const Integer m) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type barabasi_game_simple(const ::gsl::Random& rangen, const Integer size, const Integer m) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type watts_strogatz_game(const Integer size, const Integer K, const Real p, const Integer dimensions = 1) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type watts_strogatz_game_simple(const Integer size, const Integer K, const Real p, const Integer dimensions = 1) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type watts_strogatz_game_simple(const ::gsl::Random& rangen, const Integer size, const Integer K, const Real p, const Integer dimensions = 1) MAY_THROW_EXCEPTION;
		
		Graph& rewire_edges(const Real prob) MAY_THROW_EXCEPTION;
		/// Randomly rewire edges and produce a simple graph.
		Graph& rewire_edges_simple(const Real prob) MAY_THROW_EXCEPTION;
		Graph& rewire_edges_simple(const ::gsl::Random& rangen, const Real prob) MAY_THROW_EXCEPTION;
				
#pragma mark -
#pragma mark Basic Properties
		bool are_connected(const Vertex from, const Vertex to) MAY_THROW_EXCEPTION;
		
#pragma mark -
#pragma mark Graph Components
		enum Connectedness {
			WeaklyConnected = IGRAPH_WEAK,
			StronglyConnected = IGRAPH_STRONG
		};
		
		::tempobj::force_temporary_class<VertexVector>::type subcomponent(const Vertex representative, const NeighboringMode mode = OutNeighbors) const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<Graph>::type subgraph(const VertexSelector& vs) const MAY_THROW_EXCEPTION;
		void cluster(Vector& cluster_id_each_vertex_belongs_to, Vector& size_of_each_cluster, Connectedness connectedness = WeaklyConnected) const MAY_THROW_EXCEPTION;
		Integer cluster_count(const Connectedness connectedness = WeaklyConnected) const MAY_THROW_EXCEPTION;
		
		bool is_connected(const Connectedness connectedness = WeaklyConnected) const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<ReferenceVector<Graph> >::type decompose(Connectedness connectedness = WeaklyConnected, long max_component_number = -1, long min_size_of_components = -1);
		// TODO: igraph_biconnected_components
		::tempobj::force_temporary_class<VertexVector>::type articulation_points() const MAY_THROW_EXCEPTION;

#pragma mark -
#pragma mark 10.9 Transitivity or Clustering Coefficient

	Real transitivity() const MAY_THROW_EXCEPTION;
	Real transitivity_local(Vertex i) const MAY_THROW_EXCEPTION;
	::tempobj::force_temporary_class<Vector>::type transitivity_local(const VertexSelector& vids) const MAY_THROW_EXCEPTION;
	::tempobj::force_temporary_class<Vector>::type transitivity_local() const MAY_THROW_EXCEPTION;
	Real transitivity_avglocal() const MAY_THROW_EXCEPTION;

#pragma mark -
#pragma mark 10.10 Directedness conversion
		enum ToDirectedMode {
			ToDirectedMode_Arbitrary = IGRAPH_TO_DIRECTED_ARBITRARY,
			ToDirectedMode_Mutual = IGRAPH_TO_DIRECTED_MUTUAL,
			// Backward compatibility.
			EachEdgeToArc = IGRAPH_TO_DIRECTED_ARBITRARY,
			SplitEdges = IGRAPH_TO_DIRECTED_MUTUAL
		};
		
		Graph& to_undirected(const ToDirectedMode mode = ToDirectedMode_Mutual) MAY_THROW_EXCEPTION;
		Graph& to_directed(const ToUndirectedMode mode = ToUndirectedMode_Collapse) MAY_THROW_EXCEPTION;
		
#pragma mark -
#pragma mark Non-simple graphs: multiple and loop edges
		bool is_simple() const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<BoolVector>::type is_loop(const EdgeSelector& es) const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<BoolVector>::type is_multiple(const EdgeSelector& es) const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<Vector>::type count_multiple(const EdgeSelector& es) const MAY_THROW_EXCEPTION;
		
		Graph& simplify() MAY_THROW_EXCEPTION;
		Graph& simplify_loops() MAY_THROW_EXCEPTION;
		Graph& simplify_multiple_edges() MAY_THROW_EXCEPTION;
		
		
#pragma mark -
#pragma mark Cliques
		::tempobj::force_temporary_class<ReferenceVector<Vector> >::type cliques(const Integer max_size = 0) const;
		::tempobj::force_temporary_class<ReferenceVector<Vector> >::type cliques(const Integer min_size, const Integer max_size) const;
		::tempobj::force_temporary_class<ReferenceVector<Vector> >::type largest_cliques() const;
		::tempobj::force_temporary_class<ReferenceVector<Vector> >::type maximal_cliques() const;
		Integer clique_number() const MAY_THROW_EXCEPTION;
		
#pragma mark -
#pragma mark Reading and Writing Graphs from and to Files
		/// Construct a GraphWriter for writing the Graph into a file.
		/// The Graph's lifetime must be longer than the GraphWriter's. 
		::tempobj::force_temporary_class<GraphWriter>::type writer(const char* filename) const;
		::tempobj::force_temporary_class<GraphWriter>::type writer(std::FILE* filestream) const throw();
		
		/**
		 \brief Write to file and match the type by file extension.
		 
		 The type being matched follows this association:
		   - .ncol                                => ncol
		   - .lgl                                 => lgl
		   - .graphml                             => GraphML
		   - .dot, .graphviz                      => GraphViz
		   - .net, .pajek                         => Pajek
		   - .dimacs                              => DIMACS
		   - .edgelist, .edges, .edge, .dat, .txt => Edge list
		   - .adjlist                             => Adjacency list
		 
		 If the type cannot be determined, nothing will be written.
		 
		 \return Whether it has successfully written the file or not.
		 */
		bool write(const char* filename, GraphFormat format = GraphFormat_auto) const;
		
		static ::tempobj::force_temporary_class<GraphReader>::type reader(const char* filename);
		static ::tempobj::force_temporary_class<GraphReader>::type reader(std::FILE* filestream) throw();
		
		static ::tempobj::force_temporary_class<Graph>::type read(const char* filename, GraphFormat format = GraphFormat_auto);
		
#pragma mark -
#pragma mark 18. Graph Operators

		static ::tempobj::force_temporary_class<Graph>::type disjoint_union(const Graph& x, const Graph& y) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type disjoint_union(const ReferenceVector<Graph>& g) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type merge(const Graph& x, const Graph& y) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type merge(const ReferenceVector<Graph>& g) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type intersection(const Graph& x, const Graph& y) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type intersection(const ReferenceVector<Graph>& g) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type difference(const Graph& x, const Graph& y) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type complementer(const Graph& x, SelfLoops loops=NoSelfLoops) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type compose(const Graph& x, const Graph& y) MAY_THROW_EXCEPTION;

		::tempobj::force_temporary_class<Graph>::type operator^ (const Graph& other) MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<Graph>::type operator| (const Graph& other) MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<Graph>::type operator& (const Graph& other) MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<Graph>::type operator- (const Graph& other) MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<Graph>::type operator~ () MAY_THROW_EXCEPTION;
		Graph& operator^= (const Graph& other) MAY_THROW_EXCEPTION;
		Graph& operator|= (const Graph& other) MAY_THROW_EXCEPTION;
		Graph& operator&= (const Graph& other) MAY_THROW_EXCEPTION;
		Graph& operator-= (const Graph& other) MAY_THROW_EXCEPTION;

		friend class VertexSelector;
		friend class EdgeSelector;
		friend class VertexIterator;
		friend class EdgeIterator;
		friend class AdjacencyList;
	};
	
	MEMORY_MANAGER_INTERFACE_EX(Graph);
}

#endif
