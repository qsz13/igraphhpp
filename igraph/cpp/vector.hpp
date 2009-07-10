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
#include <cstdio>
#if XXINTRNL_CXX0X
#include <initializer_list>
#endif

namespace igraph {
	template <typename T>
	class BasicMatrix;
	
	XXINTRNL_PREPARE_UNDERLYING_TYPES(BasicVector, vector);	
	/**
	 \class BasicVector
	 \brief Wrapper for igraph vectors
	 
	 The BasicVector data type is a simple and efficient growable array
	 implementation. It is compatible with C++'s \c std::vector<T>. This structure
	 provides a random access iterator, so you can use it in STL algorithms.
	 
	 BasicVector is a wrapper above the \c igraph_vector_t type. The are only 4
	 kinds of \type igraph_vector_t, and therefore the scope of BasicVector is
	 also limited to this. They are:
		 - Real (double)
		 - long
		 - char
		 - Boolean (int)
	 
	 Attempting to use BasicVector for other types will generate a compile-time
	 error. 
	 
	 In igraph, mostly a real-valued vector is used. Therefore BasicVector<Real>
	 is aliased as Vector for convenient. Vector is also aliased as VertexVector
	 and EdgeVector to indicate the container stores Vertex and Edge, instead of
	 just plain numbers.
	 
	 Example usage:
	 \code
		 BasicVector<long> v = BasicVector<long>::seq(1, 10);
		 printf("%lg", v.prod());	// computes 10! = 3,628,800.
	 \endcode
	 */
	template <typename T>
	class BasicVector {
		XXINTRNL_UNDERLYING_TYPE(BasicVector) _;
		
	public:
		MEMORY_MANAGER_INTERFACE_WITH_TEMPLATE(BasicVector, <T>);
		XXINTRNL_WRAPPER_CONSTRUCTOR_INTERFACE(BasicVector, XXINTRNL_UNDERLYING_TYPE(BasicVector));
		
		/**
		 \brief Initializes a vector from an ordinary C array.
		 \param[in] array A regular C array.
		 \param[in] count The number of elements in the array.
		 
		 - \b Complexity: O(\p count)
		 - \b Wraps: \c igraph_vector_init_copy
		 */
		BasicVector(T* array, long count) MAY_THROW_EXCEPTION;
		
		/**
		 \brief Initializes a zero vector.
		 
		 Note that you cannot pass 0 to \p count. This is because 0 can also
		 ambiguously denote a NULL pointer. If you want to create an empty vector,
		 use the static constructor n().
		 
		 \param[in] count The size of vector.
		 
		 - \b Complexity: O(\p count)
		 - \b Wraps: \c igraph_vector_init
		 */
		BasicVector(long count) MAY_THROW_EXCEPTION;
		
#if XXINTRNL_CXX0X
		/// Create a Vector using initializer list (C++0x only.)
		BasicVector(::std::initializer_list<T> elements) MAY_THROW_EXCEPTION;
#endif
		
		/**
		 \brief Create a vector using content of string.
		 
		 Any non-numbers will be treated as a separator. Therefore "1 2 3 4"
		 and "1,2,3,4" and "1 birds 2 geese 3 cats 4 dogs" all produce
		 the same vector.
		 
		 Example:
		 \code
		 Vector v = Vector("42 54 64 75");
		 printf("%lg", v.prod());	// prints 10886400
		 \endcode
		 
		 \param[in] stringized_elements The string to parse.
		 
		 - \b Complexity: O(strlen)
		 */
		BasicVector(const char* stringized_elements) MAY_THROW_EXCEPTION;
		
		/**
		 \brief Create a vector sequentially between two values inclusively.		 
		 The vector will contain the numbers \p from, \p from+1, ..., \p to.
		 \param[in] from The lower limit in the sequence (inclusive).
		 \param[in] to The upper limit in the sequence (inclusive).
		 
		 - \b Complexity: O(\p to - \p from)
		 - \b Wraps: \c igraph_vector_init_seq
		 */
		static typename ::tempobj::force_temporary_class<BasicVector<T> >::type seq(T from, T to) MAY_THROW_EXCEPTION;
		
