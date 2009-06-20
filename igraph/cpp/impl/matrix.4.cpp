/*

matrix.4.cpp ... Macro-parametrized implementation of each of the 4 types of an igraph matrix.

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

template<> XXINTRNL_WRAPPER_CONSTRUCTOR_IMPLEMENTATION(BasicMatrix, TYPE, FUNC(copy), <BASE>);

// We don't need _WITH_TEMPLATE here because the typename can be resolve unambiguously. The typename is not required.
template<> IMPLEMENT_COPY_METHOD(BasicMatrix, <BASE>) { TRY(FUNC(copy)(&_, &other._)); }
template<> IMPLEMENT_MOVE_METHOD(BasicMatrix, <BASE>) { _ = ::std::move(other._); }
template<> IMPLEMENT_DEALLOC_METHOD(BasicMatrix, <BASE>) { FUNC(destroy)(&_); }

template<> BasicMatrix<BASE>::BasicMatrix(const long nrow, const long ncol) MAY_THROW_EXCEPTION {
	XXINTRNL_DEBUG_CALL_INITIALIZER(BasicMatrix, <BASE>);
	TRY(FUNC(init)(&_, nrow, ncol));
}

template<> ::tempobj::force_temporary_class<BasicMatrix<BASE> >::type BasicMatrix<BASE>::n() MAY_THROW_EXCEPTION {
	TYPE _;
	TRY(FUNC(init)(&_, 0, 0));
	return ::tempobj::force_move(BasicMatrix<BASE>(&_, ::tempobj::OwnershipTransferMove));
}

template<> BasicMatrix<BASE>::BasicMatrix(const char* stringized_elements, const char* row_separator) MAY_THROW_EXCEPTION {
	XXINTRNL_DEBUG_CALL_INITIALIZER(BasicMatrix, <BASE>);
	TRY(FUNC(init)(&_, 1, 1));
	long i = 0, j = 0, cols = 1;
	size_t rowsep_len = ::std::strlen(row_separator);
	const char* end_of_string = ::std::strchr(stringized_elements, '\0');
	while (stringized_elements < end_of_string) {
		BASE res;
		if (XXINTRNL_sscanf(stringized_elements, res)) {
			if (j >= cols) {
				TRY(FUNC(add_cols)(&_, 1));
				++ cols;
			}
			MATRIX(_, i, j) = res;
			++ j;
		} else if (::std::memcmp(stringized_elements-1, row_separator, rowsep_len) == 0) {
			TRY(FUNC(add_rows)(&_, 1));
			j = 0;
			++ i;
			stringized_elements += rowsep_len-1;
		}
	}
	if (j == 0) {
		TRY(FUNC(remove_row)(&_, i));
	}
}

#pragma mark -
#pragma mark Initializing elements

template<> BasicMatrix<BASE>& BasicMatrix<BASE>::null() throw() { FUNC(null)(&_); return *this; }
template<> BasicMatrix<BASE>& BasicMatrix<BASE>::fill(const BASE e) throw() { FUNC(fill)(&_, e); return *this; }

#pragma mark -
#pragma mark Accessing elements

template<> BASE BasicMatrix<BASE>::e(long i, long j) const throw() { return FUNC(e)(&_, i, j); }
template<> BASE* BasicMatrix<BASE>::e_ptr(long i, long j) const throw() { return FUNC(e_ptr)(&_, i, j); }
template<> BasicMatrix<BASE>& BasicMatrix<BASE>::set(long i, long j, const BASE value) throw() { FUNC(set)(&_, i, j, value); return *this; }

#pragma mark -
#pragma mark Copying matrices

template<> void BasicMatrix<BASE>::copy_to(BASE* store) const throw() { FUNC(copy_to)(&_, store); }
template<> BasicMatrix<BASE>& BasicMatrix<BASE>::update(const BasicMatrix<BASE>& update_from) MAY_THROW_EXCEPTION { TRY(FUNC(update)(&_, &update_from._)); return *this; }
template<> void BasicMatrix<BASE>::swap(BasicMatrix<BASE>& swap_with) MAY_THROW_EXCEPTION { TRY(FUNC(swap)(&_, &swap_with._)); }

#pragma mark -
#pragma mark Operations on rows and columns

template<> ::tempobj::force_temporary_class<BasicVector<BASE> >::type BasicMatrix<BASE>::get_row(long index) const MAY_THROW_EXCEPTION {
	::tempobj::force_temporary_class<BasicVector<BASE> >::type res = ::tempobj::force_move(BasicVector<BASE>::n());
	TRY(FUNC(get_row)(&_, &res._, index));
	return res;
}
template<> ::tempobj::force_temporary_class<BasicVector<BASE> >::type BasicMatrix<BASE>::get_col(long index) const MAY_THROW_EXCEPTION {
	::tempobj::force_temporary_class<BasicVector<BASE> >::type res = ::tempobj::force_move(BasicVector<BASE>::n());
	TRY(FUNC(get_col)(&_, &res._, index));
	return res;
}
template<> BasicMatrix<BASE>& BasicMatrix<BASE>::set_row(long index, const BasicVector<BASE>& row_vec) MAY_THROW_EXCEPTION {
	TRY(FUNC(set_row)(&_, &row_vec._, index));
	return *this;
}
template<> BasicMatrix<BASE>& BasicMatrix<BASE>::set_col(long index, const BasicVector<BASE>& col_vec) MAY_THROW_EXCEPTION {
	TRY(FUNC(set_col)(&_, &col_vec._, index));
	return *this;
}
template<> BasicMatrix<BASE>& BasicMatrix<BASE>::swap_rows(long from, long to) MAY_THROW_EXCEPTION {
	TRY(FUNC(swap_rows)(&_, from, to));
	return *this;
}
template<> BasicMatrix<BASE>& BasicMatrix<BASE>::swap_cols(long from, long to) MAY_THROW_EXCEPTION {
	TRY(FUNC(swap_cols)(&_, from, to));
	return *this;
}
template<> ::tempobj::force_temporary_class<BasicMatrix<BASE> >::type BasicMatrix<BASE>::select_rows(const Vector& row_indices) const MAY_THROW_EXCEPTION {
	TYPE retval;
	TRY(FUNC(init)(&retval, 0, 0));
	TRY(FUNC(select_rows)(&_, &retval, &row_indices._));
	return ::tempobj::force_move(BasicMatrix<BASE>(&retval, ::tempobj::OwnershipTransferMove));
}
template<> ::tempobj::force_temporary_class<BasicMatrix<BASE> >::type BasicMatrix<BASE>::select_cols(const Vector& col_indices) const MAY_THROW_EXCEPTION {
	TYPE retval;
	TRY(FUNC(init)(&retval, 0, 0));
	TRY(FUNC(select_cols)(&_, &retval, &col_indices._));
	return ::tempobj::force_move(BasicMatrix<BASE>(&retval, ::tempobj::OwnershipTransferMove));
}

#pragma mark -
#pragma mark Matrix operations

template<> BasicMatrix<BASE>& BasicMatrix<BASE>::operator+= (const BASE k) throw() { FUNC(add_constant)(&_, k); return *this; }
template<> BasicMatrix<BASE>& BasicMatrix<BASE>::operator-= (const BASE k) throw() { FUNC(add_constant)(&_, -k); return *this; }
template<> BasicMatrix<BASE>& BasicMatrix<BASE>::operator*= (const BASE k) throw() { FUNC(scale)(&_, k); return *this; }
template<> BasicMatrix<BASE>& BasicMatrix<BASE>::operator/= (const BASE k) throw() { FUNC(scale)(&_, 1/k); return *this; }

template<> BasicMatrix<BASE>& BasicMatrix<BASE>::operator+= (const BasicMatrix<BASE>& k) MAY_THROW_EXCEPTION { TRY(FUNC(add)(&_, &k._)); return *this; }
template<> BasicMatrix<BASE>& BasicMatrix<BASE>::operator-= (const BasicMatrix<BASE>& k) MAY_THROW_EXCEPTION { TRY(FUNC(sub)(&_, &k._)); return *this; }
template<> BasicMatrix<BASE>& BasicMatrix<BASE>::mul_elements(const BasicMatrix<BASE>& k) MAY_THROW_EXCEPTION { TRY(FUNC(mul_elements)(&_, &k._)); return *this; }
template<> BasicMatrix<BASE>& BasicMatrix<BASE>::div_elements(const BasicMatrix<BASE>& k) MAY_THROW_EXCEPTION { TRY(FUNC(div_elements)(&_, &k._)); return *this; }

IMMEDIATE_OPERATOR_IMPLEMENTATION_COMMUTATIVE(BasicMatrix<BASE>, +, BASE);
IMMEDIATE_OPERATOR_IMPLEMENTATION_RHS(BasicMatrix<BASE>, -, BASE);
IMMEDIATE_OPERATOR_IMPLEMENTATION_COMMUTATIVE(BasicMatrix<BASE>, *, BASE);
IMMEDIATE_OPERATOR_IMPLEMENTATION_RHS(BasicMatrix<BASE>, /, BASE);
IMMEDIATE_OPERATOR_IMPLEMENTATION(BasicMatrix<BASE>, +);
IMMEDIATE_OPERATOR_IMPLEMENTATION(BasicMatrix<BASE>, -);

template<> Real BasicMatrix<BASE>::sum() const throw() { return FUNC(sum)(&_); }
template<> Real BasicMatrix<BASE>::prod() const throw() { return FUNC(prod)(&_); }
template<> ::tempobj::force_temporary_class<Vector>::type BasicMatrix<BASE>::rowsum() const MAY_THROW_EXCEPTION {
	igraph_vector_t res;
	TRY(igraph_vector_init(&res, 0));
	TRY(FUNC(rowsum)(&_, &res));
	return ::tempobj::force_move(Vector(&res, ::tempobj::OwnershipTransferMove));
}
template<> ::tempobj::force_temporary_class<Vector>::type BasicMatrix<BASE>::colsum() const MAY_THROW_EXCEPTION {
	igraph_vector_t res;
	TRY(igraph_vector_init(&res, 0));
	TRY(FUNC(colsum)(&_, &res));
	return ::tempobj::force_move(Vector(&res, ::tempobj::OwnershipTransferMove));
}
template<> BasicMatrix<BASE>& BasicMatrix<BASE>::transpose() MAY_THROW_EXCEPTION {
	TRY(FUNC(transpose)(&_));
	return *this;
}

#pragma mark -
#pragma mark Combining matrices

template<> BasicMatrix<BASE>& BasicMatrix<BASE>::rbind(const BasicMatrix<BASE>& from) MAY_THROW_EXCEPTION {
	TRY(FUNC(rbind)(&_, &from._));
	return *this;
}
template<> BasicMatrix<BASE>& BasicMatrix<BASE>::cbind(const BasicMatrix<BASE>& from) MAY_THROW_EXCEPTION {
	TRY(FUNC(cbind)(&_, &from._));
	return *this;
}

#pragma mark -
#pragma mark Finding minimum and maximum

template<> Real BasicMatrix<BASE>::min() const throw() { return FUNC(min)(&_); }
template<> Real BasicMatrix<BASE>::max() const throw() { return FUNC(max)(&_); }
template<> ::std::pair<long,long> BasicMatrix<BASE>::which_min() const throw() {
	long i, j;
	TRY(FUNC(which_min)(&_, &i, &j));
	return ::std::pair<long,long>(i, j);
}
template<> ::std::pair<long,long> BasicMatrix<BASE>::which_max() const throw() {
	long i, j;
	TRY(FUNC(which_max)(&_, &i, &j));
	return ::std::pair<long,long>(i, j);
}
template<> void BasicMatrix<BASE>::minmax(BASE& minStore, BASE& maxStore) const MAY_THROW_EXCEPTION { TRY(FUNC(minmax)(&_, &minStore, &maxStore)); }
template<> void BasicMatrix<BASE>::which_minmax(::std::pair<long,long>& minStore, ::std::pair<long,long>& maxStore) const MAY_THROW_EXCEPTION { TRY(FUNC(which_minmax)(&_, &minStore.first, &minStore.second, &maxStore.first, &maxStore.second)); }

#pragma mark -
#pragma mark Matrix properties

template<> bool BasicMatrix<BASE>::empty() const throw() { return FUNC(empty)(&_); }
template<> bool BasicMatrix<BASE>::isnull() const throw() { return FUNC(isnull)(&_); }
template<> long BasicMatrix<BASE>::size() const throw() { return FUNC(size)(&_); }
template<> long BasicMatrix<BASE>::nrow() const throw() { return FUNC(nrow)(&_); }
template<> long BasicMatrix<BASE>::ncol() const throw() { return FUNC(ncol)(&_); }
template<> bool BasicMatrix<BASE>::is_symmetric() const throw() { return FUNC(is_symmetric)(&_); }

template<> bool BasicMatrix<BASE>::operator== (const BasicMatrix<BASE>& other) const throw() { return FUNC(is_equal)(&_, &other._); }
template<> bool BasicMatrix<BASE>::operator!= (const BasicMatrix<BASE>& other) const throw() { return !(*this == other); }
template<> BASE BasicMatrix<BASE>::maxdifference(const BasicMatrix<BASE>& other) const throw() { return FUNC(maxdifference)(&_, &other._); }

#pragma mark -
#pragma mark Searching for elements

template<> bool BasicMatrix<BASE>::contains(const BASE e) const throw() { return FUNC(contains)(&_, e); }
template<> bool BasicMatrix<BASE>::search(const BASE what, const long from) const throw() {
	long pos, row, col;
	return FUNC(search)(&_, from, what, &pos, &row, &col);
}
template<> bool BasicMatrix<BASE>::search(BASE what, long from, ::std::pair<long, long>& pos) const throw() {
	long linear_pos;
	return FUNC(search)(&_, from, what, &linear_pos, &pos.first, &pos.second);
}
template<> bool BasicMatrix<BASE>::search(BASE what, long from, ::std::pair<long, long>& pos, long& linear_pos) const throw() {
	return FUNC(search)(&_, from, what, &linear_pos, &pos.first, &pos.second);
}

#pragma mark -
#pragma mark Resizing operations

template<> BasicMatrix<BASE>& BasicMatrix<BASE>::resize(long new_nrow, long new_ncol) MAY_THROW_EXCEPTION { TRY(FUNC(resize)(&_, new_nrow, new_ncol)); return *this; }
template<> BasicMatrix<BASE>& BasicMatrix<BASE>::add_rows(long n) MAY_THROW_EXCEPTION { TRY(FUNC(add_rows)(&_, n)); return *this; }
template<> BasicMatrix<BASE>& BasicMatrix<BASE>::add_cols(long n) MAY_THROW_EXCEPTION { TRY(FUNC(add_cols)(&_, n)); return *this; }
template<> BasicMatrix<BASE>& BasicMatrix<BASE>::remove_row(long i) MAY_THROW_EXCEPTION { TRY(FUNC(remove_row)(&_, i)); return *this; }
template<> BasicMatrix<BASE>& BasicMatrix<BASE>::remove_col(long j) MAY_THROW_EXCEPTION { TRY(FUNC(remove_row)(&_, j)); return *this; }
