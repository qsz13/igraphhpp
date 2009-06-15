/*

referencevector.cpp ... Wrapper implementation of igraph reference vector.

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

#ifndef IGRAPH_REFERENCEVECTOR_CPP
#define IGRAPH_REFERENCEVECTOR_CPP

#include <igraph/cpp/referencevector.hpp>
#include <cstdarg>
#include <cstring>

namespace igraph {
	template<typename T>
	ReferenceVector<T>::iterator::iterator(const const_iterator& other) throw() : m_ptr(other.m_ptr) {}
	
	MEMORY_MANAGER_IMPLEMENTATION_WITH_TEMPLATE(template<typename T>, ReferenceVector, <T>);
	
	template<typename T>
	XXINTRNL_WRAPPER_CONSTRUCTOR_IMPLEMENTATION(ReferenceVector, igraph_vector_ptr_t, igraph_vector_ptr_copy, <T>);
	
	template<typename T>
	IMPLEMENT_COPY_METHOD_WITH_TEMPLATE(ReferenceVector, <T>) {
		manage_children_by_new_and_delete = other.manage_children_by_new_and_delete;
		if (manage_children_by_new_and_delete) {
			TRY(igraph_vector_ptr_init(&_, igraph_vector_ptr_size(&other._)));
			const T* const* cit = reinterpret_cast<const T* const*>(other._.stor_begin);
			T** it = reinterpret_cast<T**>(_.stor_begin);
			for (; cit != reinterpret_cast<const T* const*>(other._.end); ++ cit, ++ it)
				*it = new T(**cit);
		} else
			TRY(igraph_vector_ptr_copy(&_, &other._));
	}
	
	template<typename T>
	IMPLEMENT_MOVE_METHOD_WITH_TEMPLATE(ReferenceVector, <T>) {
		manage_children_by_new_and_delete = ::std::move(other.manage_children_by_new_and_delete);
		other.manage_children_by_new_and_delete = false;
		_ = ::std::move(other._);
	}
	
	template<typename T>
	IMPLEMENT_DEALLOC_METHOD_WITH_TEMPLATE(ReferenceVector, <T>) {
		if (manage_children_by_new_and_delete) {
			delete_all();
			igraph_vector_ptr_null(&_);
		}
		igraph_vector_ptr_destroy(&_);
	}
	
	template<typename T>
	ReferenceVector<T>::ReferenceVector(const long count) MAY_THROW_EXCEPTION : manage_children_by_new_and_delete(false) {
		TRY(igraph_vector_ptr_init(&_, count));
	}
	
	template<typename T>
	ReferenceVector<T>::ReferenceVector(pointer* ptr_array, const long count) MAY_THROW_EXCEPTION : manage_children_by_new_and_delete(false) {
		XXINTRNL_DEBUG_CALL_INITIALIZER(ReferenceVector, <T>);
		TRY(igraph_vector_ptr_init_copy(&_, reinterpret_cast<void**>(ptr_array), count));
	}
	
	template<typename T>
	ReferenceVector<T>::ReferenceVector(const_pointer array, const long count) MAY_THROW_EXCEPTION : manage_children_by_new_and_delete(true) {
		XXINTRNL_DEBUG_CALL_INITIALIZER(ReferenceVector, <T>);
		TRY(igraph_vector_ptr_init(&_, count));
		for (long i = 0; i < count; ++ i)
			VECTOR(_)[i] = new T(array[i]);
	}
	
	template<typename T>
	ReferenceVector<T>::ReferenceVector(const long count, ...) MAY_THROW_EXCEPTION : manage_children_by_new_and_delete(false) {
		XXINTRNL_DEBUG_CALL_INITIALIZER(ReferenceVector, <T>);
#if __GNUC__ >= 3
		void* arr[count];
#else
		void** arr = new void*[count];
#endif
		std::va_list va;
		va_start(va, count);
		for (long i = 0; i < count; ++ i)
			arr[i] = va_arg(va, pointer);
		va_end(va);
		
		TRY(igraph_vector_ptr_init_copy(&_, arr, count));
		
#if !(__GNUC__ >= 3)
		// FIXME: May cause memory leak when an exception is thrown.
		delete[] arr;
#endif
	}
	
	template<typename T>
	typename RETRIEVE_TEMPORARY_CLASS(ReferenceVector<T> ) view(typename ReferenceVector<T>::pointer* array, const long count) throw() {
		igraph_vector_ptr_t _;
		igraph_vector_ptr_view(&_, array, count);
		return ::std::move(ReferenceVector<T>(_, ::tempobj::OwnershipTransferNoOwnership));
	}
	
	template<typename T>
	ReferenceVector<T>& ReferenceVector<T>::null() throw() {
		if (manage_children_by_new_and_delete)
			delete_all();
		igraph_vector_ptr_null(&_);
		return *this;
	}
	
	template<typename T> typename ReferenceVector<T>::pointer ReferenceVector<T>::e(const long index) const throw() { return igraph_vector_ptr_e(&_, index); }
	template<typename T> ReferenceVector<T>& ReferenceVector<T>::set(const long index, const_pointer value, const ::tempobj::OwnershipTransfer transfer) throw() {
		if (transfer == ::tempobj::OwnershipTransferCopy && manage_children_by_new_and_delete)
			value = new T(*value);
		igraph_vector_ptr_set(&_, index, value);
		return *this;
	}
	
	template<typename T> bool ReferenceVector<T>::empty() const throw() { return igraph_vector_ptr_empty(&_); }
	template<typename T> long ReferenceVector<T>::size() const throw() { return igraph_vector_ptr_size(&_); }
	
	template<typename T> ReferenceVector<T>& ReferenceVector<T>::clear() throw() {
		if (manage_children_by_new_and_delete)
			delete_all();
		igraph_vector_ptr_clear(&_);
		return *this;
	}
	template<typename T> ReferenceVector<T>& ReferenceVector<T>::reserve(const long new_size) MAY_THROW_EXCEPTION { TRY(igraph_vector_ptr_reserve(&_, new_size)); return *this; }
	template<typename T> ReferenceVector<T>& ReferenceVector<T>::resize(const long new_size) MAY_THROW_EXCEPTION { 
		long old_size;
		if (manage_children_by_new_and_delete) {
			old_size = _.end - _.stor_begin;
			if (new_size < old_size)
				for (void** it = _.stor_begin + new_size; it != _.end; ++ it)
					delete *it;
		}
		TRY(igraph_vector_ptr_resize(&_, new_size));
		if (manage_children_by_new_and_delete)
			if (new_size > old_size)
				::std::memset(_.stor_begin + old_size, 0, (new_size - old_size) * sizeof(void*));
		return *this;
	}
	template<typename T> ReferenceVector<T>& ReferenceVector<T>::push_back(pointer e, const ::tempobj::OwnershipTransfer transfer) MAY_THROW_EXCEPTION {
		if (manage_children_by_new_and_delete && transfer == ::tempobj::OwnershipTransferCopy)
			e = new T(*e);
		TRY(igraph_vector_ptr_push_back(&_, e));
		return *this;
	}
	template<typename T> ReferenceVector<T>& ReferenceVector<T>::insert(const long pos, pointer e, const ::tempobj::OwnershipTransfer transfer) MAY_THROW_EXCEPTION {
		if (manage_children_by_new_and_delete && transfer == ::tempobj::OwnershipTransferCopy)
			e = new T(*e);
		TRY(igraph_vector_ptr_insert(&_, pos, e));
		return *this;
	}
	template<typename T> ReferenceVector<T>& ReferenceVector<T>::remove(const long pos) throw() {
		if (manage_children_by_new_and_delete)
			delete VECTOR(_)[pos];
		igraph_vector_ptr_remove(&_, pos);
		return *this;
	}
	
	template<typename T> void ReferenceVector<T>::copy_to(pointer* store, const ::tempobj::OwnershipTransfer transfer) const throw() {
		if (manage_children_by_new_and_delete) {
			if (transfer == ::tempobj::OwnershipTransferCopy) {
				for (const T* const* it = _.stor_begin; it != _.end; ++it, ++store)
					*store = new T(**it);
				return;
			} else if (transfer == ::tempobj::OwnershipTransferMove)
				manage_children_by_new_and_delete = false;
		}
		igraph_vector_ptr_copy_to(&_, store);
	}
	template<typename T> ReferenceVector<T>& ReferenceVector<T>::sort(int(*compar)(const_reference, const_reference)) { igraph_vector_ptr_sort(&_, compar); return *this; }
	
	template<typename T>
	ReferenceVector<T>& ReferenceVector<T>::perform(void(*fptr)(pointer obj, void* context), void* context) {
		for (T* const* cit = _.stor_begin; cit != _.end; ++ cit)
			fptr(*cit, context);
		return *this;
	}
	
	template<typename T>
	bool ReferenceVector<T>::operator==(const ReferenceVector<T>& other) const throw() {
		long my_size = size();
		if (my_size != other.size())
			return false;
		if (manage_children_by_new_and_delete) {
			const T* const* cit = reinterpret_cast<const T* const*>(_.stor_begin);
			const T* const* cit2 = reinterpret_cast<const T* const*>(other._.stor_begin);
			for (; cit != reinterpret_cast<const T* const*>(_.end); ++ cit, ++ cit2)
				if (**cit != **cit2)
					return false;
			return true;
		} else
			return ::std::memcmp(other._.stor_begin, _.stor_begin, my_size * sizeof(void*)) == 0;
	}
	
	template<typename T>
	bool ReferenceVector<T>::operator!=(const ReferenceVector<T>& other) const throw() { return !(*this == other); }
	
}

#endif