		static typename ::tempobj::force_temporary_class<BasicVector<T> >::type zeros(long count = 0) MAY_THROW_EXCEPTION
		__attribute__((deprecated,warning("BasicVector<T>::zeros is deprecated. Use the constructor BasicVector(x) (for x > 0) or BasicVector<T>::n (for x = 0) instead.")));
		
		/**
		 \brief Create an empty vector.
		 The returned vector will have size of 0.
		 
		 - \b Complexity: O(1)
		 - \b Wraps: \c igraph_vector_init(&_, 0)
		 */
		static typename ::tempobj::force_temporary_class<BasicVector<T> >::type n() MAY_THROW_EXCEPTION;
		
		/**
		 \brief Temporarily wrap a C array as a BasicVector. 
		 
		 Use this constructor if you need to temporarily treat a C array as a 
		 BasicVector. The time complexity of this is only O(1), whereas the copy
		 constructor needs O(n) time.
		 
		 The life cycle of BasicVector will not affect this array. Make sure you
		 don't change the size of the vector or deallocate the original C array,
		 or you may crash the program.
		 
		 \param[in] array A regular C array.
		 \param[in] count The number of elements in the array.
		 
		 - \b Complexity: O(1)
		 - \b Wraps: \c igraph_vector_view
		 */
		static typename ::tempobj::force_temporary_class<BasicVector<T> >::type view(const T* array, long count) throw();
		
		/**
		 \brief Fill the vector with zeros.
		 Note that BasicVector(long count) already zeros out the whole vector,
		 so you don't need to call this again.
		 
		 If you want to make the size of the vector to 0 too, call clear() instead.
		 - \b Complexity: O(n)
		 - \b Wraps: \c igraph_vector_null
		 */
		BasicVector<T>& null() throw();
		
		/**
		 \brief Fill the vector with a constant element.
		 Call null() to set the whole vector to zero, which is more efficient.
		 \param[in] e The element to fill with.		 
		 
		 - \b Complexity: O(n)
		 - \b Wraps: \c igraph_vector_fill
		 */
		BasicVector<T>& fill(T e) throw();
		
		/**
		 \brief Get the pointer to the first element in the vector.
		 \sa e_ptr
		 
		 - \b Complexity: O(1)
		 */
		T* ptr() throw() { return VECTOR(_); }
		/**
		 \brief Get the value at \p index.
		 \param[in] index Index.
		 
		 - \b Complexity: O(1)
		 */
		const T& operator[](long index) const throw() { return VECTOR(_)[index]; }
		T& operator[](long index) throw() { return VECTOR(_)[index]; }
		
		/**
		 \brief Get the value at \p index.
		 Using operator[] is probably faster.
		 \param[in] index Index.
		 
		 - \b Complexity: O(1)
		 - \b Wraps: \c igraph_vector_e
		 */
		T e(long index) const throw();
		/**
		 \brief Get the pointer of the value at \p index.
		 No range checking is done in this method.		 
		 \param[in] index Index.
		 
		 - \b Complexity: O(1)
		 - \b Wraps: \c igraph_vector_e_ptr
		 */
		T* e_ptr(long index) const throw();
		
		/**
		 \brief Assign the value at \p index to another \p value.
		 Using operator[] is probably faster.
		 \param[in] index Index of element to change.
		 \param[in] value Value to assign.
		 
		 - \b Complexity: O(1)
		 - \b Wraps: \c igraph_vector_set
		 */
		BasicVector<T>& set(long index, T value) throw();
		/**
		 \brief Get the last value in the vector.
		 
		 - \b Complexity: O(1)
		 - \b Wraps: \c igraph_vector_tail
		 */
		T tail() const throw();
		
