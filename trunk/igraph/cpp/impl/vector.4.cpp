/*

vector.4.cpp ... Macro-parametrized implementation of each of the 4 types of an igraph vector.

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

template<> XXINTRNL_WRAPPER_CONSTRUCTOR_IMPLEMENTATION(BasicVector, TYPE, FUNC(copy), <BASE>);

// We don't need _WITH_TEMPLATE here because the typename can be resolve unambiguously. The typename is not required.
template<> IMPLEMENT_COPY_METHOD(BasicVector, <BASE>) { TRY(FUNC(copy)(&_, &other._)); }
template<> IMPLEMENT_MOVE_METHOD(BasicVector, <BASE>) { _ = ::std::move(other._); }
template<> IMPLEMENT_DEALLOC_METHOD(BasicVector, <BASE>) { FUNC(destroy)(&_); }

template<> BasicVector<BASE>::BasicVector(BASE* const array, const long count) MAY_THROW_EXCEPTION {
	XXINTRNL_DEBUG_CALL_INITIALIZER(BasicVector, <BASE>);
	TRY(FUNC(init_copy)(&_, array, count));
}

template<> BasicVector<BASE>::BasicVector(const long count) MAY_THROW_EXCEPTION {
	XXINTRNL_DEBUG_CALL_INITIALIZER(BasicVector, <BASE>);
	TRY(FUNC(init)(&_, count));
}

#if XXINTRNL_CXX0X
template<> BasicVector<BASE>::BasicVector(::std::initializer_list<BASE> elements) MAY_THROW_EXCEPTION {
	XXINTRNL_DEBUG_CALL_INITIALIZER(BasicVector, <BASE>);
	TRY(FUNC(init)(&_, elements.size()));
	BASE* q = VECTOR(_);
	for (auto p = elements.begin(); p != elements.end(); ++p, ++q)
		*q = *p;
}
#endif

template<> BasicVector<BASE>::BasicVector(const char* vertices_as_string) MAY_THROW_EXCEPTION {
	XXINTRNL_DEBUG_CALL_INITIALIZER(BasicVector, <BASE>);
	TRY(FUNC(init)(&_, 0));
	const char* end_of_string = ::std::strchr(vertices_as_string, '\0');
	while (vertices_as_string < end_of_string) {
		BASE res;
		if (XXINTRNL_sscanf(vertices_as_string, res))
			TRY(FUNC(push_back)(&_, res));
	}
}

template<> RETRIEVE_TEMPORARY_CLASS(BasicVector<BASE>) BasicVector<BASE>::seq(const BASE from, const BASE to) MAY_THROW_EXCEPTION {
	TYPE _;
	TRY(FUNC(init_seq)(&_, from, to));
	return FORCE_STD_MOVE(BasicVector<BASE>)(BasicVector<BASE>(&_, ::tempobj::OwnershipTransferMove));
}

template<> RETRIEVE_TEMPORARY_CLASS(BasicVector<BASE>) BasicVector<BASE>::zeros(const long count) MAY_THROW_EXCEPTION {
	return FORCE_STD_MOVE(BasicVector<BASE>)(BasicVector<BASE>(count));
}

template<> RETRIEVE_TEMPORARY_CLASS(BasicVector<BASE>) BasicVector<BASE>::n() MAY_THROW_EXCEPTION {
	TYPE _;
	TRY(FUNC(init)(&_, 0));
	return FORCE_STD_MOVE(BasicVector<BASE>)(BasicVector<BASE>(&_, ::tempobj::OwnershipTransferMove));
}

#pragma mark -
#pragma mark Initializing elements

template<> BasicVector<BASE>& BasicVector<BASE>::null() throw() { FUNC(null)(&_); return *this; }
template<> BasicVector<BASE>& BasicVector<BASE>::fill(const BASE e) throw() { FUNC(fill)(&_, e); return *this; }

#pragma mark -
#pragma mark Accessing elements

template<> BASE BasicVector<BASE>::e(const long index) const throw() { return FUNC(e)(&_, index); }
template<> BASE* BasicVector<BASE>::e_ptr(const long index) const throw() { return FUNC(e_ptr)(&_, index); }
template<> BasicVector<BASE>& BasicVector<BASE>::set(const long index, const BASE value) throw() { FUNC(set)(&_, index, value); return *this; }
template<> BASE BasicVector<BASE>::tail() const throw() { return FUNC(tail)(&_); }

#pragma mark -
#pragma mark Vector views

template<> RETRIEVE_TEMPORARY_CLASS(BasicVector<BASE>) BasicVector<BASE>::view(const BASE* const array, const long count) throw() {
	TYPE _;
	FUNC(view)(&_, array, count);
	return FORCE_STD_MOVE(BasicVector<BASE>)(BasicVector<BASE>(&_, ::tempobj::OwnershipTransferNoOwnership));
}

#pragma mark -
#pragma mark Copying vectors

template<> void BasicVector<BASE>::copy_to(BASE* store) const throw() { FUNC(copy_to)(&_, store); }
template<> BasicVector<BASE>& BasicVector<BASE>::update(const BasicVector<BASE>& update_from) MAY_THROW_EXCEPTION { TRY(FUNC(update)(&_, &update_from._)); return *this; }
template<> BasicVector<BASE>& BasicVector<BASE>::append(const BasicVector<BASE>& append_from) MAY_THROW_EXCEPTION { TRY(FUNC(append)(&_, &append_from._)); return *this; }
template<> void BasicVector<BASE>::swap(BasicVector<BASE>& swap_with) MAY_THROW_EXCEPTION { TRY(FUNC(swap)(&_, &swap_with._)); }

#pragma mark -
#pragma mark Exchanging elements

template<> BasicVector<BASE>& BasicVector<BASE>::swap_elements(const long i, const long j) MAY_THROW_EXCEPTION { TRY(FUNC(swap_elements)(&_, i, j)); return *this; }
template<> BasicVector<BASE>& BasicVector<BASE>::reverse() MAY_THROW_EXCEPTION { TRY(FUNC(reverse)(&_)); return *this; }

#pragma mark -
#pragma mark BasicVector<BASE> operations

template<> BasicVector<BASE>& BasicVector<BASE>::operator+= (const BASE k) throw() { FUNC(add_constant)(&_, k); return *this; }
template<> BasicVector<BASE>& BasicVector<BASE>::operator-= (const BASE k) throw() { FUNC(add_constant)(&_, -k); return *this; }
template<> BasicVector<BASE>& BasicVector<BASE>::operator*= (const BASE k) throw() { FUNC(scale)(&_, k); return *this; }
template<> BasicVector<BASE>& BasicVector<BASE>::operator/= (const BASE k) throw() { FUNC(scale)(&_, 1/k); return *this; }

template<> BasicVector<BASE>& BasicVector<BASE>::operator+= (const BasicVector<BASE>& k) MAY_THROW_EXCEPTION { TRY(FUNC(add)(&_, &k._)); return *this; }
template<> BasicVector<BASE>& BasicVector<BASE>::operator-= (const BasicVector<BASE>& k) MAY_THROW_EXCEPTION { TRY(FUNC(sub)(&_, &k._)); return *this; }
template<> BasicVector<BASE>& BasicVector<BASE>::operator*= (const BasicVector<BASE>& k) MAY_THROW_EXCEPTION { TRY(FUNC(mul)(&_, &k._)); return *this; }
template<> BasicVector<BASE>& BasicVector<BASE>::operator/= (const BasicVector<BASE>& k) MAY_THROW_EXCEPTION { TRY(FUNC(div)(&_, &k._)); return *this; }

IMMEDIATE_OPERATOR_IMPLEMENTATION_COMMUTATIVE(BasicVector<BASE>, +, BASE);
IMMEDIATE_OPERATOR_IMPLEMENTATION_RHS(BasicVector<BASE>, -, BASE);
IMMEDIATE_OPERATOR_IMPLEMENTATION_COMMUTATIVE(BasicVector<BASE>, *, BASE);
IMMEDIATE_OPERATOR_IMPLEMENTATION_RHS(BasicVector<BASE>, /, BASE);
IMMEDIATE_OPERATOR_IMPLEMENTATION(BasicVector<BASE>, +);
IMMEDIATE_OPERATOR_IMPLEMENTATION(BasicVector<BASE>, -);
IMMEDIATE_OPERATOR_IMPLEMENTATION(BasicVector<BASE>, *);
IMMEDIATE_OPERATOR_IMPLEMENTATION(BasicVector<BASE>, /);

#pragma mark -
#pragma mark Finding minimum and maximum

template<> BASE BasicVector<BASE>::min() const throw() { return FUNC(min)(&_); }
template<> BASE BasicVector<BASE>::max() const throw() { return FUNC(max)(&_); }
template<> long BasicVector<BASE>::which_min() const throw() { return FUNC(which_min)(&_); }
template<> long BasicVector<BASE>::which_max() const throw() { return FUNC(which_max)(&_); }
template<> void BasicVector<BASE>::minmax(BASE& minStore, BASE& maxStore) const MAY_THROW_EXCEPTION { TRY(FUNC(minmax)(&_, &minStore, &maxStore)); }
template<> void BasicVector<BASE>::which_minmax(long& minStore, long& maxStore) const MAY_THROW_EXCEPTION { TRY(FUNC(which_minmax)(&_, &minStore, &maxStore)); }

#pragma mark -
#pragma mark Vector properties

template<> bool BasicVector<BASE>::empty() const throw() { return FUNC(empty)(&_); }
template<> long BasicVector<BASE>::size() const throw() { return FUNC(size)(&_); }
template<> bool BasicVector<BASE>::isnull() const throw() { return FUNC(isnull)(&_); }

template<> BASE BasicVector<BASE>::sum() const throw() { return FUNC(sum)(&_); }
template<> BASE BasicVector<BASE>::prod() const throw() { return FUNC(prod)(&_); }
template<> bool BasicVector<BASE>::isininterval(BASE low, BASE high) const throw() { return FUNC(isininterval)(&_, low, high); }
template<> bool BasicVector<BASE>::any_smaller(BASE upper_limit) const throw() { return FUNC(any_smaller)(&_, upper_limit); }
template<> bool BasicVector<BASE>::operator== (const BasicVector<BASE>& other) const throw() { return FUNC(is_equal)(&_, &other._); }
template<> bool BasicVector<BASE>::operator!= (const BasicVector<BASE>& other) const throw() { return !(*this == other); }
template<> BASE BasicVector<BASE>::maxdifference(const BasicVector<BASE>& other) const throw() { return FUNC(maxdifference)(&_, &other._); }

#pragma mark -
#pragma mark Searching for elements

template<> bool BasicVector<BASE>::contains(const BASE e) const throw() { return FUNC(contains)(&_, e); }
template<> bool BasicVector<BASE>::search(const BASE what, const long from) const throw() { return FUNC(search)(&_, from, what, NULL); }
template<> bool BasicVector<BASE>::search(const BASE what, const long from, long& pos) const throw() { return FUNC(search)(&_, from, what, &pos); }
template<> bool BasicVector<BASE>::binsearch(const BASE what) const throw() { return FUNC(binsearch2)(&_, what); }
template<> bool BasicVector<BASE>::binsearch(const BASE what, long& pos) const throw() { return FUNC(binsearch)(&_, what, &pos); }

#pragma mark -
#pragma mark Resizing operations

template<> BasicVector<BASE>& BasicVector<BASE>::clear() throw() { FUNC(clear)(&_); return *this; }
template<> BasicVector<BASE>& BasicVector<BASE>::reserve(const long new_size) MAY_THROW_EXCEPTION { TRY(FUNC(reserve)(&_, new_size)); return *this; }
template<> BasicVector<BASE>& BasicVector<BASE>::resize(const long new_size) MAY_THROW_EXCEPTION { TRY(FUNC(resize)(&_, new_size)); return *this; }
template<> BasicVector<BASE>& BasicVector<BASE>::push_back(const BASE e) MAY_THROW_EXCEPTION { TRY(FUNC(push_back)(&_, e)); return *this; }
template<> BASE BasicVector<BASE>::pop_back() throw() { return FUNC(pop_back)(&_); }
template<> BasicVector<BASE>& BasicVector<BASE>::insert(const long pos, const BASE e) MAY_THROW_EXCEPTION { TRY(FUNC(insert)(&_, pos, e)); return *this; }
template<> BasicVector<BASE>& BasicVector<BASE>::remove(const long pos) throw() { FUNC(remove)(&_, pos); return *this; }
template<> BasicVector<BASE>& BasicVector<BASE>::remove_section(const long from, const long to) throw() { FUNC(remove_section)(&_, from, to); return *this; }

#pragma mark -
#pragma mark Sorting

template<> BasicVector<BASE>& BasicVector<BASE>::sort() throw() { FUNC(sort)(&_); return *this; }

#pragma mark -
#pragma mark Etc.

/// Convert from std::vector.
template<>
template<typename InputIterator>
BasicVector<BASE>::BasicVector(const InputIterator from, const InputIterator to) MAY_THROW_EXCEPTION {
	if (from != to) {
		TRY(FUNC(init)(&_, 0));
		for (InputIterator cit = from; cit != to; ++ cit)
			TRY(FUNC(push_back)(&_, *cit));
	} else
		mm_dont_dealloc = true;
}

template<> BasicVector<BASE>& BasicVector<BASE>::remove_first_matching(const BASE e) throw() {
	long pos;
	if (search(e, 0, pos))
		remove(pos);
	return *this;
}

template<> BasicVector<BASE>& BasicVector<BASE>::remove_all_matching(const BASE e) throw() {
	for (long i = size()-1; i >= 0; -- i)
		if (VECTOR(_)[i] == e)
			FUNC(remove)(&_, i);
	return *this;
}
template<> BasicVector<BASE>& BasicVector<BASE>::remove_first_matching_assume_sorted(const BASE e) throw() {
	long pos;
	if (binsearch(e, pos))
		remove(pos);
	return *this;
}

template<> ::tempobj::temporary_class<Vector>::type BasicVector<BASE>::distribution() const MAY_THROW_EXCEPTION {
	assert(min() >= 0);
	igraph_vector_t res;
	TRY(igraph_vector_init(&res, static_cast<long>(max()) ));
	Real delta = 1./size();
	for (const_iterator cit = begin(); cit != end(); ++ cit)
		VECTOR(res)[static_cast<long>(*cit)] += delta;
	return FORCE_STD_MOVE(Vector)(Vector(&res, ::tempobj::OwnershipTransferMove));
}
