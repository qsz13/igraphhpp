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
 \brief Wrapper for igraph matrices
 \author KennyTM~
 \date June 12th, 2009
 */

#ifndef IGRAPH_MATRIX_HPP
#define IGRAPH_MATRIX_HPP

#include <igraph/igraph.h>
#include <igraph/cpp/common.hpp>
#include <igraph/cpp/exception.hpp>
#include <igraph/cpp/vector.hpp>
#include <cstdio>
#include <utility>
#if XXINTRNL_CXX0X
#include <initializer_list>
#endif

namespace igraph {
	
	XXINTRNL_PREPARE_UNDERLYING_TYPES(BasicMatrix, matrix);	
	/**
	 \class BasicMatrix
	 \brief Wrapper for igraph matrices
	 
	 This template class is a wrapper of the igraph matrices type.
	 In igraph there are four kinds of matrices: Matrices of Reals, longs, chars and Booleans.
	 */
	template <typename T>
	class BasicMatrix {
		XXINTRNL_UNDERLYING_TYPE(BasicMatrix) _;
		
	public:
		MEMORY_MANAGER_INTERFACE_WITH_TEMPLATE(BasicMatrix, <T>);
		XXINTRNL_WRAPPER_CONSTRUCTOR_INTERFACE(BasicMatrix, XXINTRNL_UNDERLYING_TYPE(BasicMatrix));
		
		/// Create a matrix with specified dimensions 
		BasicMatrix(const long nrow, const long ncol) MAY_THROW_EXCEPTION;
		
		/// Create a null matrix.
		static typename ::tempobj::force_temporary_class<BasicMatrix<T> >::type n() MAY_THROW_EXCEPTION;
		
		/**
		 \brief Create a Matrix using content of string.
		 
		 Example:
		 \code
		 Matrix m = Matrix("42 54; 64 75");
		 m.get_row(0).print();	// prints 42 54
		 m.get_col(1).print();	// prints 54 75
		 \endcode
		 */
		BasicMatrix(const char* stringized_elements, const char* row_separator = ";") MAY_THROW_EXCEPTION;
		
		BasicMatrix<T>& null() throw();
		BasicMatrix<T>& fill(const T e) throw();
		
		T* ptr() throw() { return &(MATRIX(_, 0, 0)); }
		const T& operator[](::std::pair<long,long> c) const throw() { return MATRIX(_, c.first, c.second); }
		T& operator[](::std::pair<long,long> c) throw() { return MATRIX(_, c.first, c.second); }
		const T& operator()(long i, long j) const throw() { return MATRIX(_, i, j); }
		T& operator()(long i, long j) throw() { return MATRIX(_, i, j); }
		
		T e(long i, long j) const throw();
		T* e_ptr(long i, long j) const throw();
		BasicMatrix<T>& set(long i, long j, T value) throw();
		
		void copy_to(T* store) const throw();
		BasicMatrix<T>& update(const BasicMatrix<T>& update_from) MAY_THROW_EXCEPTION;
		void swap(BasicMatrix<T>& swap_with) MAY_THROW_EXCEPTION;
		
		typename ::tempobj::force_temporary_class<BasicVector<T> >::type get_row(long index) const MAY_THROW_EXCEPTION;
		typename ::tempobj::force_temporary_class<BasicVector<T> >::type get_col(long index) const MAY_THROW_EXCEPTION;
		BasicMatrix<T>& set_row(long index, const BasicVector<T>& row_vec) MAY_THROW_EXCEPTION;
		BasicMatrix<T>& set_col(long index, const BasicVector<T>& col_vec) MAY_THROW_EXCEPTION;
		BasicMatrix<T>& swap_rows(long from, long to) MAY_THROW_EXCEPTION;
		BasicMatrix<T>& swap_cols(long from, long to) MAY_THROW_EXCEPTION;
		typename ::tempobj::force_temporary_class<BasicMatrix<T> >::type select_rows(const Vector& row_indices) const MAY_THROW_EXCEPTION;
		typename ::tempobj::force_temporary_class<BasicMatrix<T> >::type select_cols(const Vector& col_indices) const MAY_THROW_EXCEPTION;
		