		/**
		 \brief Copy content of vector into a C array.
		 The \p store must be large enough to hold everything.
		 \param[out] store The C array to store the vector content.
		 
		 - \b Complexity: O(n)
		 - \b Wraps: \c igraph_vector_copy_to
		 */
		void copy_to(T* store) const throw();
		/**
		 \brief Update a vector from another one.
		 After this operation the contents of the caller to will be exactly the
		 same as \p update_from. This vector will be resized if it was originally
		 shorter or longer than \p update_from.
		 \param[in] update_from The vector to update from
		 
		 - \b Complexity: O(\p update_from.size)
		 - \b Wraps: \c igraph_vector_update
		 */
		BasicVector<T>& update(const BasicVector<T>& update_from) MAY_THROW_EXCEPTION;
		/**
		 \brief Append a vector.
		 \param[in] append_from The content to append at the end.
		 
		 - \b Complexity: O(\p append_from.size)
		 - \b Wraps: \c igraph_vector_append
		 */
		BasicVector<T>& append(const BasicVector<T>& append_from) MAY_THROW_EXCEPTION;
		/**
		 \brief Swap the content of 2 vectors.
		 The two vectors must have the same length, otherwise an exception will
		 be thrown. Swapping is done in element-by-element fashion, so the time
		 complexity is high. You should do this instead:
		 \code
			 Vector tmp = ::std::move(a);
			 a = ::std::move(b);
			 b = ::std::move(tmp);
		 \endcode
		 \param[in,out] swap_with Vector to swap with.
		 
		 - \b Complexity: O(n)
		 - \b Wraps: \c igraph_vector_swap
		 */
		void swap(BasicVector<T>& swap_with) MAY_THROW_EXCEPTION;
		
		/**
		 \brief Swap two elements in a vector.
		 \param[in] i Index of the first element.
		 \param[in] j index of the second element.
		 
		 - \b Complexity: O(1)
		 - \b Wraps: \c igraph_vector_swap_elements
		 */
		BasicVector<T>& swap_elements(long i, long j) MAY_THROW_EXCEPTION;
		/**
		 \brief Reverse the elements of a vector.
		 - \b Complexity: O(n)
		 - \b Wraps: \c igraph_vector_reverse
		 */
		BasicVector<T>& reverse() MAY_THROW_EXCEPTION;
		
		/**
		 \brief Add a constant to every element of the vector.
		 \param[in] k The constant to add.
		 
		 - \b Complexity: O(n)
		 - \b Wraps: \c igraph_vector_add_constant
		 */
		BasicVector<T>& operator+= (const T k) throw();
		/**
		 \brief Subtract a constant from every element of the vector.
		 \param[in] k The constant to subtract.
		 
		 - \b Complexity: O(n)
		 - \b Wraps: \c igraph_vector_add_constant(&_, -k)
		 */
		BasicVector<T>& operator-= (const T k) throw();
		/**
		 \brief Multiply a constant from every element of the vector.
		 \param[in] k The constant to multiply.
		 
		 - \b Complexity: O(n)
		 - \b Wraps: \c igraph_vector_scale
		 */
		BasicVector<T>& operator*= (const T k) throw();
		/**
		 \brief Divide a constant from every element of the vector.
		 \param[in] k The constant to divide.
		 
		 \bug Does not work for non-Real types.
		 
		 - \b Complexity: O(n)
		 - \b Wraps: \c igraph_vector_scale(&_, 1/k)
		 */		
		BasicVector<T>& operator/= (const T k) throw();
		
		/**
		 \brief Add two vectors element-wise.
		 The two vectors must have the same length.
		 \param[in] k The vector to add.
		 
		 - \b Complexity: O(n)
		 - \b Wraps: \c igraph_vector_add
		 */		
		BasicVector<T>& operator+= (const BasicVector<T>& k) MAY_THROW_EXCEPTION;
		/**
		 \brief Subtract two vectors element-wise.
		 The two vectors must have the same length.
		 \param[in] k The vector to subtract.
		 
		 - \b Complexity: O(n)
		 - \b Wraps: \c igraph_vector_sub
		 */
		BasicVector<T>& operator-= (const BasicVector<T>& k) MAY_THROW_EXCEPTION;
		/**
		 \brief Multiply two vectors element-wise.
		 The two vectors must have the same length.
		 \param[in] k The vector to multiply.
		 
		 - \b Complexity: O(n)
		 - \b Wraps: \c igraph_vector_mul
		 */
		BasicVector<T>& operator*= (const BasicVector<T>& k) MAY_THROW_EXCEPTION;
		/**
		 \brief Divide two vectors element-wise.
		 The two vectors must have the same length.
		 \param[in] k The vector to divide.
		 
		 - \b Complexity: O(n)
		 - \b Wraps: \c igraph_vector_div
		 */
		BasicVector<T>& operator/= (const BasicVector<T>& k) MAY_THROW_EXCEPTION;
		
