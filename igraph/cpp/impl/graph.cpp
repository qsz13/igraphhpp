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
#include <cmath>

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

	::tempobj::force_temporary_class<Graph>::type Graph::empty(Integer size, Directedness directedness) MAY_THROW_EXCEPTION {
		return ::tempobj::force_move(Graph(size, directedness));
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

	::tempobj::force_temporary_class<VertexVector>::type Graph::neighbors(Vertex vid, NeighboringMode neimode) const MAY_THROW_EXCEPTION {
		igraph_vector_t res;
		TRY(igraph_vector_init(&res, 0));
		TRY(igraph_neighbors(&_, &res, vid, (igraph_neimode_t)neimode));
		return ::tempobj::force_move(VertexVector(&res, ::tempobj::OwnershipTransferMove));
	}

	::tempobj::force_temporary_class<EdgeVector>::type Graph::adjacent(Vertex vid, NeighboringMode neimode) const MAY_THROW_EXCEPTION {
		igraph_vector_t res;
		TRY(igraph_vector_init(&res, 0));
		TRY(igraph_adjacent(&_, &res, vid, (igraph_neimode_t)neimode));
		return ::tempobj::force_move(EdgeVector(&res, ::tempobj::OwnershipTransferMove));
	}

	Integer Graph::degree_of(Vertex i, NeighboringMode neimode, SelfLoops countLoops) const MAY_THROW_EXCEPTION {
		igraph_vector_t res;
		TRY(igraph_vector_init(&res, 1));
		TRY(igraph_degree(&_, &res, igraph_vss_1(i), (igraph_neimode_t)neimode, countLoops));
		Integer resdeg = VECTOR(res)[0];
		return resdeg;
	}
	::tempobj::force_temporary_class<Vector>::type Graph::degree(const VertexSelector& vids, NeighboringMode neimode, SelfLoops countLoops) const MAY_THROW_EXCEPTION {
		igraph_vector_t res;
		TRY(igraph_vector_init(&res, 0));
		TRY(igraph_degree(&_, &res, vids._, (igraph_neimode_t)neimode, countLoops));
		return ::tempobj::force_move(Vector(&res, ::tempobj::OwnershipTransferMove));
	}
	::tempobj::force_temporary_class<Vector>::type Graph::degree(NeighboringMode neimode, SelfLoops countLoops) const MAY_THROW_EXCEPTION {
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

	::tempobj::force_temporary_class<Graph>::type Graph::create(const VertexVector& edges, const Integer min_size, const Directedness directedness) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_create(&_, &edges._, min_size, directedness));
		return ::tempobj::force_move(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	// TODO: igraph_adjacency when Matrix is implemented.
	// TODO: igraph_weighted_adjacency when Matrix is implemented.
	::tempobj::force_temporary_class<Graph>::type Graph::adjlist(const AdjacencyList& lst, const Directedness directedness, const ToUndirectedMode duplicate_edges) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_adjlist(&_, &lst._, directedness, duplicate_edges == ToUndirectedMode_Each));
		return ::tempobj::force_move(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	
	::tempobj::force_temporary_class<Graph>::type Graph::star(const Integer n, const StarMode mode, const Vertex center) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_star(&_, n, (igraph_star_mode_t)mode, center));
		return ::tempobj::force_move(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	::tempobj::force_temporary_class<Graph>::type Graph::lattice(const Vector& dimensions, const PeriodicLattice periodic, const Integer step, const Directedness directedness, const MutualConnections mutual) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_lattice(&_, &dimensions._, step, directedness, mutual, periodic));
		return ::tempobj::force_move(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	::tempobj::force_temporary_class<Graph>::type Graph::lattice_2d(const Integer width, const Integer length, const PeriodicLattice periodic, const Integer step, const Directedness directedness, const MutualConnections mutual) MAY_THROW_EXCEPTION {
		Integer raw_dimensions[2] = {width, length};
		return lattice(Vector::view(raw_dimensions, 2), periodic, step, directedness, mutual);
	}
	::tempobj::force_temporary_class<Graph>::type Graph::lattice_3d(const Integer width, const Integer length, const Integer height, const PeriodicLattice periodic, const Integer step, const Directedness directedness, const MutualConnections mutual) MAY_THROW_EXCEPTION {
		Integer raw_dimensions[3] = {width, length, height};
		return lattice(Vector::view(raw_dimensions, 3), periodic, step, directedness, mutual);
	}
	::tempobj::force_temporary_class<Graph>::type Graph::ring(const Integer size, const Directedness directedness, const MutualConnections mutual, const PeriodicLattice periodic) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_ring(&_, size, directedness, mutual, periodic));
		return ::tempobj::force_move(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	::tempobj::force_temporary_class<Graph>::type Graph::tree(const Integer n, const Integer children, const TreeMode type) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_tree(&_, n, children, (igraph_tree_mode_t)type));
		return ::tempobj::force_move(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	::tempobj::force_temporary_class<Graph>::type Graph::full(const Integer n, const Directedness directedness, const SelfLoops loops) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_full(&_, n, directedness, loops));
		return ::tempobj::force_move(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	::tempobj::force_temporary_class<Graph>::type Graph::complete(const Integer n, const Directedness directedness, const SelfLoops loops) MAY_THROW_EXCEPTION {
		return full(n, directedness, loops);
	}
	::tempobj::force_temporary_class<Graph>::type Graph::full_citation(const Integer n, const Directedness directedness) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_full_citation(&_, n, directedness));
		return ::tempobj::force_move(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	::tempobj::force_temporary_class<Graph>::type Graph::famous(const char* name) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_famous(&_, name));
		return ::tempobj::force_move(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	::tempobj::force_temporary_class<Graph>::type Graph::lcf_vector(const Integer n, const Vector& shifts, const Integer repeats) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_lcf_vector(&_, n, &shifts._, repeats));
		return ::tempobj::force_move(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	::tempobj::force_temporary_class<Graph>::type Graph::atlas(const int number) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_atlas(&_, number));
		return ::tempobj::force_move(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	::tempobj::force_temporary_class<Graph>::type Graph::de_bruijn(const Integer m, const Integer n) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_de_bruijn(&_, m, n));
		return ::tempobj::force_move(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	::tempobj::force_temporary_class<Graph>::type Graph::kautz(const Integer m, const Integer n) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_kautz(&_, m, n));
		return ::tempobj::force_move(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	// TODO: igraph_extended_chordal_ring when Matrix is implemented.

	Graph& Graph::connect_neighborhood(const Integer order, const NeighboringMode neimode) MAY_THROW_EXCEPTION {
		TRY(igraph_connect_neighborhood(&_, order, (igraph_neimode_t)neimode));
		return *this;
	}

#pragma mark -
#pragma mark Games: Randomized Graph Generators

	::tempobj::force_temporary_class<Graph>::type Graph::grg_game(const Integer size, const Real radius, const PeriodicLattice periodic) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_grg_game(&_, size, radius, periodic, NULL, NULL));
		return ::tempobj::force_move(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	::tempobj::force_temporary_class<Graph>::type Graph::grg_game(const Integer size, const Real radius, const PeriodicLattice periodic, Vector& x_coords, Vector& y_coords) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_grg_game(&_, size, radius, periodic, &x_coords._, &y_coords._));
		return ::tempobj::force_move(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	::tempobj::force_temporary_class<Graph>::type Graph::barabasi_game(const Integer size, const Integer m, const Directedness directed, const BarabasiOutPref outpref) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_barabasi_game(&_, size, m, NULL, outpref, directed));
		return ::tempobj::force_move(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	::tempobj::force_temporary_class<Graph>::type Graph::barabasi_game(const Integer size, const Vector& outseq, const Directedness directed, const BarabasiOutPref outpref) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_barabasi_game(&_, size, 0, &outseq._, outpref, directed));
		return ::tempobj::force_move(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	
	/*
	// we assume how_many is small.
	static inline void XXINTRNL_random_choose(const ::gsl::Random& rangen, unsigned long* store, unsigned long how_many, double* discrete_distrib, unsigned long count, double total) {
		// because the distribution changes everytime we pick a number, it is unwise for us to cook the PDF into inverse CDF.
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
	*/
	
	::tempobj::force_temporary_class<Graph>::type Graph::barabasi_game_simple(const Integer size, const Integer m) MAY_THROW_EXCEPTION { return barabasi_game_simple(::gsl::Random::default_generator(), size, m); }
	::tempobj::force_temporary_class<Graph>::type Graph::barabasi_game_simple(const ::gsl::Random& rangen, const Integer size, const Integer m) MAY_THROW_EXCEPTION {
		if (size <= m+1)
			return Graph::full(size);
		else {
			unsigned long M = static_cast<unsigned long>(m), Size = static_cast<unsigned long>(size);
			Vertex* citations = new Vertex[2*M*Size];
			Vertex* citation_store = citations;
			for (unsigned long i = 0; i <= 2*M; ++ i)
				for (unsigned long j = i+1; j <= 2*M; ++ j) {
					*citation_store++ = i;
					*citation_store++ = j;
				}
													 
			for (unsigned long i = 2*M+1; i < Size; ++ i) {
				
				// randomly pick M _non_repeating_ members from the possible citations.
				for (unsigned long j = 0; j < M; ++ j) {
				choose_again:
					Vertex candidate_citation = citations[rangen.uniform_int(citation_store-citations)];
					for (unsigned long l = 0; l < j; ++ l)
						if (citation_store[l] == candidate_citation)
							goto choose_again;
					citation_store[j*2]   = i;
					citation_store[j*2+1] = candidate_citation;
				}
				
				citation_store += 2*M;
			}
			
			::tempobj::force_temporary_class<Graph>::type g = Graph::create(VertexVector::view(citations, 2*M*Size));
			
			delete[] citations;
			
			return g;
		}
	}

	::tempobj::force_temporary_class<Graph>::type Graph::watts_strogatz_game(const Integer size, const Integer K, const Real p, const Integer dimensions) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_watts_strogatz_game(&_, dimensions, size, K, p));
		return ::tempobj::force_move(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	::tempobj::force_temporary_class<Graph>::type Graph::watts_strogatz_game_simple(const Integer size, const Integer K, const Real p, const Integer dimensions) MAY_THROW_EXCEPTION {
		return watts_strogatz_game_simple(::gsl::Random::default_generator(), size, K, p, dimensions);
	}
	::tempobj::force_temporary_class<Graph>::type Graph::watts_strogatz_game_simple(const ::gsl::Random& rangen, const Integer size, const Integer K, const Real p, const Integer dimensions) MAY_THROW_EXCEPTION {
		::tempobj::force_temporary_class<Graph>::type base = dimensions == 1 ? Graph::ring(size) : Graph::lattice(Vector((long)dimensions).fill(size));
		return ::tempobj::force_move(base.connect_neighborhood(K).rewire_edges_simple(rangen, p));
	}
	
	::tempobj::force_temporary_class<Graph>::type Graph::erdos_renyi_game(Integer size, Real prob, Directedness directedness, SelfLoops self_loops) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_erdos_renyi_game(&_, IGRAPH_ERDOS_RENYI_GNP, size, prob, directedness, self_loops));
		return ::tempobj::force_move(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	::tempobj::force_temporary_class<Graph>::type Graph::erdos_renyi_Gnm_game(Integer size, Integer edges, Directedness directedness, SelfLoops self_loops) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_erdos_renyi_game(&_, IGRAPH_ERDOS_RENYI_GNM, size, edges, directedness, self_loops));
		return ::tempobj::force_move(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	::tempobj::force_temporary_class<Graph>::type Graph::erdos_renyi_game_simple(Integer size, Real prob, Directedness directedness, SelfLoops self_loops) MAY_THROW_EXCEPTION {
		return erdos_renyi_game_simple(::gsl::Random::default_generator(), size, prob, directedness, self_loops);
	}
#if __MSVC__
#define XXINTRNL_LOG1P(p) (::std::log(1.0 + (p)))	// there is no log1p in windows. fake with log(1+p).
#else
#define XXINTRNL_LOG1P(p) (::log1p(p))
#endif
	// Ref: https://networkx.lanl.gov/trac/browser/networkx/trunk/networkx/generators/random_graphs.py (fast_gnp_random_graph)
	::tempobj::force_temporary_class<Graph>::type Graph::erdos_renyi_game_simple(const ::gsl::Random& rangen, Integer size, Real prob, Directedness directedness, SelfLoops self_loops) MAY_THROW_EXCEPTION {
		VertexVector edges = VertexVector::n();
		long v = 1, w = -1, n = static_cast<long>(size);
		double lp = XXINTRNL_LOG1P(-prob);
		while (v < n) {
			double lr = XXINTRNL_LOG1P(-rangen.uniform());
			w += 1 + ::std::floor(lr / lp);
			while (w >= v && v < n) {
				w -= v;
				++ v;
			}
			if (v < n) {
				edges.push_back(w);
				edges.push_back(v);
			}
		}
		return Graph::create(edges, size);
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
	
	::tempobj::force_temporary_class<VertexVector>::type Graph::subcomponent(const Vertex representative, const NeighboringMode mode) const MAY_THROW_EXCEPTION {
		igraph_vector_t res;
		TRY(igraph_vector_init(&res, 0));
		TRY(igraph_subcomponent(&_, &res, representative, (igraph_neimode_t)mode));
		return ::tempobj::force_move(VertexVector(&res, ::tempobj::OwnershipTransferMove));
	}
	::tempobj::force_temporary_class<Graph>::type Graph::subgraph(const VertexSelector& vs) const MAY_THROW_EXCEPTION {
		igraph_t res;
		TRY(igraph_subgraph(&_, &res, vs._));
		return ::tempobj::force_move(Graph(&res, ::tempobj::OwnershipTransferMove));
	}
	void Graph::cluster(Vector& cluster_id_each_vertex_belongs_to, Vector& size_of_each_cluster, Connectedness connectedness) const MAY_THROW_EXCEPTION {
		igraph_vector_t membership, csize;
		TRY(igraph_vector_init(&membership, 0));
		TRY(igraph_vector_init(&csize, 0));
		TRY(igraph_clusters(&_, &membership, &csize, NULL, (igraph_connectedness_t)connectedness));
		cluster_id_each_vertex_belongs_to = ::tempobj::force_move(Vector(&membership, ::tempobj::OwnershipTransferMove));
		size_of_each_cluster = ::tempobj::force_move(Vector(&csize, ::tempobj::OwnershipTransferMove));
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
	::tempobj::force_temporary_class<ReferenceVector<Graph> >::type Graph::decompose(Connectedness connectedness, long max_component_number, long min_size_of_components) {
		igraph_vector_ptr_t res;
		TRY(igraph_vector_ptr_init(&res, 0));
		TRY(igraph_decompose(&_, &res, (igraph_connectedness_t)connectedness, max_component_number, min_size_of_components));
		return ReferenceVector<Graph>::adopt<igraph_t>(res);
	}
	::tempobj::force_temporary_class<VertexVector>::type Graph::articulation_points() const MAY_THROW_EXCEPTION {
		igraph_vector_t res;
		TRY(igraph_vector_init(&res, 0));
		TRY(igraph_articulation_points(&_, &res));
		return ::tempobj::force_move(VertexVector(&res, ::tempobj::OwnershipTransferMove));
	}

#pragma mark -
#pragma mark 10.9 Transitivity or Clustering Coefficient

	Real Graph::transitivity() const MAY_THROW_EXCEPTION {
		Real res;
		TRY(igraph_transitivity_undirected(&_, &res));
		return res;
	}
	Real Graph::transitivity_of(Vertex i) const MAY_THROW_EXCEPTION {
		Vector res (1);
		TRY(igraph_transitivity_local_undirected(&_, &res._, igraph_vss_1(i)));
		return res[0];
	}
	::tempobj::force_temporary_class<Vector>::type Graph::transitivity_local(const VertexSelector& vids) const MAY_THROW_EXCEPTION {
		Vector res ((long)vids.size(*this));
		TRY(igraph_transitivity_local_undirected(&_, &res._, vids._));
		return res;
	}
	::tempobj::force_temporary_class<Vector>::type Graph::transitivity_local() const MAY_THROW_EXCEPTION {
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
	::tempobj::force_temporary_class<BoolVector>::type Graph::is_loop(const EdgeSelector& es) const MAY_THROW_EXCEPTION {
		igraph_vector_bool_t res;
		TRY(igraph_vector_bool_init(&res, 0));
		TRY(igraph_is_loop(&_, &res, es._));
		return ::tempobj::force_move(BoolVector(&res, ::tempobj::OwnershipTransferMove));
	}
	::tempobj::force_temporary_class<BoolVector>::type Graph::is_multiple(const EdgeSelector& es) const MAY_THROW_EXCEPTION {
		igraph_vector_bool_t res;
		TRY(igraph_vector_bool_init(&res, 0));
		TRY(igraph_is_multiple(&_, &res, es._));
		return ::tempobj::force_move(BoolVector(&res, ::tempobj::OwnershipTransferMove));
	}	
	::tempobj::force_temporary_class<Vector>::type Graph::count_multiple(const EdgeSelector& es) const MAY_THROW_EXCEPTION {
		igraph_vector_t res;
		TRY(igraph_vector_init(&res, 0));
		TRY(igraph_count_multiple(&_, &res, es._));
		return ::tempobj::force_move(Vector(&res, ::tempobj::OwnershipTransferMove));
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
	
	::tempobj::force_temporary_class<ReferenceVector<Vector> >::type Graph::cliques(const Integer min_size, const Integer max_size) const {
		igraph_vector_ptr_t res;
		TRY(igraph_vector_ptr_init(&res, 0));
		TRY(igraph_cliques(&_, &res, min_size, max_size));
		return ReferenceVector<Vector>::adopt<igraph_vector_t>(res);
	}
	
	::tempobj::force_temporary_class<ReferenceVector<Vector> >::type Graph::cliques(const Integer max_size) const {
		return cliques(0, max_size);
	}
	
	::tempobj::force_temporary_class<ReferenceVector<Vector> >::type Graph::largest_cliques() const {
		igraph_vector_ptr_t res;
		TRY(igraph_vector_ptr_init(&res, 0));
		TRY(igraph_largest_cliques(&_, &res));
		return ReferenceVector<Vector>::adopt<igraph_vector_t>(res);
	}
	
	::tempobj::force_temporary_class<ReferenceVector<Vector> >::type Graph::maximal_cliques() const {
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

	::tempobj::force_temporary_class<GraphWriter>::type Graph::writer(const char* filename) const {
		return ::tempobj::force_move(GraphWriter(&_, filename));
	}
	::tempobj::force_temporary_class<GraphWriter>::type Graph::writer(std::FILE* filestream) const throw() {
		return ::tempobj::force_move(GraphWriter(&_, filestream));
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

	::tempobj::force_temporary_class<GraphReader>::type Graph::reader(const char* filename) { return ::tempobj::force_move(GraphReader(filename)); }
	::tempobj::force_temporary_class<GraphReader>::type Graph::reader(std::FILE* filestream) throw() { return ::tempobj::force_move(GraphReader(filestream)); }

	::tempobj::force_temporary_class<Graph>::type Graph::read(const char* filename, GraphFormat format) {
		if (format == GraphFormat_auto)
			format = identify_file_format(filename, false);
		if (format != GraphFormat_auto) {
			GraphReader reader = GraphReader(filename);
			switch (format) {
				case GraphFormat_adjlist:
					return ::tempobj::force_move(reader.adjlist());
//				case GraphFormat_dimacs:
				case GraphFormat_dot:
					break;
				case GraphFormat_edgelist:
					return ::tempobj::force_move(reader.edgelist());
				case GraphFormat_gml:
					return ::tempobj::force_move(reader.gml());
				case GraphFormat_graphdb:
					return ::tempobj::force_move(reader.graphdb());
				case GraphFormat_lgl:
					return ::tempobj::force_move(reader.lgl());
//				case GraphFormat_ncol:
				case GraphFormat_pajek:
					return ::tempobj::force_move(reader.pajek());
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
	return ::tempobj::force_move(Graph(&temp, ::tempobj::OwnershipTransferMove)); \

	::tempobj::force_temporary_class<Graph>::type Graph::disjoint_union(const Graph& x, const Graph& y) MAY_THROW_EXCEPTION {
		XXINTRNL_FORWARD_GRAPH_CREATION(tmp, igraph_disjoint_union(&tmp, &x._, &y._) );
	}
	::tempobj::force_temporary_class<Graph>::type Graph::disjoint_union(const ReferenceVector<Graph>& gg) MAY_THROW_EXCEPTION {
		XXINTRNL_FORWARD_GRAPH_CREATION(tmp, igraph_disjoint_union_many(&tmp, &gg._) );
	}
	::tempobj::force_temporary_class<Graph>::type Graph::merge(const Graph& x, const Graph& y) MAY_THROW_EXCEPTION {
		XXINTRNL_FORWARD_GRAPH_CREATION(tmp, igraph_union(&tmp, &x._, &y._) );
	}
	::tempobj::force_temporary_class<Graph>::type Graph::merge(const ReferenceVector<Graph>& gg) MAY_THROW_EXCEPTION {
		XXINTRNL_FORWARD_GRAPH_CREATION(tmp, igraph_union_many(&tmp, &gg._) );
	}
	::tempobj::force_temporary_class<Graph>::type Graph::intersection(const Graph& x, const Graph& y) MAY_THROW_EXCEPTION {
		XXINTRNL_FORWARD_GRAPH_CREATION(tmp, igraph_intersection(&tmp, &x._, &y._) );
	}
	::tempobj::force_temporary_class<Graph>::type Graph::intersection(const ReferenceVector<Graph>& gg) MAY_THROW_EXCEPTION {
		XXINTRNL_FORWARD_GRAPH_CREATION(tmp, igraph_intersection_many(&tmp, &gg._) );
	}
	::tempobj::force_temporary_class<Graph>::type Graph::difference(const Graph& x, const Graph& y) MAY_THROW_EXCEPTION {
		XXINTRNL_FORWARD_GRAPH_CREATION(tmp, igraph_difference(&tmp, &x._, &y._) );
	}
	::tempobj::force_temporary_class<Graph>::type Graph::complementer(const Graph& x, SelfLoops loops) MAY_THROW_EXCEPTION {
		XXINTRNL_FORWARD_GRAPH_CREATION(tmp, igraph_complementer(&tmp, &x._, (igraph_bool_t)loops) );
	}
	::tempobj::force_temporary_class<Graph>::type Graph::compose(const Graph& x, const Graph& y) MAY_THROW_EXCEPTION {
		XXINTRNL_FORWARD_GRAPH_CREATION(tmp, igraph_compose(&tmp, &x._, &y._) );
	}

	::tempobj::force_temporary_class<Graph>::type Graph::operator^ (const Graph& other) MAY_THROW_EXCEPTION {
		XXINTRNL_FORWARD_GRAPH_CREATION(tmp, igraph_disjoint_union(&tmp, &_, &other._) );
	}
	::tempobj::force_temporary_class<Graph>::type Graph::operator| (const Graph& other) MAY_THROW_EXCEPTION  {
		XXINTRNL_FORWARD_GRAPH_CREATION(tmp, igraph_union(&tmp, &_, &other._) );
	}
	::tempobj::force_temporary_class<Graph>::type Graph::operator& (const Graph& other) MAY_THROW_EXCEPTION {
		XXINTRNL_FORWARD_GRAPH_CREATION(tmp, igraph_intersection(&tmp, &_, &other._) );
	}
	::tempobj::force_temporary_class<Graph>::type Graph::operator- (const Graph& other) MAY_THROW_EXCEPTION {
		XXINTRNL_FORWARD_GRAPH_CREATION(tmp, igraph_difference(&tmp, &_, &other._) );
	}
	::tempobj::force_temporary_class<Graph>::type Graph::operator~ () MAY_THROW_EXCEPTION {
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
