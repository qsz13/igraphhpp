/*
 
 pointervector.hpp ... Wrapper for igraph pointer vectors 
 
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
 \file pointervector.hpp
 \brief Wrapper for igraph pointer vectors
 \author KennyTM~
 \date June 12th, 2009
 */

#ifndef IGRAPH_POINTERVECTOR_HPP
#define IGRAPH_POINTERVECTOR_HPP

#include <igraph/igraph.h>
#include <igraph/cpp/common.hpp>
#include <igraph/cpp/tempobj.hpp>
#include <igraph/cpp/exception.hpp>

namespace igraph {
	
	/**
	 \class PointerVector
	 \brief Wrapper for igraph pointer vectors
	 
	 This class is a wrapper of the igraph_vector_ptr_t type, which is an array of void*.
	 */
	template<typename T>
	class PointerVector {
	private:
		igraph_vector_ptr_t _;
		
		PointerVector() throw() {}
		
	public:
		MEMORY_MANAGER_INTERFACE_WITH_TEMPLATE(PointerVector, <T>);
	
		/// Create a PointerVector with "count" elements.
		PointerVector(const long count) throw(Exception) {
			TRY(igraph_vector_ptr_init(&_, count));
		}
		
		/// Copy a C array into a PointerVector.
		PointerVector(T** const array, const long count) throw(Exception) {
			TRY(igraph_vector_ptr_init_copy(&_, reinterpret_cast<void**>(array), count));
		}
		
		/// Wrap a C array as a PointerVector.
		static PointerVector<T> view(void* const* const array, const long count) throw() {
			PointerVector<T> v;
			v.mm_dont_dealloc = true;
			igraph_vector_ptr_view(&v._, array, count);
			return v;
		}		
		
		/// Wrap or copy an igraph_vector_ptr_t to a PointerVector.
		PointerVector(const igraph_vector_ptr_t& orig, const OwnershipTransfer transfer = OwnershipTransferKeepOriginal) : COMMON_INIT_WITH(transfer) {
			if (transfer == OwnershipTransferCopy) {
				TRY(igraph_vector_ptr_copy(&_, &orig));
			} else
				_ = orig;
		}
		
		/// var_arg method to construct the PointerVector
		PointerVector(const long count, ...) throw(Exception) {
#if __GNUC__ >= 3
			void* arr[count];
#else
			void** arr = new void*[count];
#endif
			std::va_list va;
			va_start(va, count);
			for (long i = 0; i < count; ++ i)
				arr[i] = va_arg(va, T*);
			va_end(va);
			
			TRY(igraph_vector_ptr_init_copy(&_, arr, count));
			
#if !(__GNUC__ >= 3)
			// FIXME: May cause memory leak when an exception is thrown.
			delete[] arr;
#endif
		}
		
		void null() throw() { igraph_vector_ptr_null(&_); }
		T** ptr() throw() { return VECTOR(_); }
		T* const& operator[](const long index) const throw() { return VECTOR(_)[index]; }
		T*& operator[](const long index) throw() { return VECTOR(_)[index]; }
		
		T* e(const long index) const throw() { return igraph_vector_ptr_e(&_, index); }
		void set(const long index, T* const value) throw() { igraph_vector_ptr_set(&_, index, value); }
		
		bool empty() const throw() { return igraph_vector_ptr_empty(&_); }
		long size() const throw() { return igraph_vector_ptr_size(&_); }
		
		void clear() throw() { igraph_vector_ptr_clear(&_); }
		void reserve(const long new_size) throw(Exception) { TRY(igraph_vector_ptr_reserve(&_, new_size)); }
		void resize(const long new_size) throw(Exception) { TRY(igraph_vector_ptr_resize(&_, new_size)); }
		void push_back(T* const e) throw(Exception) { TRY(igraph_vector_ptr_push_back(&_, e)); }
		void insert(const long pos, T* const e) throw(Exception) { TRY(igraph_vector_ptr_insert(&_, pos, e)); }
		void remove(const long pos) throw() { igraph_vector_ptr_remove(&_, pos); }
		
		void copy_to(T** store) const throw() { igraph_vector_ptr_copy_to(&_, store); }
		void sort(int(*compar)(const T*, const T*)) { igraph_vector_ptr_sort(&_, compar); }
		
		// STL support.
		typedef T* value_type;
		typedef value_type* pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef unsigned size_type;
		typedef int difference_type;
		typedef value_type* iterator;
		typedef const value_type* const_iterator;
		
		iterator begin() { return _.stor_begin; }
		iterator end() { return _.end; }
		const_iterator begin() const { return reinterpret_cast<const_iterator>(_.stor_begin); }
		const_iterator end() const { return reinterpret_cast<const_iterator>(_.end); }
		size_type capacity() const { return _.stor_end - _.stor_begin; }
		reference front() { return *_.stor_begin; }
		const_reference front() const { *_.stor_begin; }
		reference back() { return *(_.end-1); }
		const_reference back() const { return *(_.end-1); }
		
		/// Print content of the PointerVector.
		void print() const throw() {
			for (const_iterator cit = begin(); cit != end(); ++ cit)
				printf("%p, ", *cit);
			printf("\n");
		}
		
		/// Perform the function on every object of the PointerVector.
		void perform(void(*fptr)(T*& obj, void* context), void* context = NULL) {
			for (iterator cit = begin(); cit != end(); ++ cit)
				fptr(*cit, context);
		}
		
		bool operator==(const PointerVector<T>& other) const {
			if (size() != other.size())
				return false;
			for (const_iterator cit = begin(), cit2 = other.begin(); cit != end(); ++ cit, ++ cit2)
				if (*cit != *cit2)
					return false;
			return true;
		}
		bool operator!=(const PointerVector<T>& other) const { return !(*this == other); }
		
	};
	
	MEMORY_MANAGER_IMPLEMENTATION_ATTR_WITH_TEMPLATE(template<typename T>, inline, PointerVector, <T>);
	
	template<typename T>
	inline IMPLEMENT_COPY_METHOD_WITH_TEMPLATE(PointerVector, <T>) {
		igraph_vector_ptr_copy(&_, &other._);
	}
	
	template<typename T>
	inline IMPLEMENT_MOVE_METHOD_WITH_TEMPLATE(PointerVector, <T>) {
		_ = std::move(other._);
	}
	
	template<typename T>
	inline IMPLEMENT_DEALLOC_METHOD_WITH_TEMPLATE(PointerVector, <T>) {
		igraph_vector_ptr_destroy(&_);
	}
				   
}

#endif