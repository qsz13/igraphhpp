/*

mincut.cpp ... the mincut object to be called

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

#include <igraph/cpp/mincut.hpp>
#include <igraph/cpp/graph.hpp>

namespace igraph {

	Mincut::Mincut(const Graph& ig) throw() : partition1(NULL), partition2(NULL), cut(NULL), capacity(NULL), called(false) {
		g = ig.get();
	}
	Mincut::~Mincut() {
		if(!called)  compute();
	}

	Mincut& Mincut::get_partition1(Vector& first_partition) throw() {
		partition1 = &first_partition._;
		return *this;
	}
	Mincut& Mincut::get_partition2(Vector& second_partition) throw() {
		partition2 = &second_partition._;
		return *this;
	}
	Mincut& Mincut::get_cut(Vector& set_of_cut_edges) throw() {
		cut = &set_of_cut_edges._;
		return *this;
	}
	Mincut& Mincut::set_capacity(const Vector& edge_capacity) throw() {
		capacity = &edge_capacity._;
		return *this;
	}

	Integer Mincut::compute() MAY_THROW_EXCEPTION {
		Integer value;
		TRY( igraph_mincut(g, &value, partition1, partition2, cut, capacity) );
		called = false;
		return value;
	}
}