		BasicMatrix<T>& operator+= (const T k) throw();
		BasicMatrix<T>& operator-= (const T k) throw();
		BasicMatrix<T>& operator*= (const T k) throw();
		BasicMatrix<T>& operator/= (const T k) throw();
		
		BasicMatrix<T>& operator+= (const BasicMatrix<T>& k) MAY_THROW_EXCEPTION;
		BasicMatrix<T>& operator-= (const BasicMatrix<T>& k) MAY_THROW_EXCEPTION;
		BasicMatrix<T>& mul_elements(const BasicMatrix<T>& k) MAY_THROW_EXCEPTION;
		BasicMatrix<T>& div_elements(const BasicMatrix<T>& k) MAY_THROW_EXCEPTION;
		
		Real sum() const throw();
		Real prod() const throw();
		::tempobj::force_temporary_class<Vector>::type rowsum() const MAY_THROW_EXCEPTION;
		::tempobj::force_temporary_class<Vector>::type colsum() const MAY_THROW_EXCEPTION;
		BasicMatrix<T>& transpose() MAY_THROW_EXCEPTION;
		
		BasicMatrix<T>& rbind(const BasicMatrix<T>& from) MAY_THROW_EXCEPTION;
		BasicMatrix<T>& cbind(const BasicMatrix<T>& from) MAY_THROW_EXCEPTION;
		
		Real min() const throw();
		Real max() const throw();
		::std::pair<long,long> which_min() const throw();
		::std::pair<long,long> which_max() const throw();
		void minmax(T& minStore, T& maxStore) const MAY_THROW_EXCEPTION;
		void which_minmax(::std::pair<long,long>& minStore, ::std::pair<long,long>& maxStore) const MAY_THROW_EXCEPTION;
		
		bool empty() const throw();
		bool isnull() const throw();
		long size() const throw();
		long nrow() const throw();
		long ncol() const throw();
		bool is_symmetric() const throw();
		
		bool operator== (const BasicMatrix<T>& other) const throw();
		bool operator!= (const BasicMatrix<T>& other) const throw();
		T maxdifference(const BasicMatrix<T>& other) const throw();
		
		bool contains(T e) const throw();
		bool search(T what, long from = 0) const throw();
		bool search(T what, long from, ::std::pair<long, long>& pos) const throw();
		bool search(T what, long from, ::std::pair<long, long>& pos, long& linear_pos) const throw();
		
		BasicMatrix<T>& resize(long new_nrow, long new_ncol) MAY_THROW_EXCEPTION;
		BasicMatrix<T>& add_rows(long n) MAY_THROW_EXCEPTION;
		BasicMatrix<T>& add_cols(long n) MAY_THROW_EXCEPTION;
		BasicMatrix<T>& add_row() MAY_THROW_EXCEPTION { return add_rows(1); }
		BasicMatrix<T>& add_col() MAY_THROW_EXCEPTION { return add_cols(1); }
		BasicMatrix<T>& remove_row(long i) MAY_THROW_EXCEPTION;
		BasicMatrix<T>& remove_col(long j) MAY_THROW_EXCEPTION;
				
		/// Print content of the Matrix.
		void print(const char* row_separator = "; ", const char* separator = " ", std::FILE* f = stdout) const throw() {
			long m = nrow(), n = ncol();
			for (long i = 0; i < m; ++ i) {
				if (i != 0)
					::std::fprintf(f, "%s", row_separator);
				for (long j = 0; j < n; ++ j) {
					if (j != 0)
						::std::fprintf(f, "%s", separator);
					XXINTRNL_fprintf(f, MATRIX(_, i, j));
				}
			}
			::std::fprintf(f, "\n");
		}
	};
	MEMORY_MANAGER_INTERFACE_EX_WITH_TEMPLATE(template<typename T>, BasicMatrix, <T>);
	
	typedef BasicMatrix<Real> Matrix;
}

#include <igraph/cpp/impl/matrix.cpp>
										   
#endif