		/**
		 \brief Return the smallest element of a vector
		 - \b Complexity: O(n)
		 - \b Wraps: \c igraph_vector_min

		 */
		Real min() const throw();
		/**
		 \brief Return the largest element of a vector
		 - \b Complexity: O(n)
		 - \b Wraps: \c igraph_vector_max

		 */
		Real max() const throw();
		/**
		 \brief Index of the smallest element.
		 
		 The vector must be non-empty.
		 
		 If the minimum is not unique, then the index of the first one is returned.
		 - \b Complexity: O(n)
		 - \b Wraps: \c igraph_vector_which_min

		 */
		long which_min() const throw();
		/**
		 \brief Index of the largest element.
		 
		 If the vector is empty, -1 is returned..
		 
		 If the maximum is not unique, then the index of the first one is returned.
		 - \b Complexity: O(n)
		 - \b Wraps: \c igraph_vector_which_max

		 */
		long which_max() const throw();
		/**
		 \brief Obtain both the minimum and maximum of the vector at the same time.
		 
		 This is more efficient than calling min() and max() separately, as the
		 vector only need to be traversed once. The vector must not be empty.
		 - \b Complexity: O(n)
		 - \b Wraps: \c igraph_vector_minmax

		 */
		void minmax(T& minStore, T& maxStore) const MAY_THROW_EXCEPTION;
		/**
		 \brief Obtain the indices to both the minimum and maximum of the vector at the same time.
		 
		 This is more efficient than calling which_min() and which_max() separately, as the
		 vector only need to be traversed once. The vector must not be empty.
		 - \b Complexity: O(n)
		 - \b Wraps: \c igraph_vector_which_minmax

		 */
		void which_minmax(long& minStore, long& maxStore) const MAY_THROW_EXCEPTION;
		
		/**
		 \brief Returns whether the vector is empty.
		 - \b Complexity: O(1)
		 - \b Wraps: \c igraph_vector_empty

		 */
		bool empty() const throw();
		/**
		 \brief Returns the size of vector.
		 - \b Complexity: O(1)
		 - \b Wraps: \c igraph_vector_size

		 */
		long size() const throw();
		
		/**
		 \brief Computes the sum of all elements in the vector.
		 - \b Complexity: O(n)
		 - \b Wraps: \c igraph_vector_sum

		 */
		Real sum() const throw();
		/**
		 \brief Computes the product of all elements in the vector.
		 - \b Complexity: O(n)
		 - \b Wraps: \c igraph_vector_prod

		 */
		Real prod() const throw();
		/**
		 \brief Returns whether all elements are zero.
		 - \b Complexity: O(n)
		 - \b Wraps: \c igraph_vector_isnull

		 */
		bool isnull() const throw();
		/**
		 \brief Checks if all elements of a vector are inside the given interval.
		 \param[in] low Lower limit (inclusive)
		 \param[in] high Upper limit (inclusive)
		 
		 - \b Complexity: O(n)
		 - \b Wraps: \c igraph_vector_isininterval

		 */
		bool isininterval(T low, T high) const throw();
		/**
		 \brief Checks if there is at least 1 value in the vector smaller than the given limit.
		 \param[in] upper_limit The limit (exclusive)
		 
		 - \b Complexity: O(n)
		 - \b Wraps: \c igraph_vector_any_smaller

		 */
		bool any_smaller(T upper_limit) const throw();
		/**
		 \brief Checks if two vectors' content are equal.
		 \param[in] other The vector to compare
		 
		 - \b Complexity: O(n)
		 - \b Wraps: \c igraph_vector_is_equal
		 */
		bool operator== (const BasicVector<T>& other) const throw();
		/**
		 \brief Checks if two vectors' content are unequal.
		 \param[in] other The vector to compare
		 
		 - \b Complexity: O(n)
		 */
		bool operator!= (const BasicVector<T>& other) const throw();
		/**
		 \brief Compute the maximum difference between two vectors element-wise.
		 Equivalent to computing max<sub>i</sub> |u<sub>i</sub> - v<sub>i</sub>|.
		 Both vectors must not be empty, but their sizes don't need to be equal.
		 The extra elements will just be ignored. 
		 
		 \param[in] other The vector to compare
		 
		 - \b Complexity: O(n)
		 - \b Wraps: \c igraph_vector_maxdifference
		 */
		T maxdifference(const BasicVector<T>& other) const throw();
		
