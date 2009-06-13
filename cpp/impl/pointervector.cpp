/*

pointervector.cpp ... Wrapper implementation of igraph pointer vector.

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

#ifndef IGRAPH_POINTERVECTOR_CPP
#define IGRAPH_POINTERVECTOR_CPP


#include <igraph/cpp/pointervector.hpp>

namespace igraph {
	MEMORY_MANAGER_IMPLEMENTATION_ATTR_WITH_TEMPLATE(template<typename T>, , PointerVector, <T>);
	
	template<typename T>
	XXINTRNL_WRAPPER_CONSTRUCTOR_IMPLEMENTATION(PointerVector, igraph_vector_ptr_t, igraph_vector_ptr_copy, <T>);
	
	template<typename T>
	IMPLEMENT_COPY_METHOD_WITH_TEMPLATE(PointerVector, <T>) MAY_THROW_EXCEPTION {
		TRY(igraph_vector_ptr_copy(&_, &other._));
	}
	
	template<typename T>
	IMPLEMENT_MOVE_METHOD_WITH_TEMPLATE(PointerVector, <T>) throw() {
		_ = std::move(other._);
	}
	
	template<typename T>
	IMPLEMENT_DEALLOC_METHOD_WITH_TEMPLATE(PointerVector, <T>) {
		igraph_vector_ptr_destroy(&_);
	}
	
	template<typename T>
	PointerVector<T>::PointerVector(const long count) MAY_THROW_EXCEPTION {
		TRY(igraph_vector_ptr_init(&_, count));
	}
	
	template<typename T>
	PointerVector<T>::PointerVector(T** const array, const long count) MAY_THROW_EXCEPTION {
		XXINTRNL_DEBUG_CALL_INITIALIZER(PointerVector, <T>);
		TRY(igraph_vector_ptr_init_copy(&_, reinterpret_cast<void**>(array), count));
	}
	
	template<typename T>
	PointerVector<T>::PointerVector(const long count, ...) MAY_THROW_EXCEPTION {
		XXINTRNL_DEBUG_CALL_INITIALIZER(PointerVector, <T>);
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
	
	template<typename T>
	void PointerVector<T>::null() throw() { igraph_vector_ptr_null(&_); }
	
	template<typename T> T* PointerVector<T>::e(const long index) const throw() { return igraph_vector_ptr_e(&_, index); }
	template<typename T> void PointerVector<T>::set(const long index, T* const value) throw() { igraph_vector_ptr_set(&_, index, value); }
	
	template<typename T> bool PointerVector<T>::empty() const throw() { return igraph_vector_ptr_empty(&_); }
	template<typename T> long PointerVector<T>::size() const throw() { return igraph_vector_ptr_size(&_); }
	
	template<typename T> void PointerVector<T>::clear() throw() { igraph_vector_ptr_clear(&_); }
	template<typename T> void PointerVector<T>::reserve(const long new_size) MAY_THROW_EXCEPTION { TRY(igraph_vector_ptr_reserve(&_, new_size)); }
	template<typename T> void PointerVector<T>::resize(const long new_size) MAY_THROW_EXCEPTION { TRY(igraph_vector_ptr_resize(&_, new_size)); }
	template<typename T> void PointerVector<T>::push_back(T* const e) MAY_THROW_EXCEPTION { TRY(igraph_vector_ptr_push_back(&_, e)); }
	template<typename T> void PointerVector<T>::insert(const long pos, T* const e) MAY_THROW_EXCEPTION { TRY(igraph_vector_ptr_insert(&_, pos, e)); }
	template<typename T> void PointerVector<T>::remove(const long pos) throw() { igraph_vector_ptr_remove(&_, pos); }
	
	template<typename T> void PointerVector<T>::copy_to(T** store) const throw() { igraph_vector_ptr_copy_to(&_, store); }
	template<typename T> void PointerVector<T>::sort(int(*compar)(const T*, const T*)) { igraph_vector_ptr_sort(&_, compar); }
	
	template<typename T>
	void PointerVector<T>::perform(void(*fptr)(T*& obj, void* context), void* context) {
		for (iterator cit = begin(); cit != end(); ++ cit)
			fptr(*cit, context);
	}
	
	template<typename T>
	bool PointerVector<T>::operator==(const PointerVector<T>& other) const throw() {
		if (size() != other.size())
			return false;
		for (const_iterator cit = begin(), cit2 = other.begin(); cit != end(); ++ cit, ++ cit2)
			if (*cit != *cit2)
				return false;
		return true;
	}
	
	template<typename T>
	bool PointerVector<T>::operator!=(const PointerVector<T>& other) const throw() { return !(*this == other); }
	
}

#endif