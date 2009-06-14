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

namespace igraph {
	
#pragma mark -
#pragma mark Constructors and Destructors
	
	MEMORY_MANAGER_IMPLEMENTATION(Vector);
	XXINTRNL_WRAPPER_CONSTRUCTOR_IMPLEMENTATION(Vector, igraph_vector_t, igraph_vector_copy);
	
	IMPLEMENT_COPY_METHOD(Vector) MAY_THROW_EXCEPTION {
		TRY(igraph_vector_copy(&_, &other._));
	}
	IMPLEMENT_MOVE_METHOD(Vector) throw() {
		_ = std::move(other._);
	}
	IMPLEMENT_DEALLOC_METHOD(Vector) {
		igraph_vector_destroy(&_);
	}
	
	Vector::Vector(Real* const array, const long count) MAY_THROW_EXCEPTION {
		XXINTRNL_DEBUG_CALL_INITIALIZER(Vector);
		TRY(igraph_vector_init_copy(&_, array, count));
	}
	
	Vector::Vector(const long count, const Real first, ...) MAY_THROW_EXCEPTION {
		XXINTRNL_DEBUG_CALL_INITIALIZER(Vector);
#if __GNUC__ >= 3
		Real arr[count];
#else
		Real* arr = new Real[count];
#endif
		arr[0] = first;
		std::va_list va;
		va_start(va, first);
		for (long i = 1; i < count; ++ i)
			arr[i] = va_arg(va, Real);
		va_end(va);
		
		TRY(igraph_vector_init_copy(&_, arr, count));
		
#if !(__GNUC__ >= 3)
		// FIXME: May cause memory leak when an exception is thrown.
		delete[] arr;
#endif
	}
	
	::tempobj::temporary_class<Vector>::type Vector::seq(const Real from, const Real to) MAY_THROW_EXCEPTION {
		igraph_vector_t _;
		TRY(igraph_vector_init_seq(&_, from, to));
		return ::std::move(Vector(&_, ::tempobj::OwnershipTransferMove));
	}
	
	::tempobj::temporary_class<Vector>::type Vector::zeros(const long count) MAY_THROW_EXCEPTION {
		igraph_vector_t _;
		TRY(igraph_vector_init(&_, count));
		return ::std::move(Vector(&_, ::tempobj::OwnershipTransferMove));
	}
	
#pragma mark -
#pragma mark Initializing elements
	
	void Vector::null() throw() { igraph_vector_null(&_); }
	void Vector::fill(const Real e) throw() { igraph_vector_fill(&_, e); }
	
#pragma mark -
#pragma mark Accessing elements
	
	Real Vector::e(const long index) const throw() { return igraph_vector_e(&_, index); }
	Real* Vector::e_ptr(const long index) const throw() { return igraph_vector_e_ptr(&_, index); }
	void Vector::set(const long index, const Real value) throw() { igraph_vector_set(&_, index, value); }
	Real Vector::tail() const throw() { return igraph_vector_tail(&_); }
	
#pragma mark -
#pragma mark Vector views
	
	::tempobj::temporary_class<Vector>::type Vector::view(const Real* const array, const long count) throw() {
		igraph_vector_t _;
		igraph_vector_view(&_, array, count);
		return ::std::move(Vector(&_, ::tempobj::OwnershipTransferNoOwnership));
	}

#pragma mark -
#pragma mark Copying vectors
	
	void Vector::copy_to(Real* store) const throw() { igraph_vector_copy_to(&_, store); }
	void Vector::update(const Vector& update_from) MAY_THROW_EXCEPTION { TRY(igraph_vector_update(&_, &update_from._)); }
	void Vector::append(const Vector& append_from) MAY_THROW_EXCEPTION { TRY(igraph_vector_append(&_, &append_from._)); }
	void Vector::swap(Vector& swap_with) MAY_THROW_EXCEPTION { TRY(igraph_vector_swap(&_, &swap_with._)); }
	
#pragma mark -
#pragma mark Exchanging elements
	
	void Vector::swap_elements(const long i, const long j) MAY_THROW_EXCEPTION { TRY(igraph_vector_swap_elements(&_, i, j)); }
	void Vector::reverse() MAY_THROW_EXCEPTION { TRY(igraph_vector_reverse(&_)); }
	
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
	
	Real Vector::sum() const throw() { return igraph_vector_sum(&_); }
	Real Vector::prod() const throw() { return igraph_vector_prod(&_); }
	bool Vector::isininterval(Real low, Real high) const throw() { return igraph_vector_isininterval(&_, low, high); }
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
	
	void Vector::clear() throw() { igraph_vector_clear(&_); }
	void Vector::reserve(const long new_size) MAY_THROW_EXCEPTION { TRY(igraph_vector_reserve(&_, new_size)); }
	void Vector::resize(const long new_size) MAY_THROW_EXCEPTION { TRY(igraph_vector_resize(&_, new_size)); }
	void Vector::push_back(const Real e) MAY_THROW_EXCEPTION { TRY(igraph_vector_push_back(&_, e)); }
	Real Vector::pop_back() throw() { return igraph_vector_pop_back(&_); }
	void Vector::insert(const long pos, const Real e) MAY_THROW_EXCEPTION { TRY(igraph_vector_insert(&_, pos, e)); }
	void Vector::remove(const long pos) throw() { igraph_vector_remove(&_, pos); }
	void Vector::remove_section(const long from, const long to) throw() { igraph_vector_remove_section(&_, from, to); }
	
#pragma mark -
#pragma mark Sorting
	
	void Vector::sort() throw() { igraph_vector_sort(&_); }
	
#pragma mark -
#pragma mark Not Graph Related Functions
	::tempobj::temporary_class<Vector>::type Vector::running_mean(const Integer binwidth) const MAY_THROW_EXCEPTION {
		igraph_vector_t res;
		TRY(igraph_vector_init(&res, 0));
		TRY(igraph_running_mean(&_, &res, binwidth));
		return ::std::move(Vector(&res, ::tempobj::OwnershipTransferMove));
	}
	
	::tempobj::temporary_class<Vector>::type Vector::random_sample(const Integer low, const Integer high, const Integer vector_length) MAY_THROW_EXCEPTION {
		igraph_vector_t _;
		TRY(igraph_vector_init(&_, 0));
		TRY(igraph_random_sample(&_, low, high, vector_length));
		return ::std::move(Vector(&_, ::tempobj::OwnershipTransferMove));
	}
		
}

#endif