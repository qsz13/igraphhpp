/*

rng.cpp ... GSL Random number generator C++ wrapper

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

#ifndef GSL_RNG_CPP
#define GSL_RNG_CPP

#include <gsl/cpp/rng.hpp>
#include <ctime>

namespace gsl {
	inline void inplace_copy (gsl_rng** to, gsl_rng* const* from) throw() { *to = gsl_rng_clone(*from); }
	
	MEMORY_MANAGER_IMPLEMENTATION(Random);
	XXINTRNL_WRAPPER_CONSTRUCTOR_IMPLEMENTATION(Random, gsl_rng*, inplace_copy);
	
	IMPLEMENT_COPY_METHOD(Random) {
		_ = gsl_rng_clone(other._);
	}
	IMPLEMENT_MOVE_METHOD(Random) {
		_ = ::std::move(other._);
	}
	IMPLEMENT_DEALLOC_METHOD(Random) {
		gsl_rng_free(_);
	}

	const Random& Random::seed() const throw() { return set(::std::time(NULL)); }
	
#pragma mark -
#pragma mark Random number generator algorithms
	
	RETRIEVE_TEMPORARY_CLASS(Random) Random::default_generator() throw() {
		gsl_rng* _ = gsl_rng_alloc(gsl_rng_default);
		return ::std::move(Random(&_, ::tempobj::OwnershipTransferMove));
	}
	
#define XXINTRNL_CREATE_CONSTRUCTOR(xxname) RETRIEVE_TEMPORARY_CLASS(Random) Random::xxname() throw() { return ::std::move(Random(gsl_rng_##xxname)); }
	
	XXINTRNL_CREATE_CONSTRUCTOR(mt19937);
	XXINTRNL_CREATE_CONSTRUCTOR(ranlxs0);
	XXINTRNL_CREATE_CONSTRUCTOR(ranlxs1);
	XXINTRNL_CREATE_CONSTRUCTOR(ranlxs2);
	XXINTRNL_CREATE_CONSTRUCTOR(ranlxd1);
	XXINTRNL_CREATE_CONSTRUCTOR(ranlxd2);
	XXINTRNL_CREATE_CONSTRUCTOR(ranlux);
	XXINTRNL_CREATE_CONSTRUCTOR(ranlux389);
	XXINTRNL_CREATE_CONSTRUCTOR(cmrg);
	XXINTRNL_CREATE_CONSTRUCTOR(mrg);
	XXINTRNL_CREATE_CONSTRUCTOR(taus);
	XXINTRNL_CREATE_CONSTRUCTOR(taus2);
	XXINTRNL_CREATE_CONSTRUCTOR(gfsr4);
	
	XXINTRNL_CREATE_CONSTRUCTOR(rand);
	XXINTRNL_CREATE_CONSTRUCTOR(rand48);
	XXINTRNL_CREATE_CONSTRUCTOR(ranf);
	XXINTRNL_CREATE_CONSTRUCTOR(ranmar);
	XXINTRNL_CREATE_CONSTRUCTOR(r250);
	XXINTRNL_CREATE_CONSTRUCTOR(tt800);
	XXINTRNL_CREATE_CONSTRUCTOR(vax);
	XXINTRNL_CREATE_CONSTRUCTOR(transputer);
	XXINTRNL_CREATE_CONSTRUCTOR(randu);
	XXINTRNL_CREATE_CONSTRUCTOR(minstd);
	XXINTRNL_CREATE_CONSTRUCTOR(uni);
	XXINTRNL_CREATE_CONSTRUCTOR(uni32);
	XXINTRNL_CREATE_CONSTRUCTOR(slatec);
	XXINTRNL_CREATE_CONSTRUCTOR(zuf);
	XXINTRNL_CREATE_CONSTRUCTOR(knuthran2);
	XXINTRNL_CREATE_CONSTRUCTOR(knuthran2002);
	XXINTRNL_CREATE_CONSTRUCTOR(knuthran);
	XXINTRNL_CREATE_CONSTRUCTOR(borosh13);
	XXINTRNL_CREATE_CONSTRUCTOR(fishman18);
	XXINTRNL_CREATE_CONSTRUCTOR(fishman20);
	XXINTRNL_CREATE_CONSTRUCTOR(lecuyer21);
	XXINTRNL_CREATE_CONSTRUCTOR(waterman14);
	XXINTRNL_CREATE_CONSTRUCTOR(fishman2x);
	XXINTRNL_CREATE_CONSTRUCTOR(coveyou);
	
#define XXINTRNL_CREATE_UNIX_CONSTRUCTOR(vers) \
	RETRIEVE_TEMPORARY_CLASS(Random) Random::random_##vers(const unsigned bitlength) throw() { \
		const gsl_rng_type* type = gsl_rng_random_##vers; \
		switch (bitlength) { \
			case 8: type = gsl_rng_random8_##vers; break; \
			case 32: type = gsl_rng_random32_##vers; break; \
			case 64: type = gsl_rng_random64_##vers; break; \
			case 128: type = gsl_rng_random128_##vers; break; \
			case 256: type = gsl_rng_random256_##vers; break; \
			default: break; \
		} \
		return ::std::move(Random(type)); \
	}
	
	XXINTRNL_CREATE_UNIX_CONSTRUCTOR(bsd);
	XXINTRNL_CREATE_UNIX_CONSTRUCTOR(libc5);
	XXINTRNL_CREATE_UNIX_CONSTRUCTOR(glibc2);
	
#undef XXINTRNL_CREATE_UNIX_CONSTRUCTOR
#undef XXINTRNL_CREATE_CONSTRUCTOR	

}

#endif