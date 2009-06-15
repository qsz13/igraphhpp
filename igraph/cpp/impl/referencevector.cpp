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

namespace igraph {
	MEMORY_MANAGER_IMPLEMENTATION_WITH_TEMPLATE(template<typename T>, ReferenceVector, <T>);
	
	template<typename T>
	XXINTRNL_WRAPPER_CONSTRUCTOR_IMPLEMENTATION(ReferenceVector, igraph_vector_ptr_t, igraph_vector_ptr_copy, <T>);
	
	template<typename T>
	IMPLEMENT_COPY_METHOD_WITH_TEMPLATE(ReferenceVector, <T>) MAY_THROW_EXCEPTION {
		delete_all = other.delete_all;
		TRY(igraph_vector_ptr_copy(&_, &other._));
	}
	
	template<typename T>
	IMPLEMENT_MOVE_METHOD_WITH_TEMPLATE(ReferenceVector, <T>) throw() {
		delete_all = ::std::move(other.delete_all);
		other.delete_all = 0;
		_ = ::std::move(other._);
	}
	
	template<typename T>
	IMPLEMENT_DEALLOC_METHOD_WITH_TEMPLATE(ReferenceVector, <T>) {
		switch (delete_all) {
			case 1:
				for (iterator it = begin(); it != end(); ++ it) {
					delete &*it;
					*reinterpret_cast<pointer>(it) = NULL;
				}
				break;
			case 2:
				for (iterator it = begin(); it != end(); ++ it) {
					delete[] &*it;
					*reinterpret_cast<pointer>(it) = NULL;
				}
				break;
			default:
				break;
		}
		igraph_vector_ptr_destroy(&_);
	}
	
	template<typename T>
	ReferenceVector<T>::ReferenceVector(const long count) MAY_THROW_EXCEPTION {
		TRY(igraph_vector_ptr_init(&_, count));
	}
	
	template<typename T>
	ReferenceVector<T>::ReferenceVector(value_type* const array, const long count) MAY_THROW_EXCEPTION {
		XXINTRNL_DEBUG_CALL_INITIALIZER(ReferenceVector, <T>);
		TRY(igraph_vector_ptr_init_copy(&_, reinterpret_cast<void**>(array), count));
	}
	
	template<typename T>
	ReferenceVector<T>::ReferenceVector(const long count, ...) MAY_THROW_EXCEPTION {
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
	void ReferenceVector<T>::null() throw() { igraph_vector_ptr_null(&_); }
	
	template<typename T> value_type ReferenceVector<T>::e(const long index) const throw() { return igraph_vector_ptr_e(&_, index); }
	template<typename T> void ReferenceVector<T>::set(const long index, value_type const value) throw() { igraph_vector_ptr_set(&_, index, value); }
	
	template<typename T> bool ReferenceVector<T>::empty() const throw() { return igraph_vector_ptr_empty(&_); }
	template<typename T> long ReferenceVector<T>::size() const throw() { return igraph_vector_ptr_size(&_); }
	
	template<typename T> void ReferenceVector<T>::clear() throw() { igraph_vector_ptr_clear(&_); }
	template<typename T> void ReferenceVector<T>::reserve(const long new_size) MAY_THROW_EXCEPTION { TRY(igraph_vector_ptr_reserve(&_, new_size)); }
	template<typename T> void ReferenceVector<T>::resize(const long new_size) MAY_THROW_EXCEPTION { TRY(igraph_vector_ptr_resize(&_, new_size)); }
	template<typename T> void ReferenceVector<T>::push_back(value_type const e) MAY_THROW_EXCEPTION { TRY(igraph_vector_ptr_push_back(&_, e)); }
	template<typename T> void ReferenceVector<T>::insert(const long pos, value_type const e) MAY_THROW_EXCEPTION { TRY(igraph_vector_ptr_insert(&_, pos, e)); }
	template<typename T> void ReferenceVector<T>::remove(const long pos) throw() { igraph_vector_ptr_remove(&_, pos); }
	
	template<typename T> void ReferenceVector<T>::copy_to(value_type* store) const throw() { igraph_vector_ptr_copy_to(&_, store); }
	template<typename T> void ReferenceVector<T>::sort(int(*compar)(const value_type, const value_type)) { igraph_vector_ptr_sort(&_, compar); }
	
	template<typename T>
	void ReferenceVector<T>::perform(void(*fptr)(value_type& obj, void* context), void* context) {
		for (iterator cit = begin(); cit != end(); ++ cit)
			fptr(*cit, context);
	}
	
	template<typename T>
	bool ReferenceVector<T>::operator==(const ReferenceVector<T>& other) const throw() {
		if (size() != other.size())
			return false;
		for (const_iterator cit = begin(), cit2 = other.begin(); cit != end(); ++ cit, ++ cit2)
			if (*cit != *cit2)
				return false;
		return true;
	}
	
	template<typename T>
	bool ReferenceVector<T>::operator!=(const ReferenceVector<T>& other) const throw() { return !(*this == other); }
	
}

#endif