/*

vector.cpp ... Wrapper implementation of igraph vector.

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

#ifndef IGRAPH_VECTOR_CPP
#define IGRAPH_VECTOR_CPP

#include <igraph/cpp/vector.hpp>
#include <cstring>

namespace igraph {
	
#pragma mark -
#pragma mark Constructors and Destructors
	
	MEMORY_MANAGER_IMPLEMENTATION(Vector);
	XXINTRNL_WRAPPER_CONSTRUCTOR_IMPLEMENTATION(Vector, igraph_vector_t, igraph_vector_copy);
	
	IMPLEMENT_COPY_METHOD(Vector) {
		TRY(igraph_vector_copy(&_, &other._));
	}
	IMPLEMENT_MOVE_METHOD(Vector) {
		_ = std::move(other._);
	}
	IMPLEMENT_DEALLOC_METHOD(Vector) {
		igraph_vector_destroy(&_);
	}
	
	Vector::Vector(Real* const array, const long count) MAY_THROW_EXCEPTION {
		XXINTRNL_DEBUG_CALL_INITIALIZER(Vector);
		TRY(igraph_vector_init_copy(&_, array, count));
	}
	
	/// Create a vector with "count" elements.
	Vector::Vector(const long count) MAY_THROW_EXCEPTION {
		XXINTRNL_DEBUG_CALL_INITIALIZER(Vector);
		TRY(igraph_vector_init(&_, count));
	}
	
#if XXINTRNL_CXX0X
	Vector::Vector(::std::initializer_list<Real> elements) MAY_THROW_EXCEPTION {
		XXINTRNL_DEBUG_CALL_INITIALIZER(Vector);
		TRY(igraph_vector_init(&_, elements.size()));
		Real* q = VECTOR(_);
		for (auto p = elements.begin(); p != elements.end(); ++p, ++q)
			*q = *p;
	}
#endif
	Vector::Vector(const char* vertices_as_string) MAY_THROW_EXCEPTION {
		TRY(igraph_vector_init(&_, 0));
		const char* end_of_string = ::std::strchr(vertices_as_string, '\0');
		while (vertices_as_string < end_of_string) {
			Real value;
			int consumed_length;
			if(::std::sscanf(vertices_as_string, "%lf %n", &value, &consumed_length) != 0) {
				vertices_as_string += consumed_length;
				TRY(igraph_vector_push_back(&_, value));
			} else
				++ vertices_as_string;
		}
	}
	
	RETRIEVE_TEMPORARY_CLASS(Vector) Vector::seq(const Real from, const Real to) MAY_THROW_EXCEPTION {
		igraph_vector_t _;
		TRY(igraph_vector_init_seq(&_, from, to));
		return ::std::move(Vector(&_, ::tempobj::OwnershipTransferMove));
	}
	
	RETRIEVE_TEMPORARY_CLASS(Vector) Vector::zeros(const long count) MAY_THROW_EXCEPTION {
		return ::std::move(Vector(count));
	}
	
	RETRIEVE_TEMPORARY_CLASS(Vector) Vector::n() MAY_THROW_EXCEPTION {
		igraph_vector_t _;
		TRY(igraph_vector_init(&_, 0));
		return ::std::move(Vector(&_, ::tempobj::OwnershipTransferMove));
	}
	
#pragma mark -
#pragma mark Initializing elements
	
	Vector& Vector::null() throw() { igraph_vector_null(&_); return *this; }
	Vector& Vector::fill(const Real e) throw() { igraph_vector_fill(&_, e); return *this; }
	
#pragma mark -
#pragma mark Accessing elements
	
	Real Vector::e(const long index) const throw() { return igraph_vector_e(&_, index); }
	Real* Vector::e_ptr(const long index) const throw() { return igraph_vector_e_ptr(&_, index); }
	Vector& Vector::set(const long index, const Real value) throw() { igraph_vector_set(&_, index, value); return *this; }
	Real Vector::tail() const throw() { return igraph_vector_tail(&_); }
	
#pragma mark -
#pragma mark Vector views
	
	RETRIEVE_TEMPORARY_CLASS(Vector) Vector::view(const Real* const array, const long count) throw() {
		igraph_vector_t _;
		igraph_vector_view(&_, array, count);
		return ::std::move(Vector(&_, ::tempobj::OwnershipTransferNoOwnership));
	}

#pragma mark -
#pragma mark Copying vectors
	
	void Vector::copy_to(Real* store) const throw() { igraph_vector_copy_to(&_, store); }
	Vector& Vector::update(const Vector& update_from) MAY_THROW_EXCEPTION { TRY(igraph_vector_update(&_, &update_from._)); return *this; }
	Vector& Vector::append(const Vector& append_from) MAY_THROW_EXCEPTION { TRY(igraph_vector_append(&_, &append_from._)); return *this; }
	void Vector::swap(Vector& swap_with) MAY_THROW_EXCEPTION { TRY(igraph_vector_swap(&_, &swap_with._)); }
	
#pragma mark -
#pragma mark Exchanging elements
	
	Vector& Vector::swap_elements(const long i, const long j) MAY_THROW_EXCEPTION { TRY(igraph_vector_swap_elements(&_, i, j)); return *this; }
	Vector& Vector::reverse() MAY_THROW_EXCEPTION { TRY(igraph_vector_reverse(&_)); return *this; }
	
#pragma mark -
#pragma mark Vector operations
	
	Vector& Vector::operator+= (const Real k) throw() { igraph_vector_add_constant(&_, k); return *this; }
	Vector& Vector::operator-= (const Real k) throw() { igraph_vector_add_constant(&_, -k); return *this; }
	Vector& Vector::operator*= (const Real k) throw() { igraph_vector_scale(&_, k); return *this; }
	Vector& Vector::operator/= (const Real k) throw() { igraph_vector_scale(&_, 1/k); return *this; }
	
	Vector& Vector::operator+= (const Vector& k) MAY_THROW_EXCEPTION { TRY(igraph_vector_add(&_, &k._)); return *this; }
	Vector& Vector::operator-= (const Vector& k) MAY_THROW_EXCEPTION { TRY(igraph_vector_sub(&_, &k._)); return *this; }
	Vector& Vector::operator*= (const Vector& k) MAY_THROW_EXCEPTION { TRY(igraph_vector_mul(&_, &k._)); return *this; }
	Vector& Vector::operator/= (const Vector& k) MAY_THROW_EXCEPTION { TRY(igraph_vector_div(&_, &k._)); return *this; }
	
	IMMEDIATE_OPERATOR_IMPLEMENTATION_COMMUTATIVE(Vector, +, Real);
	IMMEDIATE_OPERATOR_IMPLEMENTATION_RHS(Vector, -, Real);
	IMMEDIATE_OPERATOR_IMPLEMENTATION_COMMUTATIVE(Vector, *, Real);
	IMMEDIATE_OPERATOR_IMPLEMENTATION_RHS(Vector, /, Real);
	IMMEDIATE_OPERATOR_IMPLEMENTATION(Vector, +);
	IMMEDIATE_OPERATOR_IMPLEMENTATION(Vector, -);
	IMMEDIATE_OPERATOR_IMPLEMENTATION(Vector, *);
	IMMEDIATE_OPERATOR_IMPLEMENTATION(Vector, /);
	
#pragma mark -
#pragma mark Finding minimum and maximum
	
	Real Vector::min() const throw() { return igraph_vector_min(&_); }
	Real Vector::max() const throw() { return igraph_vector_max(&_); }
	long Vector::which_min() const throw() { return igraph_vector_which_min(&_); }
	long Vector::which_max() const throw() { return igraph_vector_which_max(&_); }
	void Vector::minmax(Real& minStore, Real& maxStore) const MAY_THROW_EXCEPTION { TRY(igraph_vector_minmax(&_, &minStore, &maxStore)); }
	void Vector::which_minmax(long& minStore, long& maxStore) const MAY_THROW_EXCEPTION { TRY(igraph_vector_which_minmax(&_, &minStore, &maxStore)); }
	
#pragma mark -
#pragma mark Vector properties
	
	bool Vector::empty() const throw() { return igraph_vector_empty(&_); }
	long Vector::size() const throw() { return igraph_vector_size(&_); }
	bool Vector::isnull() const throw() { return igraph_vector_isnull(&_); }
	
	Real Vector::sum() const throw() { return igraph_vector_sum(&_); }
	Real Vector::prod() const throw() { return igraph_vector_prod(&_); }
	bool Vector::isininterval(Real low, Real high) const throw() { return igraph_vector_isininterval(&_, low, high); }
	bool Vector::any_smaller(Real upper_limit) const throw() { return igraph_vector_any_smaller(&_, upper_limit); }
	bool Vector::operator== (const Vector& other) const throw() { return igraph_vector_is_equal(&_, &other._); }
	bool Vector::operator!= (const Vector& other) const throw() { return !(*this == other); }
	Real Vector::maxdifference(const Vector& other) const throw() { return igraph_vector_maxdifference(&_, &other._); }
	
#pragma mark -
#pragma mark Searching for elements
	
	bool Vector::contains(const Real e) const throw() { return igraph_vector_contains(&_, e); }
	bool Vector::search(const Real what, const long from) const throw() { return igraph_vector_search(&_, from, what, NULL); }
	bool Vector::search(const Real what, const long from, long& pos) const throw() { return igraph_vector_search(&_, from, what, &pos); }
	bool Vector::binsearch(const Real what) const throw() { return igraph_vector_binsearch2(&_, what); }
	bool Vector::binsearch(const Real what, long& pos) const throw() { return igraph_vector_binsearch(&_, what, &pos); }
	
#pragma mark -
#pragma mark Resizing operations
	
	Vector& Vector::clear() throw() { igraph_vector_clear(&_); return *this; }
	Vector& Vector::reserve(const long new_size) MAY_THROW_EXCEPTION { TRY(igraph_vector_reserve(&_, new_size)); return *this; }
	Vector& Vector::resize(const long new_size) MAY_THROW_EXCEPTION { TRY(igraph_vector_resize(&_, new_size)); return *this; }
	Vector& Vector::push_back(const Real e) MAY_THROW_EXCEPTION { TRY(igraph_vector_push_back(&_, e)); return *this; }
	Real Vector::pop_back() throw() { return igraph_vector_pop_back(&_); }
	Vector& Vector::insert(const long pos, const Real e) MAY_THROW_EXCEPTION { TRY(igraph_vector_insert(&_, pos, e)); return *this; }
	Vector& Vector::remove(const long pos) throw() { igraph_vector_remove(&_, pos); return *this; }
	Vector& Vector::remove_section(const long from, const long to) throw() { igraph_vector_remove_section(&_, from, to); return *this; }
	
#pragma mark -
#pragma mark Sorting
	
	Vector& Vector::sort() throw() { igraph_vector_sort(&_); return *this; }
	
#pragma mark -
#pragma mark Not Graph Related Functions
	RETRIEVE_TEMPORARY_CLASS(Vector) Vector::running_mean(const Integer binwidth) const MAY_THROW_EXCEPTION {
		igraph_vector_t res;
		TRY(igraph_vector_init(&res, 0));
		TRY(igraph_running_mean(&_, &res, binwidth));
		return ::std::move(Vector(&res, ::tempobj::OwnershipTransferMove));
	}
	
	RETRIEVE_TEMPORARY_CLASS(Vector) Vector::random_sample(const Integer low, const Integer high, const Integer vector_length) MAY_THROW_EXCEPTION {
		igraph_vector_t _;
		TRY(igraph_vector_init(&_, 0));
		TRY(igraph_random_sample(&_, low, high, vector_length));
		return ::std::move(Vector(&_, ::tempobj::OwnershipTransferMove));
	}
			
}

#endif