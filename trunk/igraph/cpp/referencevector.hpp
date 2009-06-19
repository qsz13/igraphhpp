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
#include <iterator>

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
		bool manage_children_by_new_and_delete;
		
		void delete_all () throw() {
			for (T** it = reinterpret_cast<T**>(_.stor_begin); it != reinterpret_cast<T**>(_.end); ++ it)
				delete *it;
		}
		
	public:
		MEMORY_MANAGER_INTERFACE_WITH_TEMPLATE(ReferenceVector, <T>);
		XXINTRNL_WRAPPER_CONSTRUCTOR_INTERFACE(ReferenceVector, igraph_vector_ptr_t);
		
		typedef T value_type;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef unsigned size_type;
		typedef int difference_type;
		
		class const_iterator;
		
		class iterator {
			T** m_ptr;
			iterator(){}
			static iterator convert(void** ptr_) { iterator x; x.m_ptr = reinterpret_cast<T**>(ptr_); return x; }
			friend class ReferenceVector<T>;
		public:
			// To support iterator_traits
			typedef int difference_type;
			typedef T value_type;
			typedef T* pointer;
			typedef T& reference;
			typedef ::std::random_access_iterator_tag iterator_category;			
			
			iterator(const const_iterator& other) throw();
			reference operator*() const throw() { return **m_ptr; }
			pointer ptr() const throw() { return *m_ptr; }
			pointer operator->() const throw() { return *m_ptr; }
			iterator& operator++() throw() { ++m_ptr; return *this; }
			iterator operator++(int) throw() { iterator copy(ptr); ++m_ptr; return copy; }
			iterator& operator--() throw() { --m_ptr; return *this; }
			iterator operator--(int) throw() { iterator copy(ptr); --m_ptr; return copy; }
			iterator& operator+=(const difference_type delta) throw() { m_ptr += delta; return *this; }
			iterator& operator-=(const difference_type delta) throw() { m_ptr -= delta; return *this; }
			iterator operator+(const difference_type delta) const throw() { return iterator(m_ptr+delta); }
			iterator operator-(const difference_type delta) const throw() { return iterator(m_ptr-delta); }
			difference_type operator-(const iterator& other) const throw() { return m_ptr - other.m_ptr; }
			reference operator[](const difference_type delta) throw() { return **(m_ptr + delta); }
			bool operator<(const iterator& other) const throw() { return m_ptr < other.m_ptr; }
			bool operator>(const iterator& other) const throw() { return m_ptr > other.m_ptr; }
			bool operator<=(const iterator& other) const throw() { return m_ptr <= other.m_ptr; }
			bool operator>=(const iterator& other) const throw() { return m_ptr >= other.m_ptr; }
			bool operator==(const iterator& other) const throw() { return m_ptr == other.m_ptr; }
			bool operator!=(const iterator& other) const throw() { return m_ptr != other.m_ptr; }
		};
		
		class const_iterator {
			const T* const* m_ptr;
			const_iterator(){}
			static const_iterator convert(void* const* ptr_) { const_iterator x; x.m_ptr = reinterpret_cast<const T* const*>(ptr_); return x; }
			friend class ReferenceVector<T>;
		public:
			// To support iterator_traits
			typedef int difference_type;
			typedef T value_type;
			typedef const T* pointer;
			typedef const T& reference;
			typedef ::std::random_access_iterator_tag iterator_category;			
			
			const_iterator(const iterator& other) throw() : m_ptr(other.m_ptr) {};
			const_iterator(T* const* const ptr_) throw() : m_ptr(ptr_) {}
			reference operator*() const throw() { return **m_ptr; }
			pointer ptr() const throw() { return *m_ptr; }
			pointer operator->() const throw() { return *m_ptr; }
			const_iterator& operator++() throw() { ++m_ptr; return *this; }
			const_iterator operator++(int) throw() { const_iterator copy(m_ptr); ++m_ptr; return copy; }
			const_iterator& operator--() throw() { --m_ptr; return *this; }
			const_iterator operator--(int) throw() { const_iterator copy(m_ptr); --m_ptr; return copy; }
			const_iterator& operator+=(const difference_type delta) throw() { m_ptr += delta; return *this; }
			const_iterator& operator-=(const difference_type delta) throw() { m_ptr -= delta; return *this; }
			const_iterator operator+(const difference_type delta) const throw() { return const_iterator(m_ptr+delta); }
			const_iterator operator-(const difference_type delta) const throw() { return const_iterator(m_ptr-delta); }
			difference_type operator-(const const_iterator& other) const throw() { return m_ptr - other.m_ptr; }
			reference operator[](const difference_type delta) throw() { return **(m_ptr + delta); }
			bool operator<(const const_iterator& other) const throw() { return m_ptr < other.m_ptr; }
			bool operator>(const const_iterator& other) const throw() { return m_ptr > other.m_ptr; }
			bool operator<=(const const_iterator& other) const throw() { return m_ptr <= other.m_ptr; }
			bool operator>=(const const_iterator& other) const throw() { return m_ptr >= other.m_ptr; }
			bool operator==(const const_iterator& other) const throw() { return m_ptr == other.m_ptr; }
			bool operator!=(const const_iterator& other) const throw() { return m_ptr != other.m_ptr; }
		};
				
		/// Create a ReferenceVector with "count" elements.
		ReferenceVector(const long count) MAY_THROW_EXCEPTION;
		
		/** \brief Copy a C array into a ReferenceVector.
		 
		 Example:
		 \code
		 
		 std::string strings[3];
		 strings[0] = "foo";
		 strings[1] = "bar";
		 strings[2] = "baz";
		 
		 ReferenceVector<std::string> vec (strings, 3);
		 \endcode
		 */
		ReferenceVector(const_pointer array, const long count) MAY_THROW_EXCEPTION;
		
		/// Move a C pointer array into a ReferenceVector.
		ReferenceVector(pointer* ptr_array, const long count) MAY_THROW_EXCEPTION;
		
