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
#include <cstdarg>
#include <cstdio>
#if XXINTRNL_CXX0X
#include <initializer_list>
#endif

namespace igraph {
	/**
	 \class Vector
	 \brief Wrapper for igraph vectors
	 
	 This class is a wrapper of the igraph_vector_t type, which is an array of Real.
	 */
	class Vector {
	private:
		igraph_vector_t _;
		
	public:
		MEMORY_MANAGER_INTERFACE(Vector);
		XXINTRNL_WRAPPER_CONSTRUCTOR_INTERFACE(Vector, igraph_vector_t);
				
		/// Copy a C array into a Vector.
		Vector(Real* const array, const long count) MAY_THROW_EXCEPTION;
		
		/// Create a vector with "count" elements.
		Vector(const long count) MAY_THROW_EXCEPTION;
		
#if XXINTRNL_CXX0X
		/// Create a Vector using initializer list (C++0x only.)
		Vector(::std::initializer_list<Real> elements) MAY_THROW_EXCEPTION;
#endif
		/**
		 \brief Create a Vector using content of string.
		 
		 Example:
		 \code
		 Vector v = Vector("42 54 64 75");
		 printf("%lg", v.prod());	// prints 10886400
		 \endcode
		*/
		Vector(const char* stringized_elements) MAY_THROW_EXCEPTION;
		
		/// Create a vector sequentially between two values inclusively.
		static RETRIEVE_TEMPORARY_CLASS(Vector) seq(const Real from, const Real to) MAY_THROW_EXCEPTION;
		
		/// Create a vector with "count" elements filled with zero.
		__attribute__((deprecated,warning("Vector::zeros is deprecated. Use the constructor Vector(x) (for x > 0) or Vector::n (for x = 0) instead.")))
		static RETRIEVE_TEMPORARY_CLASS(Vector) zeros(const long count = 0) MAY_THROW_EXCEPTION;
		
		static RETRIEVE_TEMPORARY_CLASS(Vector) n() MAY_THROW_EXCEPTION;
		
		/// Wrap a C array as a Vector. 
		static RETRIEVE_TEMPORARY_CLASS(Vector) view(const Real* const array, const long count) throw();
		
		Vector& null() throw();
		Vector& fill(const Real e) throw();
		
		Real* ptr() throw() { return VECTOR(_); }
		const Real& operator[](const long index) const throw() { return VECTOR(_)[index]; }
		Real& operator[](const long index) throw() { return VECTOR(_)[index]; }
		
		Real e(const long index) const throw();
		Real* e_ptr(const long index) const throw();
		Vector& set(const long index, const Real value) throw();
		Real tail() const throw();
		
		void copy_to(Real* store) const throw();
		Vector& update(const Vector& update_from) MAY_THROW_EXCEPTION;
		Vector& append(const Vector& append_from) MAY_THROW_EXCEPTION;
		void swap(Vector& swap_with) MAY_THROW_EXCEPTION;
		
		Vector& swap_elements(const long i, const long j) MAY_THROW_EXCEPTION;
		Vector& reverse() MAY_THROW_EXCEPTION;
		
		Vector& operator+= (const Real k) throw();
		Vector& operator-= (const Real k) throw();
		Vector& operator*= (const Real k) throw();
		Vector& operator/= (const Real k) throw();
		
		Vector& operator+= (const Vector& k) MAY_THROW_EXCEPTION;
		Vector& operator-= (const Vector& k) MAY_THROW_EXCEPTION;
		Vector& operator*= (const Vector& k) MAY_THROW_EXCEPTION;
		Vector& operator/= (const Vector& k) MAY_THROW_EXCEPTION;
		
		Real min() const throw();
		Real max() const throw();
		long which_min() const throw();
		long which_max() const throw();
		void minmax(Real& minStore, Real& maxStore) const MAY_THROW_EXCEPTION;
		void which_minmax(long& minStore, long& maxStore) const MAY_THROW_EXCEPTION;
		
		bool empty() const throw();
		long size() const throw();
		
		Real sum() const throw();
		Real prod() const throw();
		bool isininterval(Real low, Real high) const throw();
		bool any_smaller(Real upper_limit) const throw();
		bool operator== (const Vector& other) const throw();
		bool operator!= (const Vector& other) const throw();
		Real maxdifference(const Vector& other) const throw();
		
		bool contains(const Real e) const throw();
		bool search(const Real what, const long from = 0) const throw();
		bool search(const Real what, const long from, long& pos) const throw();
		bool binsearch(const Real what) const throw();
		bool binsearch(const Real what, long& pos) const throw();
		
		Vector& clear() throw();
		Vector& reserve(const long new_size) MAY_THROW_EXCEPTION;
		Vector& resize(const long new_size) MAY_THROW_EXCEPTION;
		Vector& push_back(const Real e) MAY_THROW_EXCEPTION;
		Real pop_back() throw();
		Vector& insert(const long pos, const Real e) MAY_THROW_EXCEPTION;
		Vector& remove(const long pos) throw();
		Vector& remove_section(const long from, const long to) throw();
		
		Vector& sort() throw();
				
		// STL support
		typedef Real value_type;
		typedef value_type* pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef unsigned size_type;
		typedef int difference_type;
		typedef value_type* iterator;
		typedef const value_type* const_iterator;
		
		iterator begin() throw() { return _.stor_begin; }
		iterator end() throw() { return _.end; }
		const_iterator begin() const throw() { return _.stor_begin; }
		const_iterator end() const throw() { return _.end; }
		size_type capacity() const throw() { return _.stor_end - _.stor_begin; }
		reference front() throw() { return *_.stor_begin; }
		const_reference front() const throw() { return *_.stor_begin; }
		reference back() throw() { return *(_.end-1); }
		const_reference back() const throw() { return *(_.end-1); }
		
		/// Print content of the Vector.
		void print(std::FILE* f = stdout) const throw() {
			bool is_first = true;
			for (const_iterator cit = begin(); cit != end(); ++ cit) {
				if (is_first) {
					::std::fprintf(f, "%lg", *cit);
					is_first = false;
				} else
					::std::fprintf(f, " %lg", *cit);
			}
			::std::fprintf(f, "\n");
		}
		
		/// Convert from std::vector.
		template<typename InputIterator>
		Vector(const InputIterator from, const InputIterator to) MAY_THROW_EXCEPTION {
			if (from != to) {
				TRY(igraph_vector_init(&_, 1));
				for (const InputIterator cit = from; cit != to; ++ cit)
					TRY(igraph_vector_push_back(&_, *cit));
			} else
				mm_dont_dealloc = true;
		}
		
		/// Calculates the running mean of a vector.
		RETRIEVE_TEMPORARY_CLASS(Vector) running_mean(const Integer binwidth) const MAY_THROW_EXCEPTION;
		
		/// Generates an increasing random sequence of integers
		static RETRIEVE_TEMPORARY_CLASS(Vector) random_sample(const Integer low, const Integer high, const Integer vector_length) MAY_THROW_EXCEPTION;
		
		
		friend class VertexSelector;
		friend class VertexIterator;
		friend class EdgeSelector;
		friend class EdgeIterator;
		friend class Graph;
		friend class GraphWriter;
		friend class GraphReader;
	};
	MEMORY_MANAGER_INTERFACE_EX(Vector);

	typedef Vector VertexVector;
	typedef Vector EdgeVector;
}
										   
#endif