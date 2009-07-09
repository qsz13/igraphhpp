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
#include <igraph/cpp/community.hpp>

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
		
		const igraph_t* get() const throw() { return &_; }
		
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
		
		Integer degree_of(Vertex i, NeighboringMode neimode = OutNeighbors, SelfLoops countLoops = ContainSelfLoops) const MAY_THROW_EXCEPTION;
		__attribute__((deprecated,warning("Graph::degree(Vertex) is deprecated. Use Graph::degree_of(Vertex) instead.")))
		Integer degree(Vertex i, NeighboringMode neimode = OutNeighbors, SelfLoops countLoops = ContainSelfLoops) const MAY_THROW_EXCEPTION { return degree_of(i, neimode, countLoops); }
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
#pragma mark 7.1 Deterministic Graph Generators

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
		
		enum AdjacencyMode {
			AdjacencyMode_Directed = IGRAPH_ADJ_DIRECTED,
			AdjacencyMode_Undirected = IGRAPH_ADJ_UNDIRECTED,
			AdjacencyMode_Max = IGRAPH_ADJ_MAX,
			AdjacencyMode_Min = IGRAPH_ADJ_MIN,
			AdjacencyMode_Plus = IGRAPH_ADJ_PLUS,
			AdjacencyMode_Upper = IGRAPH_ADJ_UPPER,
			AdjacencyMode_Lower = IGRAPH_ADJ_LOWER,
		};
		
		static ::tempobj::force_temporary_class<Graph>::type create(const VertexVector& edges, const Integer min_size = 0, const Directedness directedness = Undirected) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type adjacency(Matrix& adjmatrix, AdjacencyMode mode) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type weighted_adjacency(Matrix& adjmatrix, AdjacencyMode mode, const char* attr) MAY_THROW_EXCEPTION;
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
		static ::tempobj::force_temporary_class<Graph>::type extended_chordal_ring(Integer nodes, const Matrix& W) MAY_THROW_EXCEPTION;
		
		Graph& connect_neighborhood(const Integer order = 1, const NeighboringMode neimode = OutNeighbors) MAY_THROW_EXCEPTION; 
		