#if XXINTRNL_CXX0X
		/// Copy elements from an initializer list to ReferenceVector (C++0x only.)
		ReferenceVector(::std::initializer_list<const_reference> elements) MAY_THROW_EXCEPTION;
#endif
			
		/// Wrap a C pointer array as a ReferenceVector.
		static RETRIEVE_TEMPORARY_CLASS_WITH_TEMPLATE(ReferenceVector<T>) view(pointer* array, const long count) throw();
		
		/// Move an igraph_vector_ptr_t into a ReferenceVector, and also convert its content into the corresponding C++ types.
		template <typename OriginalType>
		static RETRIEVE_TEMPORARY_CLASS_WITH_TEMPLATE(ReferenceVector<T>) adopt(igraph_vector_ptr_t& raw);
		 
		ReferenceVector<T>& null() throw();
		
		pointer* ptr() throw() { return VECTOR(_); }
		const_reference operator[](const long index) const throw() { return **reinterpret_cast<const const_pointer*>(VECTOR(_)[index]); }
		reference operator[](const long index) throw() { return **reinterpret_cast<pointer*>(VECTOR(_)[index]); }
		
		pointer e(const long index) const throw();
		/// Note: DO NOT supply OwnershipTransferKeepOriginal in the last argument.
		ReferenceVector<T>& set(const long index, const_pointer value, const ::tempobj::OwnershipTransfer transfer = ::tempobj::OwnershipTransferMove) throw();
		
		bool empty() const throw();
		long size() const throw();
		
		ReferenceVector<T>& clear() throw();
		ReferenceVector<T>& reserve(const long new_size) MAY_THROW_EXCEPTION;
		ReferenceVector<T>& resize(const long new_size) MAY_THROW_EXCEPTION;
		ReferenceVector<T>& push_back(const_reference obj) MAY_THROW_EXCEPTION { return push_back(new T(obj)); }
		ReferenceVector<T>& push_back(pointer e, const ::tempobj::OwnershipTransfer transfer = ::tempobj::OwnershipTransferMove) MAY_THROW_EXCEPTION;
		ReferenceVector<T>& insert(const long pos, const_reference obj, const ::tempobj::OwnershipTransfer transfer = ::tempobj::OwnershipTransferCopy) MAY_THROW_EXCEPTION { return insert(pos, &obj, transfer); }
		ReferenceVector<T>& insert(const long pos, pointer e, const ::tempobj::OwnershipTransfer transfer = ::tempobj::OwnershipTransferMove) MAY_THROW_EXCEPTION;
		ReferenceVector<T>& remove(const long pos) throw();
		
		void copy_to(pointer* store, const ::tempobj::OwnershipTransfer transfer = ::tempobj::OwnershipTransferCopy) const throw();
		ReferenceVector<T>& sort(int(*compar)(const_reference, const_reference));
		
		// STL support.
		iterator begin() throw() { return iterator::convert(_.stor_begin); }
		iterator end() throw() { return iterator::convert(_.end); }
		const_iterator begin() const throw() { return const_iterator::convert(_.stor_begin); }
		const_iterator end() const throw() { return const_iterator::convert(_.end); }
		size_type capacity() const throw() { return _.stor_end - _.stor_begin; }
		reference front() throw() { return **reinterpret_cast<pointer*>(_.stor_begin); }
		const_reference front() const throw() { **reinterpret_cast<const const_pointer*>(_.stor_begin); }
		reference back() throw() { return **reinterpret_cast<pointer*>(_.end-1); }
		const_reference back() const throw() { return **reinterpret_cast<const const_pointer*>(_.end-1); }
				
		/// Perform the function on every object of the ReferenceVector.
		ReferenceVector<T>& perform(void(*fptr)(pointer obj, void* context), void* context = NULL);
		
		bool operator==(const ReferenceVector<T>& other) const throw();
		bool operator!=(const ReferenceVector<T>& other) const throw();
		
		friend class Graph;
	};
	
	MEMORY_MANAGER_INTERFACE_EX_WITH_TEMPLATE(template<typename T>, ReferenceVector, <T>);
}

#include <igraph/cpp/impl/referencevector.cpp>

#endif
