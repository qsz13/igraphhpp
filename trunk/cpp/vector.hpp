/*
 
 vector.hpp ... Wrapper for igraph vectors 
 
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

/**
 \file vector.hpp
 \brief Wrapper for igraph vectors
 \author KennyTM~
 \date June 12th, 2009
 */

#ifndef IGRAPH_VECTOR_HPP
#define IGRAPH_VECTOR_HPP

#include <igraph/igraph.h>
#include <igraph/cpp/common.hpp>
#include <igraph/cpp/exception.hpp>
#include <igraph/cpp/tempobj.hpp>
#include <cstdarg>

namespace igraph {
	/**
	 \class Vector
	 \brief Wrapper for igraph vectors
	 
	 This class is a wrapper of the igraph_vector_t type, which is an array of Real.
	 */
	class Vector {
	private:
		igraph_vector_t _;
		Vector() throw() {};
		
	public:
		MEMORY_MANAGER_INTERFACE(Vector);
		
		/// Create a vector with "count" elements filled with zero.
		Vector(const long count) throw(Exception) {
			TRY(igraph_vector_init(&_, count));
		}		
		
		/// Copy a C array into a Vector.
		Vector(Real* const array, const long count) throw(Exception) {
			TRY(igraph_vector_init_copy(&_, array, count));
		}
		
		/// Create a vector sequentially between two values inclusively.
		static Vector seq(const Real from, const Real to) throw(Exception) {
			Vector v;
			TRY(igraph_vector_init_seq(&v._, from, to));
			return v;
		}
				
		/// Wrap a C array as a Vector. 
		static Vector view(const Real* const array, const long count) throw() {
			Vector v;
			v.mm_dont_dealloc = true;
			igraph_vector_view(&v._, array, count);
			return v;
		}
		
		/// var_arg method to construct the vector
		Vector(const long count, ...) throw(Exception) {
#if __GNUC__ >= 3
			Real arr[count];
#else
			Real* arr = new Real[count];
#endif
			std::va_list va;
			va_start(va, count);
			for (long i = 0; i < count; ++ i)
				arr[i] = va_arg(va, Real);
			va_end(va);
			
			TRY(igraph_vector_init_copy(&_, arr, count));
			
#if !(__GNUC__ >= 3)
			// FIXME: May cause memory leak when an exception is thrown.
			delete[] arr;
#endif
		}
		
		/// Wrap or copy an igraph_vector_t to a Vector.
		Vector(const igraph_vector_t& orig, const OwnershipTransfer transfer = OwnershipTransferKeepOriginal) throw(Exception) : COMMON_INIT_WITH(transfer) {
			if (transfer == OwnershipTransferCopy) {
				TRY(igraph_vector_copy(&_, &orig));
			} else
				_ = orig;
		}
		
		
		void null() throw() { igraph_vector_null(&_); }
		void fill(const Real e) throw() { igraph_vector_fill(&_, e); }
		
		Real* ptr() throw() { return VECTOR(_); }
		const Real& operator[](const long index) const throw() { return VECTOR(_)[index]; }
		Real& operator[](const long index) throw() { return VECTOR(_)[index]; }
		
		Real e(const long index) const throw() { return igraph_vector_e(&_, index); }
		Real* e_ptr(const long index) const throw() { return igraph_vector_e_ptr(&_, index); }
		void set(const long index, const Real value) throw() { igraph_vector_set(&_, index, value); }
		Real tail() const throw() { return igraph_vector_tail(&_); }
		
		void copy_to(Real* store) const throw() { igraph_vector_copy_to(&_, store); }
		void update(const Vector& update_from) throw(Exception) { TRY(igraph_vector_update(&_, &update_from._)); }
		void append(const Vector& append_from) throw(Exception) { TRY(igraph_vector_append(&_, &append_from._)); }
		void swap(Vector& swap_with) throw(Exception) { TRY(igraph_vector_swap(&_, &swap_with._)); }
		
		void swap_elements(const long i, const long j) throw(Exception) { TRY(igraph_vector_swap_elements(&_, i, j)); }
		void reverse() throw(Exception) { TRY(igraph_vector_reverse(&_)); }
		
		Vector& operator+= (const Real k) throw() { igraph_vector_add_constant(&_, k); return *this; }
		Vector& operator-= (const Real k) throw() { igraph_vector_add_constant(&_, -k); return *this; }
		Vector& operator*= (const Real k) throw() { igraph_vector_scale(&_, k); return *this; }
		Vector& operator/= (const Real k) throw() { igraph_vector_scale(&_, 1/k); return *this; }
		
		Vector& operator+= (const Vector& k) throw(Exception) { TRY(igraph_vector_add(&_, &k._)); return *this; }
		Vector& operator-= (const Vector& k) throw(Exception) { TRY(igraph_vector_sub(&_, &k._)); return *this; }
		Vector& operator*= (const Vector& k) throw(Exception) { TRY(igraph_vector_mul(&_, &k._)); return *this; }
		Vector& operator/= (const Vector& k) throw(Exception) { TRY(igraph_vector_div(&_, &k._)); return *this; }
		