#pragma mark -
#pragma mark 7.2 Games: Randomized Graph Generators
		enum BarabasiOutPref {
			BarabasiOutPref_InDegreeOnly,
			BarabasiOutPref_TotalDegree,
		};
		enum DegSeqGenerator {
			 DegSeqGenerator_Simple = IGRAPH_DEGSEQ_SIMPLE,
			 DegSeqGenerator_VL = IGRAPH_DEGSEQ_VL,
		};
		
		static ::tempobj::force_temporary_class<Graph>::type grg_game(const Integer size, const Real radius, const PeriodicLattice periodic = PeriodicLattice_Periodic) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type grg_game(const Integer size, const Real radius, const PeriodicLattice periodic, Vector& x_coords, Vector& y_coords) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type barabasi_game(const Integer size, const Integer m, const Directedness directed = Undirected, const BarabasiOutPref outpref = BarabasiOutPref_InDegreeOnly) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type barabasi_game(const Integer size, const Vector& outseq, const Directedness directed = Undirected, const BarabasiOutPref outpref = BarabasiOutPref_InDegreeOnly) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type barabasi_game_simple(const Integer size, const Integer m) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type barabasi_game_simple(const ::gsl::Random& rangen, const Integer size, const Integer m) MAY_THROW_EXCEPTION;		
		static ::tempobj::force_temporary_class<Graph>::type nonlinear_barabasi_game(Integer n, Real power, Integer m, const Vector& outseq, Boolean outpref, Real zeroappeal, Directedness directedness = Undirected) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type watts_strogatz_game(const Integer size, const Integer K, const Real p, const Integer dimensions = 1) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type watts_strogatz_game_simple(const Integer size, const Integer K, const Real p, const Integer dimensions = 1) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type watts_strogatz_game_simple(const ::gsl::Random& rangen, const Integer size, const Integer K, const Real p, const Integer dimensions = 1) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type erdos_renyi_game(Integer size, Real prob, Directedness directedness = Undirected, SelfLoops self_loops = NoSelfLoops) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type erdos_renyi_game_simple(const ::gsl::Random& rangen, Integer size, Real prob, Directedness directedness = Undirected, SelfLoops self_loops = NoSelfLoops) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type erdos_renyi_game_simple(Integer size, Real prob, Directedness directedness = Undirected, SelfLoops self_loops = NoSelfLoops) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type erdos_renyi_Gnm_game(Integer size, Integer edges, Directedness directedness = Undirected, SelfLoops self_loops = NoSelfLoops) MAY_THROW_EXCEPTION;
		// TODO: erdos_renyi_Gnm_game_simple
		
		Graph& rewire_edges(const Real prob) MAY_THROW_EXCEPTION;
		/// Randomly rewire edges and produce a simple graph.
		Graph& rewire_edges_simple(const Real prob) MAY_THROW_EXCEPTION;
		Graph& rewire_edges_simple(const ::gsl::Random& rangen, const Real prob) MAY_THROW_EXCEPTION;
		
		static ::tempobj::force_temporary_class<Graph>::type degree_sequence_game(const Vector& out_deg, const Vector& in_deg, DegSeqGenerator method) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type forest_fire_game(Integer n, Integer nodes, Real fw_prob, Real bw_factor, Integer pambs, Directedness directed) MAY_THROW_EXCEPTION;

		// What is this? No probability at all?
		Graph& rewire(Integer max_trials = 16) MAY_THROW_EXCEPTION;

		static ::tempobj::force_temporary_class<Graph>::type growing_random_game(Integer n, Integer m, Directedness directed, Boolean citation) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type callaway_traits_game(Integer nodes, Integer types, Integer edges_per_step, Vector& type_dist, Matrix& pref_matrix, Directedness directed) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type establishment_game(Integer nodes, Integer types, Integer k, Vector& type_dist, Matrix& pref_matrix, Directedness directed) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type preference_game(Integer nodes, Integer types, Vector& type_dist, Matrix& pref_matrix, Vector& node_type_vec, Directedness directed, SelfLoops loops) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type asymmetric_preference_game(Integer nodes, Integer types, Matrix& type_dist_matrix, Matrix& pref_matrix, Vector& node_type_in_vec, Vector& node_type_out_vec, SelfLoops loops) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type recent_degree_game(Integer n, Real power, Integer window, Integer m, const Vector& outseq, Boolean outpref, Real zero_appeal, Directedness directed) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type barabasi_aging_game(Integer nodes, Integer m, const Vector& outseq, Boolean outpref, Real pa_exp, Real aging_exp, Integer aging_bin, Real zero_deg_appeal, Real zero_age_appeal, Real deg_coef, Real age_coef, Directedness directed) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type recent_degree_aging_game(Integer nodes, Integer m, const Vector& outseq, Boolean outpref, igraph_real_t pa_exp, igraph_real_t aging_exp, Integer aging_bin, Integer time_window, igraph_real_t zero_appeal, Directedness directed) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type cited_type_game(Integer nodes, const Vector& types, const Vector& pref, Integer edges_per_step, Directedness directed) MAY_THROW_EXCEPTION;
		static ::tempobj::force_temporary_class<Graph>::type citing_cited_type_game(Integer nodes, const Vector& types, const Matrix& pref, Integer edges_per_step, Directedness directed) MAY_THROW_EXCEPTION;
				
#pragma mark -
#pragma mark 10.1 Basic Properties
		bool are_connected(const Vertex from, const Vertex to) MAY_THROW_EXCEPTION;

