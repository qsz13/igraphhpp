/*

community.cpp ... Community detection algorithm.

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

#include <igraph/cpp/community.hpp>
#include <igraph/cpp/graph.hpp>

namespace igraph {
	Community::Community(const Graph& g) throw() : _g(&g._), _weights(NULL), _fixed(NULL), _modularity(NULL), _temperature(NULL), _membership(NULL), _csize(NULL), _merges(NULL), _result(NULL), _bridges(NULL), _edge_betweenness(NULL) {};
	
	Community& Community::set_weights(const Vector& edge_weights) { _weights = &edge_weights._; return *this; };
	Community& Community::set_fixed_labels(const BoolVector& fixed) { _fixed = &fixed._; return *this; };
	
	Community& Community::get_modularity(Vector& modularity_store) throw() { _modularity = &modularity_store._; return *this; }
	Community& Community::get_membership(Vector& membership_store) throw() { _membership = &membership_store._; return *this; }
	Community& Community::get_csize(Vector& csize_store) throw() { _csize = &csize_store._; return *this; }
	Community& Community::get_merges(Matrix& merges_store) throw() { _merges = &merges_store._; return *this; }
	
	Community& Community::get_edges_to_remove(Vector& ebres_store) throw() { _result = &ebres_store._; return *this; }
	Community& Community::get_bridges(Vector& ebbridge_store) throw() { _bridges = &ebbridge_store._; return *this; }
	Community& Community::get_edge_betweenness(Vector& eb_store) throw() { _edge_betweenness = &eb_store._; return *this; }

	Community& Community::get_temperature(Real& temperature_store) throw() { _temperature = &temperature_store; return *this; }
	
	
	void Community::spinglass(Integer spins, SpinglassUpdateScheme update_scheme, Real start_temperature, Real stop_temperature, Real cooling_factor, SpinglassUpdateRule update_rule, Real gamma) MAY_THROW_EXCEPTION {
		Real modularity_scalar;
		TRY(igraph_community_spinglass(_g, _weights, (_modularity != NULL) ? &modularity_scalar : NULL, _temperature, _membership, _csize, spins, static_cast<igraph_bool_t>(update_scheme), start_temperature, stop_temperature, cooling_factor, static_cast<igraph_spincomm_update_t>(update_rule), gamma));
		if (_modularity != NULL)
			igraph_vector_push_back(_modularity, modularity_scalar);
	}
		
	void Community::walktrap(int steps) MAY_THROW_EXCEPTION {
		// We should populate _merges also even if it is NULL, when _membership is requested.
		TRY(igraph_community_walktrap(_g, _weights, steps, _merges, _modularity));
	}
	
	void Community::edge_betweenness(Directedness directed) MAY_THROW_EXCEPTION {
		igraph_vector_t tmp_result;
		bool need_free_result = _result == NULL;
		if (need_free_result) {
			_result = &tmp_result;
			igraph_vector_init(_result, 0);
		}
		TRY(igraph_community_edge_betweenness(_g, _result, _edge_betweenness, _merges, _bridges, static_cast<igraph_bool_t>(directed)));
		if (need_free_result) {
			igraph_vector_destroy(_result);
		}
	}
	
	void Community::fastgreedy() MAY_THROW_EXCEPTION {
		TRY(igraph_community_fastgreedy(_g, _weights, _merges, _modularity));
	}
	
	/*
	void Community::label_propagation() MAY_THROW_EXCEPTION {
		TRY(igraph_community_label_propagation(_g, _membership, _weights, _membership, _fixed));
	}
	 */
}