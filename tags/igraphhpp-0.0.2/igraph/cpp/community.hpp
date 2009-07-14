/*

community.hpp ... Community detection algorithms.

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

#ifndef IGRAPH_COMMUNITY_HPP
#define IGRAPH_COMMUNITY_HPP

#include <igraph/igraph.h>
#include <igraph/cpp/exception.hpp>
#include <igraph/cpp/vector.hpp>
#include <igraph/cpp/matrix.hpp>

namespace igraph {
	class Graph;
	
	class Community {
		const igraph_t* _g;
		const igraph_vector_t* _weights;
		const igraph_vector_bool_t* _fixed;
		
		igraph_vector_t* _modularity;
		igraph_real_t* _temperature;
		igraph_vector_t* _membership;
		igraph_vector_t* _csize;
		igraph_matrix_t* _merges;
		
		igraph_vector_t* _result, *_bridges, *_edge_betweenness;
				
	public:
		
		Community(const Graph& graph) throw();
		
		Community& set_weights(const Vector& edge_weights) throw();
		
		Community& set_fixed_labels(const BoolVector& fixed) throw();
		
		Community& get_modularity(Vector& modularity_store) throw();
		Community& get_membership(Vector& membership_store) throw();
		Community& get_csize(Vector& csize_store) throw();
		Community& get_merges(Matrix& merges_store) throw();
		
		// For edge betweeness only.
		Community& get_edges_to_remove(Vector& ebres_store) throw();
		Community& get_bridges(Vector& ebbridge_store) throw();
		Community& get_edge_betweenness(Vector& eb_store) throw();
		
		// For spinglass only.
		Community& get_temperature(Real& temperature_store) throw();
		
		
		enum SpinglassUpdateScheme {
			Asynchronous = 0,
			Synchronous = 1
		};
		enum SpinglassUpdateRule {
			Simple = IGRAPH_SPINCOMM_UPDATE_SIMPLE,
			Config = IGRAPH_SPINCOMM_UPDATE_CONFIG
		};
		
		void spinglass(Integer spins = 25, SpinglassUpdateScheme update_scheme = Asynchronous,
					   Real start_temperature = 1, Real stop_temperature = 0.01, Real cooling_factor = 0.99,
					   SpinglassUpdateRule update_rule = Simple, Real gamma = 1) MAY_THROW_EXCEPTION;
		void walktrap(int steps = 4) MAY_THROW_EXCEPTION;
		void edge_betweenness(Directedness directed = Directed) MAY_THROW_EXCEPTION;
		void fastgreedy() MAY_THROW_EXCEPTION;
		void label_propagation() MAY_THROW_EXCEPTION;
	};
}

#endif