#pragma mark -
#pragma mark 10.2 Shortest Path Related Functions
		/// For unweighted shortest path, use igraph_shortest_paths(). For weighted, use Dijkstra, Bellman Ford and Johnson instead.
		::tempobj::force_temporary_class<Matrix>::type shortest_paths(const VertexSelector& from, NeighboringMode mode = AllNeighbors) const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<Matrix>::type shortest_paths_dijkstra(const VertexSelector& from, Vector& weights, NeighboringMode mode) const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<Matrix>::type shortest_paths_bellman_ford(const VertexSelector& from, Vector& weights, NeighboringMode mode) const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<Matrix>::type shortest_paths_johnson(const VertexSelector& from, Vector& weights, NeighboringMode mode) const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<ReferenceVector<Vector> >::type get_shortest_paths(Integer from, const VertexSelector& to, NeighboringMode mode) const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<ReferenceVector<Vector> >::type get_shortest_paths_dijkstra(Integer from, const VertexSelector& to, Vector& weights, NeighboringMode mode) const MAY_THROW_EXCEPTION;
		// TODO: implement igraph_get_all_shortest_paths()
		Real average_path_length(Directedness directedness, Boolean unconn) const MAY_THROW_EXCEPTION;
		std::pair<Vector,Real> path_length_hist(Directedness directedness) const MAY_THROW_EXCEPTION;
		Integer diameter(Directedness directedness, Boolean unconn) const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<Vector>::type get_diameter(Directedness directedness, Boolean unconn) const MAY_THROW_EXCEPTION;
		std::pair<Integer,Integer> farthest_nodes(Directedness directedness, Boolean unconn) const MAY_THROW_EXCEPTION;
		Integer girth() const MAY_THROW_EXCEPTION;
		Integer girth(Vector& circle) const MAY_THROW_EXCEPTION;

#pragma mark -
#pragma mark 10.3 Neighborhood of a vertex
		::tempobj::force_temporary_class<Vector>::type neighborhood_size(VertexSelector& vs, Integer order, NeighboringMode mode) const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<ReferenceVector<VertexVector> >::type neighborhood(VertexSelector& vs, Integer order, NeighboringMode mode) const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<ReferenceVector<Graph> >::type neighborhood_graphs(VertexSelector& vs, Integer order, NeighboringMode mode) const MAY_THROW_EXCEPTION;


#pragma mark -
#pragma mark 10.4 Graph Components
		enum Connectedness {
			WeaklyConnected = IGRAPH_WEAK,
			StronglyConnected = IGRAPH_STRONG
		};
		
		::tempobj::force_temporary_class<VertexVector>::type subcomponent(const Vertex representative, const NeighboringMode mode = OutNeighbors) const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<Graph>::type subgraph(const VertexSelector& vs) const MAY_THROW_EXCEPTION;
		void cluster(Vector& cluster_id_each_vertex_belongs_to, Vector& size_of_each_cluster, Connectedness connectedness = WeaklyConnected) const MAY_THROW_EXCEPTION;
		Integer cluster_count(const Connectedness connectedness = WeaklyConnected) const MAY_THROW_EXCEPTION;
		
		bool is_connected(const Connectedness connectedness = WeaklyConnected) const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<ReferenceVector<Graph> >::type decompose(Connectedness connectedness = WeaklyConnected, long max_component_number = -1, long min_size_of_components = -1) const MAY_THROW_EXCEPTION;
		Integer biconnected_components_count() const MAY_THROW_EXCEPTION;
		Integer biconnected_components(ReferenceVector<VertexVector>& components, VertexVector& articulation_points) const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<ReferenceVector<VertexVector> >::type biconnected_components() const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<VertexVector>::type articulation_points() const MAY_THROW_EXCEPTION;


#pragma mark -
#pragma mark 10.5 Centrality Measures

		::tempobj::force_temporary_class<Vector>::type closeness(const VertexSelector& vids, NeighboringMode neimode) const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<Vector>::type betweenness(const VertexSelector& vids, Directedness directedness) const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<Vector>::type edge_betweenness(Directedness directedness) const MAY_THROW_EXCEPTION;
		// TODO: igraph_pagerank
		// TODO: igraph_pagerank_old
		::tempobj::force_temporary_class<Vector>::type constraint(const VertexSelector& vids) const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<Vector>::type constraint(const VertexSelector& vids, const Vector& weights) const MAY_THROW_EXCEPTION;
		Integer maxdegree(const VertexSelector& vids, NeighboringMode neimode, SelfLoops countloops) const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<Vector>::type strength(const VertexSelector& vids, const Vector& weights, NeighboringMode neimode, SelfLoops countloops) const MAY_THROW_EXCEPTION;
		// TODO: igraph_eigenvector_centrality
		// TODO: igraph_hub_score
		// TODO: igraph_authority_score


