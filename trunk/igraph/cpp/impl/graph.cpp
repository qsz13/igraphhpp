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
		return ::std::move(Graph(size, directedness));
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
		VertexVector res = VertexVector::n();
		TRY(igraph_neighbors(&_, &res._, vid, (igraph_neimode_t)neimode));
		return ::std::move(res);
	}

	RETRIEVE_TEMPORARY_CLASS(EdgeVector) Graph::adjacent(Vertex vid, NeighboringMode neimode) const MAY_THROW_EXCEPTION {
		EdgeVector res = EdgeVector::n();
		TRY(igraph_adjacent(&_, &res._, vid, (igraph_neimode_t)neimode));
		return ::std::move(res);
	}

	RETRIEVE_TEMPORARY_CLASS(Vector) Graph::degree(const VertexSelector& vids, NeighboringMode neimode, SelfLoops countLoops) const MAY_THROW_EXCEPTION {
		Vector res ((long)vids.size(*this));
		TRY(igraph_degree(&_, &res._, vids._, (igraph_neimode_t)neimode, countLoops));
		return ::std::move(res);
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
		return ::std::move(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	// TODO: igraph_adjacency when Matrix is implemented.
	// TODO: igraph_weighted_adjacency when Matrix is implemented.
	// TODO: igraph_adjlist when AdjacencyList is implemented.
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::star(const Integer n, const StarMode mode, const Vertex center) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_star(&_, n, (igraph_star_mode_t)mode, center));
		return ::std::move(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::lattice(const Vector& dimensions, const PeriodicLattice periodic, const Integer step, const Directedness directedness, const MutualConnections mutual) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_lattice(&_, &dimensions._, step, directedness, mutual, periodic));
		return ::std::move(Graph(&_, ::tempobj::OwnershipTransferMove));
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
		return ::std::move(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::tree(const Integer n, const Integer children, const TreeMode type) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_tree(&_, n, children, (igraph_tree_mode_t)type));
		return ::std::move(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::full(const Integer n, const Directedness directedness, const SelfLoops loops) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_full(&_, n, directedness, loops));
		return ::std::move(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::complete(const Integer n, const Directedness directedness, const SelfLoops loops) MAY_THROW_EXCEPTION {
		return full(n, directedness, loops);
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::full_citation(const Integer n, const Directedness directedness) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_full_citation(&_, n, directedness));
		return ::std::move(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::famous(const char* name) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_famous(&_, name));
		return ::std::move(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::lcf_vector(const Integer n, const Vector& shifts, const Integer repeats) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_lcf_vector(&_, n, &shifts._, repeats));
		return ::std::move(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::atlas(const int number) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_atlas(&_, number));
		return ::std::move(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::de_bruijn(const Integer m, const Integer n) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_de_bruijn(&_, m, n));
		return ::std::move(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::kautz(const Integer m, const Integer n) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_kautz(&_, m, n));
		return ::std::move(Graph(&_, ::tempobj::OwnershipTransferMove));
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
		return ::std::move(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::grg_game(const Integer size, const Real radius, const PeriodicLattice periodic, Vector& x_coords, Vector& y_coords) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_grg_game(&_, size, radius, periodic, &x_coords._, &y_coords._));
		return ::std::move(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::barabasi_game(const Integer size, const Integer m, const Directedness directed, const BarabasiOutPref outpref) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_barabasi_game(&_, size, m, NULL, outpref, directed));
		return ::std::move(Graph(&_, ::tempobj::OwnershipTransferMove));
	}
	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::barabasi_game(const Integer size, const Vector& outseq, const Directedness directed, const BarabasiOutPref outpref) MAY_THROW_EXCEPTION {
		igraph_t _;
		TRY(igraph_barabasi_game(&_, size, 0, &outseq._, outpref, directed));
		return ::std::move(Graph(&_, ::tempobj::OwnershipTransferMove));
	}

#pragma mark -
#pragma mark Basic Properties

	bool Graph::are_connected(const Vertex from, const Vertex to) MAY_THROW_EXCEPTION {
		igraph_bool_t res;
		TRY(igraph_are_connected(&_, from, to, &res));
		return res;
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
		return ::std::move(GraphWriter(&_, filename));
	}
	RETRIEVE_TEMPORARY_CLASS(GraphWriter) Graph::writer(std::FILE* filestream) const throw() {
		return ::std::move(GraphWriter(&_, filestream));
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

	RETRIEVE_TEMPORARY_CLASS(GraphReader) Graph::reader(const char* filename) { return ::std::move(GraphReader(filename)); }
	RETRIEVE_TEMPORARY_CLASS(GraphReader) Graph::reader(std::FILE* filestream) throw() { return ::std::move(GraphReader(filestream)); }

	RETRIEVE_TEMPORARY_CLASS(Graph) Graph::read(const char* filename, GraphFormat format) {
		if (format == GraphFormat_auto)
			format = identify_file_format(filename, false);
		if (format != GraphFormat_auto) {
			GraphReader reader = GraphReader(filename);
			switch (format) {
				case GraphFormat_adjlist:
					return ::std::move(reader.adjlist());
//				case GraphFormat_dimacs:
				case GraphFormat_dot:
					break;
				case GraphFormat_edgelist:
					return ::std::move(reader.edgelist());
				case GraphFormat_gml:
					return ::std::move(reader.gml());
				case GraphFormat_graphdb:
					return ::std::move(reader.graphdb());
				case GraphFormat_lgl:
					return ::std::move(reader.lgl());
//				case GraphFormat_ncol:
				case GraphFormat_pajek:
					return ::std::move(reader.pajek());
				default:
					break;
			}
		}
		throw ::std::runtime_error("File type cannot be determined.");
	}
}

#endif
