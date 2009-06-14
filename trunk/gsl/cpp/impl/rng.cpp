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
	
	IMPLEMENT_COPY_METHOD(Random) throw() {
		_ = gsl_rng_clone(other._);
	}
	IMPLEMENT_MOVE_METHOD(Random) throw() {
		_ = ::std::move(other._);
	}
	IMPLEMENT_DEALLOC_METHOD(Random) {
		gsl_rng_free(_);
	}

	const Random& Random::seed() const throw() { return set(::std::time(NULL)); }
	
#pragma mark -
#pragma mark Random number generator algorithms
	
	::tempobj::temporary_class<Random>::type Random::default_generator() throw() {
		gsl_rng* _ = gsl_rng_alloc(gsl_rng_default);
		gsl_rng_set(_, gsl_rng_default_seed);
		return ::std::move(Random(&_, ::tempobj::OwnershipTransferMove));
	}
	
#define CREATE_CONSTRUCTOR(xxname) ::tempobj::temporary_class<Random>::type Random::xxname() throw() { return ::std::move(Random(gsl_rng_##xxname)); }
	
	CREATE_CONSTRUCTOR(mt19937);
	CREATE_CONSTRUCTOR(ranlxs0);
	CREATE_CONSTRUCTOR(ranlxs1);
	CREATE_CONSTRUCTOR(ranlxs2);
	CREATE_CONSTRUCTOR(ranlxd1);
	CREATE_CONSTRUCTOR(ranlxd2);
	CREATE_CONSTRUCTOR(ranlux);
	CREATE_CONSTRUCTOR(ranlux389);
	CREATE_CONSTRUCTOR(cmrg);
	CREATE_CONSTRUCTOR(mrg);
	CREATE_CONSTRUCTOR(taus);
	CREATE_CONSTRUCTOR(taus2);
	CREATE_CONSTRUCTOR(gfsr4);
	
	CREATE_CONSTRUCTOR(rand);
	CREATE_CONSTRUCTOR(rand48);
	CREATE_CONSTRUCTOR(ranf);
	CREATE_CONSTRUCTOR(ranmar);
	CREATE_CONSTRUCTOR(r250);
	CREATE_CONSTRUCTOR(tt800);
	CREATE_CONSTRUCTOR(vax);
	CREATE_CONSTRUCTOR(transputer);
	CREATE_CONSTRUCTOR(randu);
	CREATE_CONSTRUCTOR(minstd);
	CREATE_CONSTRUCTOR(uni);
	CREATE_CONSTRUCTOR(uni32);
	CREATE_CONSTRUCTOR(slatec);
	CREATE_CONSTRUCTOR(zuf);
	CREATE_CONSTRUCTOR(knuthran2);
	CREATE_CONSTRUCTOR(knuthran2002);
	CREATE_CONSTRUCTOR(knuthran);
	CREATE_CONSTRUCTOR(borosh13);
	CREATE_CONSTRUCTOR(fishman18);
	CREATE_CONSTRUCTOR(fishman20);
	CREATE_CONSTRUCTOR(lecuyer21);
	CREATE_CONSTRUCTOR(waterman14);
	CREATE_CONSTRUCTOR(fishman2x);
	CREATE_CONSTRUCTOR(coveyou);
	
#define CREATE_UNIX_CONSTRUCTOR(vers) \
	::tempobj::temporary_class<Random>::type Random::random_##vers(const unsigned bitlength) throw() { \
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
	
	CREATE_UNIX_CONSTRUCTOR(bsd);
	CREATE_UNIX_CONSTRUCTOR(libc5);
	CREATE_UNIX_CONSTRUCTOR(glibc2);
	
#undef CREATE_UNIX_CONSTRUCTOR
#undef CREATE_CONSTRUCTOR	

}

#endif