		/**
		 \brief Check if the vector contains the given value by linear search.
		 Use binsearch() if you knew the vector is already sorted.
		 \param[in] e The value to look for.
		 
		 - \b Complexity: O(n)
		 - \b Wraps: \c igraph_vector_contains
		 */
		bool contains(const T e) const throw();
		/**
		 \brief Check if the vector contains the given value by linear search starting from a specific index.
		 \param[in] what The value to look for.
		 \param[in] from The starting index.
		 
		 - \b Complexity: O(n - \p index)
		 - \b Wraps: \c igraph_vector_search
		 */
		bool search(const T what, const long from = 0) const throw();
		/**
		 \brief Search for a value by linear search.
		 Use binsearch() if you knew the vector is already sorted.
		 \param[in] what The value to look for.
		 \param[in] from The starting index.
		 \param[out] pos The index of the value. If the value does not exist, this argument will not be changed.
		 \returns Whether is value can be found.
		 
		 - \b Complexity: O(n - \p index)
		 - \b Wraps: \c igraph_vector_search
		 */
		bool search(const T what, const long from, long& pos) const throw();
		/**
		 \brief Check if the vector contains the given value by binary search.
		 Use contains() if the vector is not sorted.
		 \param[in] what The value to look for.
		 
		 - \b Complexity: O(log n)
		 - \b Wraps: \c igraph_vector_binsearch2
		 */
		bool binsearch(const T what) const throw();
		/**
		 \brief Search for a value by binary search.
		 Use search() if the vector is not sorted.
		 \param[in] what The value to look for.
		 \param[out] pos The index of the value. If the value does not exist, this argument will not be changed.
		 \returns Whether is value can be found.
		 
		 - \b Complexity: O(log n)
		 - \b Wraps: \c igraph_vector_binsearch
		 */
		bool binsearch(const T what, long& pos) const throw();
		
