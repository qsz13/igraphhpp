/*
 
 referencevector.hpp ... Wrapper for igraph reference vectors 
 
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
 \file referencervector.hpp
 \brief Wrapper for igraph reference vectors
 \author KennyTM~
 \date June 12th, 2009
 */

#ifndef IGRAPH_REFERENCEVECTOR_HPP
#define IGRAPH_REFERENCEVECTOR_HPP

#include <igraph/igraph.h>
#include <igraph/cpp/common.hpp>
#include <igraph/cpp/exception.hpp>

namespace igraph {
	
	/**
	 \class ReferenceVector
	 \brief Wrapper for igraph pointer vectors
	 
	 This class is a wrapper of the igraph_vector_ptr_t type, which is an array of void*.
	 */
	template<typename T>
	class ReferenceVector {
	private:
		igraph_vector_ptr_t _;
		int delete_all;
		
	public:
		typedef T& value_type;

		MEMORY_MANAGER_INTERFACE_WITH_TEMPLATE(ReferenceVector, <T>);
		XXINTRNL_WRAPPER_CONSTRUCTOR_INTERFACE(ReferenceVector, igraph_vector_ptr_t);
	
		/// Create a ReferenceVector with "count" elements.
		ReferenceVector(const long count) MAY_THROW_EXCEPTION;
		
		/// Copy a C array into a ReferenceVector.
		ReferenceVector(value_type* const array, const long count) MAY_THROW_EXCEPTION;
		
		/// var_arg method to construct the ReferenceVector
		ReferenceVector(const long count, ...) MAY_THROW_EXCEPTION;
		
		/// Wrap a C array as a ReferenceVector.
		static ReferenceVector<T> view(value_type const* const array, const long count) throw() {
			igraph_vector_ptr_t _;
			igraph_vector_ptr_view(&_, array, count);
			return ReferenceVector<T>(_, ::tempobj::OwnershipTransferNoOwnership);
		}
		
		void null() throw();
		
		value_type* ptr() throw() { return VECTOR(_); }
		value_type const& operator[](const long index) const throw() { return VECTOR(_)[index]; }
		value_type& operator[](const long index) throw() { return VECTOR(_)[index]; }
		
		value_type& e(const long index) const throw();
		void set(const long index, value_type const value) throw();
		
		bool empty() const throw();
		long size() const throw();
		
		void clear() throw();
		void reserve(const long new_size) MAY_THROW_EXCEPTION;
		void resize(const long new_size) MAY_THROW_EXCEPTION;
		void push_back(value_type const e) MAY_THROW_EXCEPTION;
		void insert(const long pos, value_type const e) MAY_THROW_EXCEPTION;
		void remove(const long pos) throw();
		
		void copy_to(value_type* store) const throw();
		void sort(int(*compar)(const value_type, const value_type));
		
		// STL support.
		typedef value_type* pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef unsigned size_type;
		typedef int difference_type;
		typedef value_type* iterator;
		typedef const value_type* const_iterator;
		
		iterator begin() throw() { return _.stor_begin; }
		iterator end() throw() { return _.end; }
		const_iterator begin() const throw() { return reinterpret_cast<const_iterator>(_.stor_begin); }
		const_iterator end() const throw() { return reinterpret_cast<const_iterator>(_.end); }
		size_type capacity() const throw() { return _.stor_end - _.stor_begin; }
		reference front() throw() { return *_.stor_begin; }
		const_reference front() const throw() { *_.stor_begin; }
		reference back() throw() { return *(_.end-1); }
		const_reference back() const throw() { return *(_.end-1); }
		
		/// Print content of the ReferenceVector.
		void print() const throw() {
			for (const_iterator cit = begin(); cit != end(); ++ cit)
				printf("%p, ", *cit);
			printf("\n");
		}
		
		/// Perform the function on every object of the ReferenceVector.
		void perform(void(*fptr)(value_type& obj, void* context), void* context = NULL);
		
		bool operator==(const ReferenceVector<T>& other) const throw();
		bool operator!=(const ReferenceVector<T>& other) const throw();
		
	};
	
	template<typename T>
	MEMORY_MANAGER_INTERFACE_EX_WITH_TEMPLATE(ReferenceVector, <T>);
}

#endif
