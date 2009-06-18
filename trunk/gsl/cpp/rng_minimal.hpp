/*

rng_minimal.hpp ... Minimal interface to support gsl::Random without installing GSL.

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

#ifdef GSL_VAR

#include <gsl/cpp/rng.hpp>

#else

#ifndef GSL_RNG_MINIMAL_HPP
#define GSL_RNG_MINIMAL_HPP

#include <tempobj.hpp>
#include <cstdlib>
#include <ctime>
#include <algorithm>

struct gsl_rng_type;

namespace gsl {
	
	class Random {
	public:
		MEMORY_MANAGER_INTERFACE(Random);
	
#pragma mark -
#pragma mark Random number generator initialization
		Random(const gsl_rng_type* type) {}
		const Random& set(unsigned long s = 0) const throw() { ::std::srand(s); return *this; }
		const Random& seed() const throw() { ::std::srand(::std::time(0)); return *this; }
		
#pragma mark -
#pragma mark Sampling from a random number generator
		unsigned long get() const throw() { return static_cast<unsigned long>(::std::rand()); }
		double uniform() const throw() { return static_cast<double>(::std::rand()) / RAND_MAX; }
		double uniform_pos() const throw() {
			unsigned long res;
			do { res = get(); } while (res == 0);
			return static_cast<double>(res) / RAND_MAX;
		}
		unsigned long uniform_int(unsigned long modulo) const throw() {
			unsigned long scale = RAND_MAX / modulo, res;
			do { res = get()/scale; } while (res >= modulo);
			return res;
		}
		
#pragma mark -
#pragma mark Auxiliary random number generator functions
		const char* name() const throw() { return "rand"; }
		unsigned long max() const throw() { return RAND_MAX; }
		unsigned long min() const throw() { return 0; }
		
		void* state() const throw() { return NULL; }
		size_t size() const throw() { return 0; }
		
		static const gsl_rng_type** types_setup() throw() { return NULL; }
		
#pragma mark -
#pragma mark Random number environment variables
		static const gsl_rng_type* env_setup() throw() { return NULL; }
		static const gsl_rng_type* default_type() throw() { return NULL; }
		static const unsigned long default_seed() throw() { return 0; }
		
#pragma mark -
#pragma mark Reading and writing random number generator state
		int fread(...) throw() { return 0; } 
		int fwrite(...) const throw() { return 0; }
		
#pragma mark -
#pragma mark Random number generator algorithms
		static RETRIEVE_TEMPORARY_CLASS(Random) default_generator() throw();
		static RETRIEVE_TEMPORARY_CLASS(Random) rand() throw();

#pragma mark -
#pragma mark Shuffling and Sampling
				
		template <typename T>
		void shuffle(T* arr, const size_t count) const throw() {
			for (size_t i = count - 1; i > 0; --i) {
				size_t j = uniform_int(i+1);
				if (i != j)
					::std::swap(arr[i], arr[j]);
			}
		}
		
		template <typename T>
		T* choose(T* store, const size_t how_many, T* source, const size_t count) const {
			for (size_t i = 0, j = 0; i < count && j < how_many; i++)
				if (uniform_int(count-i) < how_many-j)
					store[j++] = source[i];
			return store;
		}
		
		template <typename T>
		T* choose(const size_t how_many, T* source, const size_t count) const { return choose(new T[how_many], how_many, source, count); }
		
		template <typename T>
		T* sample(T* store, const size_t how_many, T* source, const size_t count) const {
			for (size_t j = 0; j < how_many; ++ j)
				store[j] = source[uniform_int(count)];
			return store;
		}
		
		template <typename T>
		T* sample(const size_t how_many, T* source, const size_t count) const { return sample(new T[how_many], how_many, source, count); }
				
		template <typename T>
		friend class IRandomDistribution;
	};
	MEMORY_MANAGER_INTERFACE_EX(Random);
	
	MEMORY_MANAGER_IMPLEMENTATION(Random);
	IMPLEMENT_COPY_METHOD(Random) {}
	IMPLEMENT_MOVE_METHOD(Random) {}
	IMPLEMENT_DEALLOC_METHOD(Random) {}
	
	RETRIEVE_TEMPORARY_CLASS(Random) Random::default_generator() throw() { return FORCE_STD_MOVE(Random)(Random(NULL)); }
	RETRIEVE_TEMPORARY_CLASS(Random) Random::rand() throw() { return FORCE_STD_MOVE(Random)(Random(NULL)); }
}

#endif

#endif