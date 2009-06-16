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
	
	XXINTRNL_PREPARE_UNDERLYING_TYPES(BasicVector, vector);	
	/**
	 \class Vector
	 \brief Wrapper for igraph vectors
	 
	 This template class is a wrapper of the igraph vectors type.
	 In igraph there are four kinds of vectors: Vectors of Reals, longs, chars and Booleans.
	 */
	template <typename T>
	class BasicVector {
		XXINTRNL_UNDERLYING_TYPE(BasicVector) _;
		
	public:
		MEMORY_MANAGER_INTERFACE_WITH_TEMPLATE(BasicVector, <T>);
		XXINTRNL_WRAPPER_CONSTRUCTOR_INTERFACE(BasicVector, XXINTRNL_UNDERLYING_TYPE(BasicVector));
		
		/// Copy a C array into a Vector.
		BasicVector(T* const array, const long count) MAY_THROW_EXCEPTION;
		
		/// Create a vector with "count" elements.
		BasicVector(const long count) MAY_THROW_EXCEPTION;
		
#if XXINTRNL_CXX0X
		/// Create a Vector using initializer list (C++0x only.)
		BasicVector(::std::initializer_list<T> elements) MAY_THROW_EXCEPTION;
#endif
		
		/**
		 \brief Create a Vector using content of string.
		 
		 Example:
		 \code
		 Vector v = Vector("42 54 64 75");
		 printf("%lg", v.prod());	// prints 10886400
		 \endcode
		 */
		BasicVector(const char* stringized_elements) MAY_THROW_EXCEPTION;
		
		/// Create a vector sequentially between two values inclusively.
		static RETRIEVE_TEMPORARY_CLASS_WITH_TEMPLATE(BasicVector<T>) seq(const T from, const T to) MAY_THROW_EXCEPTION;
		
		/// Create a vector with "count" elements filled with zero.
		__attribute__((deprecated,warning("BasicVector<T>::zeros is deprecated. Use the constructor BasicVector(x) (for x > 0) or BasicVector<T>::n (for x = 0) instead.")))
		static RETRIEVE_TEMPORARY_CLASS_WITH_TEMPLATE(BasicVector<T>) zeros(const long count = 0) MAY_THROW_EXCEPTION;
		
		/// Create a vector with "count" elements filled with zero.
		static RETRIEVE_TEMPORARY_CLASS_WITH_TEMPLATE(BasicVector<T>) n() MAY_THROW_EXCEPTION;
		
		/// Wrap a C array as a Vector. 
		static RETRIEVE_TEMPORARY_CLASS_WITH_TEMPLATE(BasicVector<T>) view(const T* const array, const long count) throw();
		
		BasicVector<T>& null() throw();
		BasicVector<T>& fill(const T e) throw();
		
		T* ptr() throw() { return VECTOR(_); }
		const T& operator[](const long index) const throw() { return VECTOR(_)[index]; }
		T& operator[](const long index) throw() { return VECTOR(_)[index]; }
		
		T e(const long index) const throw();
		T* e_ptr(const long index) const throw();
		BasicVector<T>& set(const long index, const T value) throw();
		T tail() const throw();
		
		void copy_to(T* store) const throw();
		BasicVector<T>& update(const BasicVector<T>& update_from) MAY_THROW_EXCEPTION;
		BasicVector<T>& append(const BasicVector<T>& append_from) MAY_THROW_EXCEPTION;
		void swap(BasicVector<T>& swap_with) MAY_THROW_EXCEPTION;
		
		BasicVector<T>& swap_elements(const long i, const long j) MAY_THROW_EXCEPTION;
		BasicVector<T>& reverse() MAY_THROW_EXCEPTION;
		
		BasicVector<T>& operator+= (const T k) throw();
		BasicVector<T>& operator-= (const T k) throw();
		BasicVector<T>& operator*= (const T k) throw();
		BasicVector<T>& operator/= (const T k) throw();
		
		BasicVector<T>& operator+= (const BasicVector<T>& k) MAY_THROW_EXCEPTION;
		BasicVector<T>& operator-= (const BasicVector<T>& k) MAY_THROW_EXCEPTION;
		BasicVector<T>& operator*= (const BasicVector<T>& k) MAY_THROW_EXCEPTION;
		BasicVector<T>& operator/= (const BasicVector<T>& k) MAY_THROW_EXCEPTION;
		
		T min() const throw();
		T max() const throw();
		long which_min() const throw();
		long which_max() const throw();
		void minmax(T& minStore, T& maxStore) const MAY_THROW_EXCEPTION;
		void which_minmax(long& minStore, long& maxStore) const MAY_THROW_EXCEPTION;
		
		bool empty() const throw();
		long size() const throw();
		
		T sum() const throw();
		T prod() const throw();
		bool isnull() const throw();
		bool isininterval(T low, T high) const throw();
		bool any_smaller(T upper_limit) const throw();
		bool operator== (const BasicVector<T>& other) const throw();
		bool operator!= (const BasicVector<T>& other) const throw();
		T maxdifference(const BasicVector<T>& other) const throw();
		
		bool contains(const T e) const throw();
		bool search(const T what, const long from = 0) const throw();
		bool search(const T what, const long from, long& pos) const throw();
		bool binsearch(const T what) const throw();
		bool binsearch(const T what, long& pos) const throw();
		
		BasicVector<T>& clear() throw();
		BasicVector<T>& reserve(const long new_size) MAY_THROW_EXCEPTION;
		BasicVector<T>& resize(const long new_size) MAY_THROW_EXCEPTION;
		BasicVector<T>& push_back(const T e) MAY_THROW_EXCEPTION;
		T pop_back() throw();
		BasicVector<T>& insert(const long pos, const T e) MAY_THROW_EXCEPTION;
		BasicVector<T>& remove(const long pos) throw();
		BasicVector<T>& remove_section(const long from, const long to) throw();
		
		BasicVector<T>& sort() throw();
				
		// STL support
		typedef T value_type;
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
				if (is_first)
					is_first = false;
				else
					::std::fprintf(f, " ");
				XXINTRNL_fprintf(f, *cit);
			}
			::std::fprintf(f, "\n");
		}
		
		/// Convert from std::vector.
		template<typename InputIterator>
		BasicVector(const InputIterator from, const InputIterator to) MAY_THROW_EXCEPTION;
		
		friend class VertexSelector;
		friend class VertexIterator;
		friend class EdgeSelector;
		friend class EdgeIterator;
		friend class Graph;
		friend class GraphWriter;
		friend class GraphReader;
	};
	MEMORY_MANAGER_INTERFACE_EX_WITH_TEMPLATE(template<typename T>, BasicVector, <T>);
	
	typedef BasicVector<Real> Vector;
	typedef BasicVector<Boolean> BoolVector;
	typedef Vector VertexVector;
	typedef Vector EdgeVector;
}
										   
#endif