		/**
		 \brief Removes all elements from a vector.
		 This function will set the size to 0. If you simply want to zero the
		 whole vector without changing its size, use null() instead.
		 - \b Complexity: O(1)
		 - \b Wraps: \c igraph_vector_clear
		 */
		BasicVector<T>& clear() throw();
		/**
		 \brief Reserve memory.
		 BasicVector's are flexible, they can grow and shrink. Growing 
		 however occasionally needs the data in the vector to be copyed.
		 In order to avoid you can call this function to reserve space for
		 future growth of the vector. 
		 
		 Note that this function does \em not change the size of the vector.
		 Let us see a small example to clarify things: if you reserve space for
		 100 elements and the size of your vector was (and still is) 60, then
		 you can surely add additional 40 elements to your vector before it will
		 be copied.
		 
		 You seldom need to call this function explicitly,

		 To change the size, use resize() instead.
		 \param[in] new_capac New capacity of the vector.
		 
		 - \b Complexity: O(\p new_capac)
		 - \b Wraps: \c igraph_vector_reserve
		 */
		BasicVector<T>& reserve(long new_capac) MAY_THROW_EXCEPTION;
		/**
		 \brief Resize the vector.
		 If you increase the size of a vector, the newly appeared elements are
		 \em not set to zero, they are uninitialized.
		 
		 To resize a vector to zero 
		 \param[in] new_size The new size of the vector
		 
		 - \b Complexity: O(1) if n \>= \p new_size, O(new_size) otherwise.
		 - \b Wraps: \c igraph_vector_resize
		 */
		BasicVector<T>& resize(long new_size) MAY_THROW_EXCEPTION;
		/**
		 \brief Appends one element to the end of a vector.
		 \param[in] e The element to append.
		 
		 - \b Complexity: O(n), but amortized O(1).
		 - \b Wraps: \c igraph_vector_push_back
		 */
		BasicVector<T>& push_back(T e) MAY_THROW_EXCEPTION;
		/**
		 \brief Returns the last element of a vector, and remove that element.
		 Calling it on an empty vector will terminate the program immediately or
		 result in undefined behavior.
		 - \b Complexity: O(1).
		 - \b Wraps: \c igraph_vector_pop_back
		 */
		T pop_back() throw();
		/**
		 \brief Insert a single element into a vector.
		 Note that this function does not do range checking. Insertion will shift the
		 elements from the position given to the end of the vector one position to the
		 right, and the new element will be inserted in the empty space created at
		 the given position. The size of the vector will increase by one.
		 \param[in] pos The position where the new element is inserted
		 \param[in] e The value to insert.
		 
		 - \b Complexity: O(n)
		 - \b Wraps: \c igraph_vector_insert
		 */
		BasicVector<T>& insert(long pos, T e) MAY_THROW_EXCEPTION;
		/**
		 \brief Remove a single element from a vector.
		 \param[in] pos The position where the element will be removed.
		 
		 - \b Complexity: O(n - \p pos)
		 - \b Wraps: \c igraph_vector_remove
		 */
		BasicVector<T>& remove(long pos) throw();
		/**
		 \brief Delete a section (slice) from a vector
		 \param[in] from The starting index to remove (inclusive).
		 \param[in] to The ending index to remove (\em exclusive).
		 
		 - \b Complexity: O(n - \p from)
		 - \b Wraps: \c igraph_vector_remove_section
		 */
		BasicVector<T>& remove_section(long from, long to) throw();
		
		/**
		 \brief Sort a vector ascendingly.
		 It uses the standard C library's \c qsort (Quicksort) function to sort
		 the vector. 
		 
		 - \b Complexity: O(n log n) in most case; O(n<sup>2</sup>) the worst-case scenario.
		 - \b Wraps: \c igraph_vector_sort
		 */
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
		
		/**
		 \brief Returns the begin iterator for use in STL.
		 */
		iterator begin() throw() { return _.stor_begin; }
		/**
		 \brief Returns the end iterator for use in STL.
		 */
		iterator end() throw() { return _.end; }
		const_iterator begin() const throw() { return _.stor_begin; }
		const_iterator end() const throw() { return _.end; }
		size_type capacity() const throw() { return _.stor_end - _.stor_begin; }
		reference front() throw() { return *_.stor_begin; }
		const_reference front() const throw() { return *_.stor_begin; }
		reference back() throw() { return *(_.end-1); }
		const_reference back() const throw() { return *(_.end-1); }
		
		/** Print content of the Vector.
		 \param[in] separator The separator between each element. It should not contain any numbers.
		 \param[in] f The file stream to write to.
		 
		 - \b Complexity: O(n).
		 */
		void print(const char* separator = " ", std::FILE* f = stdout) const throw() {
			bool is_first = true;
			for (const_iterator cit = begin(); cit != end(); ++ cit) {
				if (is_first)
					is_first = false;
				else
					::std::fprintf(f, "%s", separator);
				XXINTRNL_fprintf(f, *cit);
			}
			::std::fprintf(f, "\n");
		}
		
		/**
		 \brief Construct a Vector by reading from an input iterator.
		 \param[in] from The begin iterator
		 \param[in] to The end iterator.
		 
		 - \b Complexity: O(\p to - \p from).
		 */
		template<typename InputIterator>
		BasicVector(InputIterator from, InputIterator to) MAY_THROW_EXCEPTION;
		