#pragma mark -
#pragma mark 10.6 Estimating Centrality Measures

		::tempobj::force_temporary_class<Vector>::type closeness_estimate(const VertexSelector& vids, NeighboringMode neimode, Integer cutoff) const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<Vector>::type betweenness_estimate(const VertexSelector& vids, Directedness directedness, Integer cutoff) const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<Vector>::type edge_betweenness_estimate(Directedness directedness, Integer cutoff) const MAY_THROW_EXCEPTION;


#pragma mark -
#pragma mark 10.7 Similarity Measures

		::tempobj::force_temporary_class<Matrix>::type bibcoupling(const VertexSelector& vids) const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<Matrix>::type cocitation(const VertexSelector& vids) const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<Matrix>::type similarity_jaccard(const VertexSelector& vids, NeighboringMode neimode, SelfLoops countloops) const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<Matrix>::type similarity_dice(const VertexSelector& vids, NeighboringMode neimode, SelfLoops countloops) const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<Matrix>::type similarity_inverse_log_weighted(const VertexSelector& vids, NeighboringMode neimode) const MAY_THROW_EXCEPTION;


#pragma mark -
#pragma mark 10.8 Spanning Tree

		::tempobj::force_temporary_class<Graph>::type minimum_spanning_tree() const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<Graph>::type minimum_spanning_tree(const Vector weights) const MAY_THROW_EXCEPTION;


#pragma mark -
#pragma mark 10.9 Transitivity or Clustering Coefficient

		Real transitivity() const MAY_THROW_EXCEPTION;
		Real transitivity_of(Vertex i) const MAY_THROW_EXCEPTION;
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
#pragma mark 10.11 Spectral properties

		::tempobj::force_temporary_class<Matrix>::type laplacian(Boolean normalized = false) const MAY_THROW_EXCEPTION;


#pragma mark -
#pragma mark 10.12 Non-simple graphs: multiple and loop edges

		bool is_simple() const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<BoolVector>::type is_loop(const EdgeSelector& es) const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<BoolVector>::type is_multiple(const EdgeSelector& es) const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<Vector>::type count_multiple(const EdgeSelector& es) const MAY_THROW_EXCEPTION;
		
		Graph& simplify() MAY_THROW_EXCEPTION;
		Graph& simplify_loops() MAY_THROW_EXCEPTION;
		Graph& simplify_multiple_edges() MAY_THROW_EXCEPTION;
		

#pragma mark -
#pragma mark 10.13 K-Cores

		::tempobj::force_temporary_class<Vector>::type coreness(NeighboringMode neimode) const MAY_THROW_EXCEPTION;


#pragma mark -
#pragma mark 10.14 Topological sorting

		::tempobj::force_temporary_class<Vector>::type topological_sorting(NeighboringMode neimode) const MAY_THROW_EXCEPTION;


#pragma mark -
#pragma mark 10.15 Line graphs

		::tempobj::force_temporary_class<Graph>::type linegraph() const MAY_THROW_EXCEPTION;


#pragma mark -
#pragma mark 10.16 Unfolding a graph into a tree

		::tempobj::force_temporary_class<Graph>::type unfold_tree(const Vector& roots, NeighboringMode neimode) const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<Graph>::type unfold_tree(const Vector& roots, Vector& vertex_index, NeighboringMode neimode) const MAY_THROW_EXCEPTION;


#pragma mark -
#pragma mark 10.17 Other Operations
		enum GetAdjacency {
			GetAdjacency_Upper = IGRAPH_GET_ADJACENCY_UPPER,
			GetAdjacency_Lower = IGRAPH_GET_ADJACENCY_LOWER,
			GetAdjacency_Both = IGRAPH_GET_ADJACENCY_BOTH,
		};
		enum EdgelistSequenceOrdering {
			EdgelistSequenceOrdering_Default = 0,
			EdgelistSequenceOrdering_ByColumns = 1
		};

		Real density(SelfLoops countLoops) const MAY_THROW_EXCEPTION;
		Real reciprocity(Boolean ignore_loops) const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<BoolVector>::type is_mutual(const EdgeSelector& es) /*const?*/ MAY_THROW_EXCEPTION;
		// TODO: igraph_avg_nearest_neighbor_degree
		::tempobj::force_temporary_class<Matrix>::type get_adjacency(GetAdjacency type = GetAdjacency_Both) const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<Vector>::type get_edgelist(EdgelistSequenceOrdering bycol = EdgelistSequenceOrdering_Default) const MAY_THROW_EXCEPTION;


