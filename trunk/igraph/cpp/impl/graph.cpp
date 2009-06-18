/*

Graph.cpp ... Implementation for Graphs

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

#ifndef IGRAPH_GRAPH_CPP
#define IGRAPH_GRAPH_CPP

#include <igraph/cpp/graph.hpp>
#include <igraph/cpp/adjlist.hpp>
#include <gsl/cpp/rng_minimal.hpp>
#include <stdexcept>

namespace igraph {

#if !IGRAPH_NO_ATTRIBUTES
	__attribute__((constructor))
#endif
	void attach_attribute_table() {
		igraph_i_set_attribute_table(&igraph_cattribute_table);
	}


	MEMORY_MANAGER_IMPLEMENTATION(Graph);
	XXINTRNL_WRAPPER_CONSTRUCTOR_IMPLEMENTATION(Graph, igraph_t, igraph_copy);

	IMPLEMENT_COPY_METHOD(Graph) {
		igraph_copy(&_, &other._);
	}
	IMPLEMENT_MOVE_METHOD(Graph) {
		_ = ::std::move(other._);
	}
	IMPLEMENT_DEALLOC_METHOD(Graph) {
		igraph_destroy(&_);
	}


	Graph::Graph(Integer size, Directedness directedness) MAY_THROW_EXCEPTION {
		XXINTRNL_DEBUG_CALL_INITIALIZER(Graph);
		TRY(igraph_empty(&_, size, directedness));
	}

	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::empty(Integer size, Directedness directedness) MAY_THROW_EXCEPTION {
		return FORCE_STD_MOVE(Graph)(Graph(size, directedness));
	}

#pragma mark -
#pragma mark Basic Query Operations

	void Graph::edge(const Edge edge_id, Vertex& from, Vertex& to) const MAY_THROW_EXCEPTION {
		TRY(igraph_edge(&_, edge_id, &from, &to));
	}
	Edge Graph::get_eid(const Vertex from, const Vertex to, const Directedness arc) const MAY_THROW_EXCEPTION {
		Edge e;
		TRY(igraph_get_eid(&_, &e, from, to, arc));
		return e;
	}

	RETRIEVE_TEMPORARY_CLASS(VertexVector) Graph::neighbors(Vertex vid, NeighboringMode neimode) const MAY_THROW_EXCEPTION {
		igraph_vector_t res;
		TRY(igraph_vector_init(&res, 0));
		TRY(igraph_neighbors(&_, &res, vid, (igraph_neimode_t)neimode));
		return FORCE_STD_MOVE(VertexVector)(VertexVector(&res, ::tempobj::OwnershipTransferMove));
	}

	RETRIEVE_TEMPORARY_CLASS(EdgeVector) Graph::adjacent(Vertex vid, NeighboringMode neimode) const MAY_THROW_EXCEPTION {
		igraph_vector_t res;
		TRY(igraph_vector_init(&res, 0));
		TRY(igraph_adjacent(&_, &res, vid, (igraph_neimode_t)neimode));
		return FORCE_STD_MOVE(EdgeVector)(EdgeVector(&res, ::tempobj::OwnershipTransferMove));
	}

	Integer Graph::degree(Vertex i, NeighboringMode neimode, SelfLoops countLoops) const MAY_THROW_EXCEPTION {
		igraph_vector_t res;
		TRY(igraph_vector_init(&res, 1));
		TRY(igraph_degree(&_, &res, igraph_vss_1(i), (igraph_neimode_t)neimode, countLoops));
		Integer resdeg = VECTOR(res)[0];
		return resdeg;
	}
	RETRIEVE_TEMPORARY_CLASS(Vector) Graph::degree(const VertexSelector& vids, NeighboringMode neimode, SelfLoops countLoops) const MAY_THROW_EXCEPTION {
		igraph_vector_t res;
		TRY(igraph_vector_init(&res, 0));
		TRY(igraph_degree(&_, &res, vids._, (igraph_neimode_t)neimode, countLoops));
		return FORCE_STD_MOVE(Vector)(Vector(&res, ::tempobj::OwnershipTransferMove));
	}
	RETRIEVE_TEMPORARY_CLASS(Vector) Graph::degree(NeighboringMode neimode, SelfLoops countLoops) const MAY_THROW_EXCEPTION {
		return degree(VertexSelector::all(), neimode, countLoops);
	}

#pragma mark -
#pragma mark Adding and Deleting Vertices and Edges

	Graph& Graph::add_edge(const Vertex from, const Vertex to) MAY_THROW_EXCEPTION {
		TRY(igraph_add_edge(&_, from, to));
		return *this;
	}

	Graph& Graph::add_edges(const VertexVector& edges, void* const attr) MAY_THROW_EXCEPTION {
		TRY(igraph_add_edges(&_, &edges._, attr));
		return *this;
	}

	Graph& Graph::add_vertices(const Integer nv, void* const attr) MAY_THROW_EXCEPTION {
		TRY(igraph_add_vertices(&_, nv, attr));
		return *this;
	}

	Graph& Graph::delete_edges(const EdgeSelector& es) MAY_THROW_EXCEPTION {
		TRY(igraph_delete_edges(&_, es._));
		return *this;
	}

	Graph& Graph::delete_edge(const Vertex from, const Vertex to) MAY_THROW_EXCEPTION { return delete_edges(EdgeSelector::fromto(from, to)); }
	Graph& Graph::delete_edge(const Edge eid) MAY_THROW_EXCEPTION { return delete_edges(EdgeSelector::single(eid)); }

	Graph& Graph::delete_vertices(const VertexSelector& vs) MAY_THROW_EXCEPTION {
		TRY(igraph_delete_vertices(&_, vs._));
		return *this;
	}

#pragma mark -
#pragma mark Deterministic Graph Generators

	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::create(const VertexVector& edges, const Integer min_size, const Directedness directedness) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_create(&_, &edges._, min_size, directedness));
		return FORCE_STD_MOVE(Graph)(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	// TODO: igraph_adjacency when Matrix is implemented.
	// TODO: igraph_weighted_adjacency when Matrix is implemented.
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::adjlist(const AdjacencyList& lst, const Directedness directedness, const ToUndirectedMode duplicate_edges) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_adjlist(&_, &lst._, directedness, duplicate_edges == ToUndirectedMode_Each));
		return FORCE_STD_MOVE(Graph)(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::star(const Integer n, const StarMode mode, const Vertex center) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_star(&_, n, (igraph_star_mode_t)mode, center));
		return FORCE_STD_MOVE(Graph)(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::lattice(const Vector& dimensions, const PeriodicLattice periodic, const Integer step, const Directedness directedness, const MutualConnections mutual) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_lattice(&_, &dimensions._, step, directedness, mutual, periodic));
		return FORCE_STD_MOVE(Graph)(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::lattice_2d(const Integer width, const Integer length, const PeriodicLattice periodic, const Integer step, const Directedness directedness, const MutualConnections mutual) MAY_THROW_EXCEPTION {
		Integer raw_dimensions[2] = {width, length};
		return lattice(Vector::view(raw_dimensions, 2), periodic, step, directedness, mutual);
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::lattice_3d(const Integer width, const Integer length, const Integer height, const PeriodicLattice periodic, const Integer step, const Directedness directedness, const MutualConnections mutual) MAY_THROW_EXCEPTION {
		Integer raw_dimensions[3] = {width, length, height};
		return lattice(Vector::view(raw_dimensions, 3), periodic, step, directedness, mutual);
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::ring(const Integer size, const Directedness directedness, const MutualConnections mutual, const PeriodicLattice periodic) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_ring(&_, size, directedness, mutual, periodic));
		return FORCE_STD_MOVE(Graph)(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::tree(const Integer n, const Integer children, const TreeMode type) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_tree(&_, n, children, (igraph_tree_mode_t)type));
		return FORCE_STD_MOVE(Graph)(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::full(const Integer n, const Directedness directedness, const SelfLoops loops) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_full(&_, n, directedness, loops));
		return FORCE_STD_MOVE(Graph)(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::complete(const Integer n, const Directedness directedness, const SelfLoops loops) MAY_THROW_EXCEPTION {
		return full(n, directedness, loops);
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::full_citation(const Integer n, const Directedness directedness) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_full_citation(&_, n, directedness));
		return FORCE_STD_MOVE(Graph)(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::famous(const char* name) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_famous(&_, name));
		return FORCE_STD_MOVE(Graph)(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::lcf_vector(const Integer n, const Vector& shifts, const Integer repeats) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_lcf_vector(&_, n, &shifts._, repeats));
		return FORCE_STD_MOVE(Graph)(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::atlas(const int number) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_atlas(&_, number));
		return FORCE_STD_MOVE(Graph)(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::de_bruijn(const Integer m, const Integer n) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_de_bruijn(&_, m, n));
		return FORCE_STD_MOVE(Graph)(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::kautz(const Integer m, const Integer n) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_kautz(&_, m, n));
		return FORCE_STD_MOVE(Graph)(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	// TODO: igraph_extended_chordal_ring when Matrix is implemented.

	Graph& Graph::connect_neighborhood(const Integer order, const NeighboringMode neimode) MAY_THROW_EXCEPTION {
		TRY(igraph_connect_neighborhood(&_, order, (igraph_neimode_t)neimode));
		return *this;
	}

#pragma mark -
#pragma mark Games: Randomized Graph Generators

	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::grg_game(const Integer size, const Real radius, const PeriodicLattice periodic) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_grg_game(&_, size, radius, periodic, NULL, NULL));
		return FORCE_STD_MOVE(Graph)(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::grg_game(const Integer size, const Real radius, const PeriodicLattice periodic, Vector& x_coords, Vector& y_coords) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_grg_game(&_, size, radius, periodic, &x_coords._, &y_coords._));
		return FORCE_STD_MOVE(Graph)(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::barabasi_game(const Integer size, const Integer m, const Directedness directed, const BarabasiOutPref outpref) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_barabasi_game(&_, size, m, NULL, outpref, directed));
		return FORCE_STD_MOVE(Graph)(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::barabasi_game(const Integer size, const Vector& outseq, const Directedness directed, const BarabasiOutPref outpref) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_barabasi_game(&_, size, 0, &outseq._, outpref, directed));
		return FORCE_STD_MOVE(Graph)(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	
	// we assume how_many is small.
	static inline void XXINTRNL_random_choose(const ::gsl::Random& rangen, unsigned long* store, unsigned long how_many, double* discrete_distrib, unsigned long count, double total) {
		// because the distribution changes everytime we pick a number, it is unwise for us to cook the PDF into CDF.
		for (unsigned long i = 0; i < how_many; ++ i) {
			double current_level = rangen.uniform() * total;
			for (long x = count-1; x >= 0; -- x) {
				// Ignore chosen numbers.
				if (discrete_distrib[x] <= 0)
					continue;
				current_level -= discrete_distrib[x];
				if (current_level <= 0) {
					store[i] = x;
					total -= discrete_distrib[x];
					discrete_distrib[x] = -discrete_distrib[x];
					break;
				}
			}
		}
		for (unsigned long i = 0; i < how_many; ++ i) {
			discrete_distrib[store[i]] = -discrete_distrib[store[i]];
		}
	}
	
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::barabasi_game_simple(const Integer size, const Integer m) MAY_THROW_EXCEPTION { return barabasi_game_simple(::gsl::Random::default_generator(), size, m); }
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::barabasi_game_simple(const ::gsl::Random& rangen, const Integer size, const Integer m) MAY_THROW_EXCEPTION {
		if (size <= m+1)
			return Graph::full(size);
		else {
			Graph g = Graph::full(m+1);
			g.add_vertices(size - (m+1));
			
			unsigned long M = static_cast<unsigned long>(m), Size = static_cast<unsigned long>(size);
			unsigned long* vertices_to_connect = (M <= 32) ? reinterpret_cast<unsigned long*>(alloca(M * sizeof(unsigned long))) : new unsigned long[M];
			double* current_degrees = (Size <= 496) ? reinterpret_cast<double*>(alloca(Size * sizeof(double))) : new double[Size];
			for (unsigned long i = 0; i < Size; ++ i)
				current_degrees[i] = m;
			double total_degree = m*(m+1);
			
			Vector edges_to_add( (Size - (M+1))*M*2 );
			unsigned long k = 0;
				 
			for (unsigned long i = M+1; i < Size; ++ i) {
				XXINTRNL_random_choose(rangen, vertices_to_connect, M, current_degrees, i, total_degree);
				for (unsigned long j = 0; j < M; ++ j) {
					edges_to_add[k++] = i;
					edges_to_add[k++] = vertices_to_connect[j];
					current_degrees[vertices_to_connect[j]] += 1;
				}
				total_degree += m;
				/*
				for (unsigned long j = 0; j < i; ++ j)
					printf("%lg ", current_degrees[j]);
				printf("[%lg]\n", total_degree);
				 */
			}
			
			if (M > 32) delete[] vertices_to_connect;
			if (Size > 496) delete[] current_degrees;
			
			g.add_edges(edges_to_add);
			
			return FORCE_STD_MOVE(Graph)(g);
		}
	}

	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::watts_strogatz_game(const Integer size, const Integer K, const Real p, const Integer dimensions) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_watts_strogatz_game(&_, dimensions, size, K, p));
		return FORCE_STD_MOVE(Graph)(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::watts_strogatz_game_simple(const Integer size, const Integer K, const Real p, const Integer dimensions) MAY_THROW_EXCEPTION {
		return watts_strogatz_game_simple(::gsl::Random::default_generator(), size, K, p, dimensions);
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::watts_strogatz_game_simple(const ::gsl::Random& rangen, const Integer size, const Integer K, const Real p, const Integer dimensions) MAY_THROW_EXCEPTION {
			Graph::Temporary base = dimensions == 1 ? Graph::ring(size) : Graph::lattice(Vector((long)dimensions).fill(size));
		return FORCE_STD_MOVE(Graph)(base.connect_neighborhood(K).rewire_edges_simple(rangen, p));
	}
	
	Graph& Graph::rewire_edges(const Real prob) MAY_THROW_EXCEPTION {
		TRY(igraph_rewire_edges(&_, prob));
		return *this;
	}
	Graph& Graph::rewire_edges_simple(const Real prob) MAY_THROW_EXCEPTION { return rewire_edges_simple(::gsl::Random::default_generator(), prob); }
	Graph& Graph::rewire_edges_simple(const ::gsl::Random& rangen, const Real prob) MAY_THROW_EXCEPTION {
		// Will this create some sort of bias?
		for (long eid = ecount()-1; eid >= 0; -- eid) {
			if (rangen.uniform() < prob) {
				Vertex head, tail;
				edge(eid, head, tail);
				VertexVector nonadj_vertices = VertexSelector::nonadj(head, OutNeighbors).as_vector(*this).remove_first_matching_assume_sorted(head);
				delete_edge(eid);
				add_edge(head, nonadj_vertices[rangen.uniform_int(nonadj_vertices.size())]);
			}
		}
		return *this;
	}

#pragma mark -
#pragma mark Basic Properties

	bool Graph::are_connected(const Vertex from, const Vertex to) MAY_THROW_EXCEPTION {
		igraph_bool_t res;
		TRY(igraph_are_connected(&_, from, to, &res));
		return res;
	}
	
#pragma mark -
#pragma mark Graph Components
	
	RETRIEVE_TEMPORARY_CLASS(VertexVector) Graph::subcomponent(const Vertex representative, const NeighboringMode mode) const MAY_THROW_EXCEPTION {
		igraph_vector_t res;
		TRY(igraph_vector_init(&res, 0));
		TRY(igraph_subcomponent(&_, &res, representative, (igraph_neimode_t)mode));
		return FORCE_STD_MOVE(VertexVector)(VertexVector(&res, ::tempobj::OwnershipTransferMove));
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::subgraph(const VertexSelector& vs) const MAY_THROW_EXCEPTION {
		igraph_t res;
		TRY(igraph_subgraph(&_, &res, vs._));
		return FORCE_STD_MOVE(Graph)(Graph(&res, ::tempobj::OwnershipTransferMove));
	}
	void Graph::cluster(Vector& cluster_id_each_vertex_belongs_to, Vector& size_of_each_cluster, Connectedness connectedness) const MAY_THROW_EXCEPTION {
		igraph_vector_t membership, csize;
		TRY(igraph_vector_init(&membership, 0));
		TRY(igraph_vector_init(&csize, 0));
		TRY(igraph_clusters(&_, &membership, &csize, NULL, (igraph_connectedness_t)connectedness));
		cluster_id_each_vertex_belongs_to = FORCE_STD_MOVE(Vector)(Vector(&membership, ::tempobj::OwnershipTransferMove));
		size_of_each_cluster = FORCE_STD_MOVE(Vector)(Vector(&csize, ::tempobj::OwnershipTransferMove));
	}
	Integer Graph::cluster_count(const Connectedness connectedness) const MAY_THROW_EXCEPTION {
		Integer res;
		TRY(igraph_clusters(&_, NULL, NULL, &res, (igraph_connectedness_t)connectedness));
		return res;
	}
	bool Graph::is_connected(const Connectedness connectedness) const MAY_THROW_EXCEPTION {
		Boolean res;
		TRY(igraph_is_connected(&_, &res, (igraph_connectedness_t)connectedness));
		return res;
	}
	RETRIEVE_TEMPORARY_CLASS(ReferenceVector<Graph>) Graph::decompose(Connectedness connectedness, long max_component_number, long min_size_of_components) {
		igraph_vector_ptr_t res;
		TRY(igraph_vector_ptr_init(&res, 0));
		TRY(igraph_decompose(&_, &res, (igraph_connectedness_t)connectedness, max_component_number, min_size_of_components));
		return ReferenceVector<Graph>::adopt<igraph_t>(res);
	}
	RETRIEVE_TEMPORARY_CLASS(VertexVector) Graph::articulation_points() const MAY_THROW_EXCEPTION {
		igraph_vector_t res;
		TRY(igraph_vector_init(&res, 0));
		TRY(igraph_articulation_points(&_, &res));
		return FORCE_STD_MOVE(VertexVector)(VertexVector(&res, ::tempobj::OwnershipTransferMove));
	}

#pragma mark -
#pragma mark 10.9 Transitivity or Clustering Coefficient

	Real Graph::transitivity() const MAY_THROW_EXCEPTION {
		Real res;
		TRY(igraph_transitivity_undirected(&_, &res));
		return res;
	}
	Real Graph::transitivity_local(Vertex i) const MAY_THROW_EXCEPTION {
		Vector res (1);
		TRY(igraph_transitivity_local_undirected(&_, &res._, igraph_vss_1(i)));
		return res[0];
	}
	RETRIEVE_TEMPORARY_CLASS(Vector) Graph::transitivity_local(const VertexSelector& vids) const MAY_THROW_EXCEPTION {
		Vector res ((long)vids.size(*this));
		TRY(igraph_transitivity_local_undirected(&_, &res._, vids._));
		return res;
	}
	RETRIEVE_TEMPORARY_CLASS(Vector) Graph::transitivity_local() const MAY_THROW_EXCEPTION {
		Vector res (this->size());
		TRY(igraph_transitivity_local_undirected(&_, &res._, igraph_vss_all()));
		return ::std::move(res);
	}
	Real Graph::transitivity_avglocal() const MAY_THROW_EXCEPTION {
		Real res;
		TRY(igraph_transitivity_avglocal_undirected(&_, &res));
		return res;
	}
	
#pragma mark -
#pragma mark Directedness conversion	
	Graph& Graph::to_undirected(const ToDirectedMode mode) MAY_THROW_EXCEPTION {
		TRY(igraph_to_undirected(&_, (igraph_to_undirected_t)mode));
		return *this;
	}
	Graph& Graph::to_directed(const ToUndirectedMode mode) MAY_THROW_EXCEPTION {
		TRY(igraph_to_directed(&_, (igraph_to_directed_t)mode));
		return *this;
	}
	
#pragma mark -
#pragma mark Non-simple graphs: multiple and loop edges
	bool Graph::is_simple() const MAY_THROW_EXCEPTION {
		igraph_bool_t res;
		TRY(igraph_is_simple(&_, &res));
		return res;
	}
	RETRIEVE_TEMPORARY_CLASS(BoolVector) Graph::is_loop(const EdgeSelector& es) const MAY_THROW_EXCEPTION {
		igraph_vector_bool_t res;
		TRY(igraph_vector_bool_init(&res, 0));
		TRY(igraph_is_loop(&_, &res, es._));
		return FORCE_STD_MOVE(BoolVector)(BoolVector(&res, ::tempobj::OwnershipTransferMove));
	}
	RETRIEVE_TEMPORARY_CLASS(BoolVector) Graph::is_multiple(const EdgeSelector& es) const MAY_THROW_EXCEPTION {
		igraph_vector_bool_t res;
		TRY(igraph_vector_bool_init(&res, 0));
		TRY(igraph_is_multiple(&_, &res, es._));
		return FORCE_STD_MOVE(BoolVector)(BoolVector(&res, ::tempobj::OwnershipTransferMove));
	}	
	RETRIEVE_TEMPORARY_CLASS(Vector) Graph::count_multiple(const EdgeSelector& es) const MAY_THROW_EXCEPTION {
		igraph_vector_t res;
		TRY(igraph_vector_init(&res, 0));
		TRY(igraph_count_multiple(&_, &res, es._));
		return FORCE_STD_MOVE(Vector)(Vector(&res, ::tempobj::OwnershipTransferMove));
	}
	
	Graph& Graph::simplify() MAY_THROW_EXCEPTION {
		TRY(igraph_simplify(&_, true, true));
		return *this;
	}
	Graph& Graph::simplify_loops() MAY_THROW_EXCEPTION {
		TRY(igraph_simplify(&_, false, true));
		return *this;
	}
	Graph& Graph::simplify_multiple_edges() MAY_THROW_EXCEPTION {
		TRY(igraph_simplify(&_, true, false));
		return *this;
	}
	

#pragma mark -
#pragma mark Cliques
	
	RETRIEVE_TEMPORARY_CLASS(ReferenceVector<Vector>) Graph::cliques(const Integer min_size, const Integer max_size) const {
		igraph_vector_ptr_t res;
		TRY(igraph_vector_ptr_init(&res, 0));
		TRY(igraph_cliques(&_, &res, min_size, max_size));
		return ReferenceVector<Vector>::adopt<igraph_vector_t>(res);
	}
	
	RETRIEVE_TEMPORARY_CLASS(ReferenceVector<Vector>) Graph::cliques(const Integer max_size) const {
		return cliques(0, max_size);
	}
	
	RETRIEVE_TEMPORARY_CLASS(ReferenceVector<Vector>) Graph::largest_cliques() const {
		igraph_vector_ptr_t res;
		TRY(igraph_vector_ptr_init(&res, 0));
		TRY(igraph_largest_cliques(&_, &res));
		return ReferenceVector<Vector>::adopt<igraph_vector_t>(res);
	}
	
	RETRIEVE_TEMPORARY_CLASS(ReferenceVector<Vector>) Graph::maximal_cliques() const {
		igraph_vector_ptr_t res;
		TRY(igraph_vector_ptr_init(&res, 0));
		TRY(igraph_maximal_cliques(&_, &res));
		return ReferenceVector<Vector>::adopt<igraph_vector_t>(res);
	}
	
	Integer Graph::clique_number() const MAY_THROW_EXCEPTION {
		Integer res;
		TRY(igraph_clique_number(&_, &res));
		return res;
	}
	
#pragma mark -
#pragma mark Reading and Writing Graphs from and to Files (GraphWriter)

	RETRIEVE_TEMPORARY_CLASS(GraphWriter) Graph::writer(const char* filename) const {
		return FORCE_STD_MOVE(GraphWriter)(GraphWriter(&_, filename));
	}
	RETRIEVE_TEMPORARY_CLASS(GraphWriter) Graph::writer(std::FILE* filestream) const throw() {
		return FORCE_STD_MOVE(GraphWriter)(GraphWriter(&_, filestream));
	}

	bool Graph::write(const char* filename, GraphFormat format) const {
		if (format == GraphFormat_auto)
			format = identify_file_format(filename, false);
		if (format != GraphFormat_auto) {
			GraphWriter writer = GraphWriter(&_, filename);
			switch (format) {
				case GraphFormat_adjlist:
					writer.adjlist();
					break;
				case GraphFormat_dot:
					writer.dot();
					break;
				case GraphFormat_edgelist:
					writer.edgelist();
					break;
				case GraphFormat_gml:
					writer.gml();
					break;
				case GraphFormat_graphml:
					writer.graphml();
					break;
				case GraphFormat_lgl:
					writer.lgl();
					break;
				case GraphFormat_ncol:
					writer.ncol();
					break;
				case GraphFormat_pajek:
					writer.pajek();
					break;
				default:
					return false;
			}
			return true;
		}
		return false;
	}

	RETRIEVE_TEMPORARY_CLASS(GraphReader) Graph::reader(const char* filename) { return FORCE_STD_MOVE(GraphReader)(GraphReader(filename)); }
	RETRIEVE_TEMPORARY_CLASS(GraphReader) Graph::reader(std::FILE* filestream) throw() { return FORCE_STD_MOVE(GraphReader)(GraphReader(filestream)); }

	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::read(const char* filename, GraphFormat format) {
		if (format == GraphFormat_auto)
			format = identify_file_format(filename, false);
		if (format != GraphFormat_auto) {
			GraphReader reader = GraphReader(filename);
			switch (format) {
				case GraphFormat_adjlist:
					return FORCE_STD_MOVE(Graph)(reader.adjlist());
//				case GraphFormat_dimacs:
				case GraphFormat_dot:
					break;
				case GraphFormat_edgelist:
					return FORCE_STD_MOVE(Graph)(reader.edgelist());
				case GraphFormat_gml:
					return FORCE_STD_MOVE(Graph)(reader.gml());
				case GraphFormat_graphdb:
					return FORCE_STD_MOVE(Graph)(reader.graphdb());
				case GraphFormat_lgl:
					return FORCE_STD_MOVE(Graph)(reader.lgl());
//				case GraphFormat_ncol:
				case GraphFormat_pajek:
					return FORCE_STD_MOVE(Graph)(reader.pajek());
				default:
					break;
			}
		}
		throw ::std::runtime_error("File type cannot be determined.");
	}

#pragma mark -
#pragma mark 18. Graph Operators

#define XXINTRNL_FORWARD_GRAPH_CREATION(temp, statement) \
	igraph_t temp; \
	TRY(statement); \
	return FORCE_STD_MOVE(Graph)(Graph(&temp, ::tempobj::OwnershipTransferMove)); \

	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::disjoint_union(const Graph& x, const Graph& y) MAY_THROW_EXCEPTION {
		XXINTRNL_FORWARD_GRAPH_CREATION(tmp, igraph_disjoint_union(&tmp, &x._, &y._) );
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::disjoint_union(const ReferenceVector<Graph>& gg) MAY_THROW_EXCEPTION {
		XXINTRNL_FORWARD_GRAPH_CREATION(tmp, igraph_disjoint_union_many(&tmp, &gg._) );
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::merge(const Graph& x, const Graph& y) MAY_THROW_EXCEPTION {
		XXINTRNL_FORWARD_GRAPH_CREATION(tmp, igraph_union(&tmp, &x._, &y._) );
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::merge(const ReferenceVector<Graph>& gg) MAY_THROW_EXCEPTION {
		XXINTRNL_FORWARD_GRAPH_CREATION(tmp, igraph_union_many(&tmp, &gg._) );
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::intersection(const Graph& x, const Graph& y) MAY_THROW_EXCEPTION {
		XXINTRNL_FORWARD_GRAPH_CREATION(tmp, igraph_intersection(&tmp, &x._, &y._) );
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::intersection(const ReferenceVector<Graph>& gg) MAY_THROW_EXCEPTION {
		XXINTRNL_FORWARD_GRAPH_CREATION(tmp, igraph_intersection_many(&tmp, &gg._) );
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::difference(const Graph& x, const Graph& y) MAY_THROW_EXCEPTION {
		XXINTRNL_FORWARD_GRAPH_CREATION(tmp, igraph_difference(&tmp, &x._, &y._) );
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::complementer(const Graph& x, SelfLoops loops) MAY_THROW_EXCEPTION {
		XXINTRNL_FORWARD_GRAPH_CREATION(tmp, igraph_complementer(&tmp, &x._, (igraph_bool_t)loops) );
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::compose(const Graph& x, const Graph& y) MAY_THROW_EXCEPTION {
		XXINTRNL_FORWARD_GRAPH_CREATION(tmp, igraph_compose(&tmp, &x._, &y._) );
	}

	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::operator^ (const Graph& other) MAY_THROW_EXCEPTION {
		XXINTRNL_FORWARD_GRAPH_CREATION(tmp, igraph_disjoint_union(&tmp, &_, &other._) );
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::operator| (const Graph& other) MAY_THROW_EXCEPTION  {
		XXINTRNL_FORWARD_GRAPH_CREATION(tmp, igraph_union(&tmp, &_, &other._) );
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::operator& (const Graph& other) MAY_THROW_EXCEPTION {
		XXINTRNL_FORWARD_GRAPH_CREATION(tmp, igraph_intersection(&tmp, &_, &other._) );
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::operator- (const Graph& other) MAY_THROW_EXCEPTION {
		XXINTRNL_FORWARD_GRAPH_CREATION(tmp, igraph_difference(&tmp, &_, &other._) );
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::operator~ () MAY_THROW_EXCEPTION {
		XXINTRNL_FORWARD_GRAPH_CREATION(tmp, igraph_complementer(&tmp, &_, (igraph_bool_t)NoSelfLoops) );
	}
	Graph& Graph::operator^= (const Graph& other) MAY_THROW_EXCEPTION {
		return (*this = *this ^ other);
	}
	Graph& Graph::operator|= (const Graph& other) MAY_THROW_EXCEPTION {
		return (*this = *this | other);
	}
	Graph& Graph::operator&= (const Graph& other) MAY_THROW_EXCEPTION {
		return (*this = *this & other);
	}
	Graph& Graph::operator-= (const Graph& other) MAY_THROW_EXCEPTION {
		return (*this = *this - other);
	}
	
#undef XXINTRNL_FORWARD_GRAPH_CREATION

}// end of igraph namespace

#endif