		/**
		 \brief Remove the first element with value matching the specified one by linear search.
		 \param[in] e What value to remove.
		 
		 - \b Complexity: O(n)
		 */
		BasicVector<T>& remove_first_matching(T e) throw();
		/**
		 \brief Remove all elements with value matching the specified one by linear search.
		 \param[in] e What value to remove.
		 
		 - \b Complexity: O(n * number of matches)
		 */
		BasicVector<T>& remove_all_matching(T e) throw();
		/**
		 \brief Remove the first element with value matching the specified one by binary search.
		 \param[in] e What value to remove.
		 
		 - \b Complexity: O(n) if found, O(log n) if not.
		 */
		BasicVector<T>& remove_first_matching_assume_sorted(T e) throw();
		
		/**
		 \brief Compute the discrete distribution based on the statistics on this vector.
		 This vector should only contain positive integers. The returned vector
		 has size 1 + this-\>max(), with each element representing the distribution
		 of the value at that index.
		 - \b Complexity: O(max).
		 */
		::tempobj::temporary_class<BasicVector<Real> >::type distribution() const MAY_THROW_EXCEPTION;
		
		// Undocumented functinos?
		/**
		 \brief Move a section of elements to another position.
		 \param[in] from_begin The starting index of the section to move from (inclusive)
		 \param[in] from_end The ending index of the section to move from (exclusive)
		 \param[in] to_begin The starting index of the section to fill into (inclusive)
		 
		 - \b Complexity: O(\p from_end - \p from_begin)
		 - \b Wraps: \c igraph_vector_move_interval2
		 */
		BasicVector<T>& move_section(long from_begin, long from_end, long to_begin) MAY_THROW_EXCEPTION;
		BasicVector<T>& move_interval(long from_begin, long from_end, long to_begin) MAY_THROW_EXCEPTION { return move_section(from_begin, from_end, to_begin); }
		
		// We cannot wrap get_interval() because the implementation was wrong. See https://bugs.launchpad.net/igraph/+bug/390176
		
		/*
		 \brief Shuffles a vector in-place using the Fisher-Yates method
		 The Fisher-Yates shuffle ensures that every implementation is
		 equally probable when using a proper randomness source. Of course
		 this does not apply to pseudo-random generators as the cycle of
		 these generators is less than the number of possible permutations
		 of the vector if the vector is long enough.
		  - \b Complexity: O(n)
		  - \b Wraps: \c igraph_vector_shuffle
		 */
		//BasicVector<T>& shuffle() MAY_THROW_EXCEPTION;
		
		enum ElementUniqueness {
			Unique,
			NotUnique,
		};
		/**
		 \brief Calculates the intersection of two sorted vectors.
		 \param[in] other The vector to intersect with.
		 \param[in] uniqueness Whether to preserve the multiplicity of the elements.
			- \c Unique: All elements in the resulting vector will be unique,
			- \c NotUnique: Multiple elements will be retained.
		 
		 - \b Complexity: O(n), maximum size of the two vectors
		 - \b Wraps: \c igraph_vector_intersect_sorted
		 */
		typename ::tempobj::force_temporary_class<BasicVector<T> >::type intersect_sorted(const BasicVector<T>& other, ElementUniqueness uniqueness = Unique) const MAY_THROW_EXCEPTION;
		
		friend class VertexSelector;
		friend class VertexIterator;
		friend class EdgeSelector;
		friend class EdgeIterator;
		friend class Graph;
		friend class GraphWriter;
		friend class GraphReader;
		template<typename U>
		friend class BasicMatrix;
		friend class Community;
		friend class Mincut;
		 
	};
	MEMORY_MANAGER_INTERFACE_EX_WITH_TEMPLATE(template<typename T>, BasicVector<T>);
	
	/// Real vector
	typedef BasicVector<Real> Vector;
	/// Boolean vector
	typedef BasicVector<Boolean> BoolVector;
	/// Vector containing vertices
	typedef Vector VertexVector;
	/// Vector containing edges.
	typedef Vector EdgeVector;
}

#include <igraph/cpp/impl/vector.cpp>
										   
#endif