#pragma mark -
#pragma mark 11. Cliques and Independent Vertex Sets
		::tempobj::force_temporary_class<ReferenceVector<Vector> >::type cliques(const Integer max_size = 0) const;
		::tempobj::force_temporary_class<ReferenceVector<Vector> >::type cliques(const Integer min_size, const Integer max_size) const;
		::tempobj::force_temporary_class<ReferenceVector<Vector> >::type largest_cliques() const;
		::tempobj::force_temporary_class<ReferenceVector<Vector> >::type maximal_cliques() const;
		Integer clique_number() const MAY_THROW_EXCEPTION;

		::tempobj::force_temporary_class<ReferenceVector<Vector> >::type independent_vertex_sets(Integer min_size, Integer max_size) const;
		::tempobj::force_temporary_class<ReferenceVector<Vector> >::type independent_vertex_sets(const Integer max_size) const;
		::tempobj::force_temporary_class<ReferenceVector<Vector> >::type largest_independent_vertex_sets() const;
		::tempobj::force_temporary_class<ReferenceVector<Vector> >::type maximal_independent_vertex_sets() const;
		Integer independence_number() const MAY_THROW_EXCEPTION;

#pragma mark -
#pragma mark 12. Graph Isomorphism
		
		::tempobj::force_temporary_class<Graph>::type permute_vertices(const VertexVector& permutation) const MAY_THROW_EXCEPTION;

#pragma mark -
#pragma mark 14. Generating Layouts for Graph Drawing

	::tempobj::force_temporary_class<Matrix>::type layout_random() const MAY_THROW_EXCEPTION;
	::tempobj::force_temporary_class<Matrix>::type layout_circle() const MAY_THROW_EXCEPTION;
	// TODO: igraph_layout_graphopt
	// TODO: 10.1.4. The DrL layout generator
	// TODO: igraph_layout_fruchterman_reingold
	// TODO: igraph_layout_kamada_kawai
	::tempobj::force_temporary_class<Matrix>::type layout_reingold_tilford(Vertex root) const MAY_THROW_EXCEPTION;
	::tempobj::force_temporary_class<Matrix>::type layout_reingold_tilford_circular(Vertex root) const MAY_THROW_EXCEPTION;
	// TODO: igraph_layout_grid_fruchterman_reingold
	// TODO: igraph_layout_lgl
	::tempobj::force_temporary_class<Matrix>::type layout_random_3d() const MAY_THROW_EXCEPTION;
	::tempobj::force_temporary_class<Matrix>::type layout_sphere() const MAY_THROW_EXCEPTION;
	// TODO: igraph_layout_fruchterman_reingold_3d
	// TODO: igraph_layout_kamada_kawai_3d
	// TODO: igraph_layout_merge_dla

		
#pragma mark -
#pragma mark 15. Reading and Writing Graphs from and to Files
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
#pragma mark 17. Detecting Community Structure
		
		Community community() const throw() { return Community(*this); }
		
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

		::tempobj::force_temporary_class<Graph>::type operator^ (const Graph& other) const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<Graph>::type operator| (const Graph& other) const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<Graph>::type operator& (const Graph& other) const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<Graph>::type operator- (const Graph& other) const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<Graph>::type operator~ () const MAY_THROW_EXCEPTION;
		Graph& operator^= (const Graph& other) MAY_THROW_EXCEPTION;
		Graph& operator|= (const Graph& other) MAY_THROW_EXCEPTION;
		Graph& operator&= (const Graph& other) MAY_THROW_EXCEPTION;
		Graph& operator-= (const Graph& other) MAY_THROW_EXCEPTION;
		
#pragma mark -
#pragma mark Miscellaneous
		
		/** Combine two graphs together. The resulting graph has N1 + N2 vertices and M1 + M2 edges. */
		Graph& join(const Graph& other) MAY_THROW_EXCEPTION;


		friend class VertexSelector;
		friend class EdgeSelector;
		friend class VertexIterator;
		friend class EdgeIterator;
		friend class AdjacencyList;
		friend class Community;
	};
	
	MEMORY_MANAGER_INTERFACE_EX(Graph);
}

#endif