		Real min() const throw() { return igraph_vector_min(&_); }
		Real max() const throw() { return igraph_vector_max(&_); }
		long which_min() const throw() { return igraph_vector_which_min(&_); }
		long which_max() const throw() { return igraph_vector_which_max(&_); }
		void minmax(Real& minStore, Real& maxStore) const throw(Exception) { TRY(igraph_vector_minmax(&_, &minStore, &maxStore)); }
		void which_minmax(long& minStore, long& maxStore) const throw(Exception) { TRY(igraph_vector_which_minmax(&_, &minStore, &maxStore)); }
		
		bool empty() const throw() { return igraph_vector_empty(&_); }
		long size() const throw() { return igraph_vector_size(&_); }
		
		Real sum() const throw() { return igraph_vector_sum(&_); }
		Real prod() const throw() { return igraph_vector_prod(&_); }
		bool isininterval(Real low, Real high) const throw() { return igraph_vector_isininterval(&_, low, high); }
		bool operator== (const Vector& other) const throw() { return igraph_vector_is_equal(&_, &other._); }
		bool operator!= (const Vector& other) const throw() { return !(*this == other); }
		Real maxdifference(const Vector& other) const throw() { return igraph_vector_maxdifference(&_, &other._); }
		
		bool contains(const Real e) const throw() { return igraph_vector_contains(&_, e); }
		bool search(const Real what, const long from = 0) const throw() { return igraph_vector_search(&_, from, what, NULL); }
		bool search(const Real what, const long from, long& pos) const throw() { return igraph_vector_search(&_, from, what, &pos); }
		bool binsearch(const Real what) const throw() { return igraph_vector_binsearch2(&_, what); }
		bool binsearch(const Real what, long& pos) const throw() { return igraph_vector_binsearch(&_, what, &pos); }
		
		void clear() throw() { igraph_vector_clear(&_); }
		void reserve(const long new_size) throw(Exception) { TRY(igraph_vector_reserve(&_, new_size)); }
		void resize(const long new_size) throw(Exception) { TRY(igraph_vector_resize(&_, new_size)); }
		void push_back(const Real e) throw(Exception) { TRY(igraph_vector_push_back(&_, e)); }
		Real pop_back() throw() { return igraph_vector_pop_back(&_); }
		void insert(const long pos, const Real e) throw(Exception) { TRY(igraph_vector_insert(&_, pos, e)); }
		void remove(const long pos) throw() { igraph_vector_remove(&_, pos); }
		void remove_section(const long from, const long to) throw() { igraph_vector_remove_section(&_, from, to); }
		
		void sort() throw() { igraph_vector_sort(&_); }
		
		// STL support
		typedef Real value_type;
		typedef value_type* pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef unsigned size_type;
		typedef int difference_type;
		typedef value_type* iterator;
		typedef const value_type* const_iterator;
		
		iterator begin() { return _.stor_begin; }
		iterator end() { return _.end; }
		const_iterator begin() const { return _.stor_begin; }
		const_iterator end() const { return _.end; }
		size_type capacity() const { return _.stor_end - _.stor_begin; }
		reference front() { return *_.stor_begin; }
		const_reference front() const { *_.stor_begin; }
		reference back() { return *(_.end-1); }
		const_reference back() const { return *(_.end-1); }
		
		/// Print content of the Vector.
		void print() const throw() {
			for (const_iterator cit = begin(); cit != end(); ++ cit)
				printf("%lg, ", *cit);
			printf("\n");
		}
		
		/// Convert from std::vector.
		template<typename InputIterator>
		Vector(const InputIterator from, const InputIterator to) throw(Exception) {
			if (from != to) {
				TRY(igraph_vector_init(&_, 1));
				for (const InputIterator cit = from; cit != to; ++ cit)
					TRY(igraph_vector_push_back(&_, *cit));
			} else
				mm_dont_dealloc = true;
		}
		
		friend class VertexSelector;
		friend class VertexIterator;
	};

	MEMORY_MANAGER_IMPLEMENTATION_ATTR(inline, Vector);
	
	IMPLEMENT_COPY_METHOD(Vector) {
		igraph_vector_copy(&_, &other._);
	}
	IMPLEMENT_MOVE_METHOD(Vector) {
		_ = std::move(other._);
	}
	IMPLEMENT_DEALLOC_METHOD(Vector) {
		igraph_vector_destroy(&_);
	}

	IMMEDIATE_OPERATOR_IMPLEMENTATION_COMMUTATIVE_ATTR(static inline, Vector, +, Real);
	IMMEDIATE_OPERATOR_IMPLEMENTATION_RHS_ATTR(static inline, Vector, -, Real);
	IMMEDIATE_OPERATOR_IMPLEMENTATION_COMMUTATIVE_ATTR(static inline, Vector, *, Real);
	IMMEDIATE_OPERATOR_IMPLEMENTATION_RHS_ATTR(static inline, Vector, /, Real);
	IMMEDIATE_OPERATOR_IMPLEMENTATION_ATTR(static inline, Vector, +);
	IMMEDIATE_OPERATOR_IMPLEMENTATION_ATTR(static inline, Vector, -);
	IMMEDIATE_OPERATOR_IMPLEMENTATION_ATTR(static inline, Vector, *);
	IMMEDIATE_OPERATOR_IMPLEMENTATION_ATTR(static inline, Vector, /);
}
										   
#endif