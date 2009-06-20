/*

rng.hpp ... GSL Random number generator C++ wrapper

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

#ifndef GSL_RNG_HPP
#define GSL_RNG_HPP

#include <tempobj.hpp>
#include <gsl/gsl_rng.h>

namespace gsl {
	
	class Random {
	private:
		gsl_rng* _;
		
	public:
		MEMORY_MANAGER_INTERFACE(Random);
		XXINTRNL_WRAPPER_CONSTRUCTOR_INTERFACE(Random, gsl_rng*);
		
#pragma mark -
#pragma mark Random number generator initialization
		
		Random(const gsl_rng_type* type) throw() : _(gsl_rng_alloc(type)) {}
		const Random& set(unsigned long s = 0) const throw() {
			gsl_rng_set(_, s);
			return *this;
		}
		const Random& seed() const throw();
		
#pragma mark -
#pragma mark Sampling from a random number generator
		
		unsigned long get() const throw() { return gsl_rng_get(_); }
		double uniform() const throw() { return gsl_rng_uniform(_); }
		double uniform_pos() const throw() { return gsl_rng_uniform_pos(_); }
		unsigned long uniform_int(unsigned long modulo) const throw() { return gsl_rng_uniform_int(_, modulo); }
		
#pragma mark -
#pragma mark Auxiliary random number generator functions
		
		const char* name() const throw() { return gsl_rng_name(_); }
		unsigned long max() const throw() { return gsl_rng_max(_); }
		unsigned long min() const throw() { return gsl_rng_min(_); }
		
		void* state() const throw() { return gsl_rng_state(_); }
		size_t size() const throw() { return gsl_rng_size(_); }
		
		static const gsl_rng_type** types_setup() throw() { return gsl_rng_types_setup(); }
		
#pragma mark -
#pragma mark Random number environment variables
		
		static const gsl_rng_type* env_setup() throw() { return gsl_rng_env_setup(); }
		static const gsl_rng_type* default_type() throw() { return gsl_rng_default; }
		static const unsigned long default_seed() throw() { return gsl_rng_default_seed; }
		
#pragma mark -
#pragma mark Reading and writing random number generator state
		
		int fread(FILE* stream) throw() { return gsl_rng_fread(stream, _); } 
		int fwrite(FILE* stream) const throw() { return gsl_rng_fwrite(stream, _); }
		
#pragma mark -
#pragma mark Random number generator algorithms
		
		/// Call Random::env_setup() once (e.g. in main()) before using this generator.
		static ::tempobj::force_temporary_class<Random>::type default_generator() throw();
		
		static ::tempobj::force_temporary_class<Random>::type mt19937() throw();
		static ::tempobj::force_temporary_class<Random>::type ranlxs0() throw();
		static ::tempobj::force_temporary_class<Random>::type ranlxs1() throw();
		static ::tempobj::force_temporary_class<Random>::type ranlxs2() throw();
		static ::tempobj::force_temporary_class<Random>::type ranlxd1() throw();
		static ::tempobj::force_temporary_class<Random>::type ranlxd2() throw();
		static ::tempobj::force_temporary_class<Random>::type ranlux() throw();
		static ::tempobj::force_temporary_class<Random>::type ranlux389() throw();
		static ::tempobj::force_temporary_class<Random>::type cmrg() throw();
		static ::tempobj::force_temporary_class<Random>::type mrg() throw();
		static ::tempobj::force_temporary_class<Random>::type taus() throw();
		static ::tempobj::force_temporary_class<Random>::type taus2() throw();
		static ::tempobj::force_temporary_class<Random>::type gfsr4() throw();
		
#pragma mark -
#pragma mark Unix random number generators
		
		static ::tempobj::force_temporary_class<Random>::type rand() throw();
		static ::tempobj::force_temporary_class<Random>::type random_bsd(const unsigned bitlength = 0) throw();
		static ::tempobj::force_temporary_class<Random>::type random_libc5(const unsigned bitlength = 0) throw();
		static ::tempobj::force_temporary_class<Random>::type random_glibc2(const unsigned bitlength = 0) throw();
		static ::tempobj::force_temporary_class<Random>::type rand48() throw();
		
#pragma mark -
#pragma mark Other random number generators
		
		static ::tempobj::force_temporary_class<Random>::type ranf() throw();
		static ::tempobj::force_temporary_class<Random>::type ranmar() throw();
		static ::tempobj::force_temporary_class<Random>::type r250() throw();
		static ::tempobj::force_temporary_class<Random>::type tt800() throw();
		static ::tempobj::force_temporary_class<Random>::type vax() throw();
		static ::tempobj::force_temporary_class<Random>::type transputer() throw();
		static ::tempobj::force_temporary_class<Random>::type randu() throw();
		static ::tempobj::force_temporary_class<Random>::type minstd() throw();
		static ::tempobj::force_temporary_class<Random>::type uni() throw();
		static ::tempobj::force_temporary_class<Random>::type uni32() throw();
		static ::tempobj::force_temporary_class<Random>::type slatec() throw();
		static ::tempobj::force_temporary_class<Random>::type zuf() throw();
		static ::tempobj::force_temporary_class<Random>::type knuthran2() throw();
		static ::tempobj::force_temporary_class<Random>::type knuthran2002() throw();
		static ::tempobj::force_temporary_class<Random>::type knuthran() throw();
		static ::tempobj::force_temporary_class<Random>::type borosh13() throw();
		static ::tempobj::force_temporary_class<Random>::type fishman18() throw();
		static ::tempobj::force_temporary_class<Random>::type fishman20() throw();
		static ::tempobj::force_temporary_class<Random>::type lecuyer21() throw();
		static ::tempobj::force_temporary_class<Random>::type waterman14() throw();		
		static ::tempobj::force_temporary_class<Random>::type fishman2x() throw();
		static ::tempobj::force_temporary_class<Random>::type coveyou() throw();
		
#pragma mark -
#pragma mark Shuffling and Sampling
		
		template <typename T>
		void shuffle(T* arr, const size_t count) const throw() { gsl_ran_shuffle(_, arr, count, sizeof(T)); }
		
		template <typename T>
		T* choose(T* store, const size_t how_many, T* source, const size_t count) const { gsl_ran_choose(_, store, how_many, source, count, sizeof(T)); return store; }
		
		template <typename T>
		T* choose(const size_t how_many, T* source, const size_t count) const { return choose(new T[how_many], how_many, source, count); }

		template <typename T>
		T* sample(T* store, const size_t how_many, T* source, const size_t count) const { gsl_ran_sample(_, store, how_many, source, count, sizeof(T)); return store; }
		
		template <typename T>
		T* sample(const size_t how_many, T* source, const size_t count) const { return sample(new T[how_many], how_many, source, count); }
				
		template <typename T>
		friend class IRandomDistribution;
	};
	MEMORY_MANAGER_INTERFACE_EX(Random);
}

#endif