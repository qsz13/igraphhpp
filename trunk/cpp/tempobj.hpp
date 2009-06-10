/*

tempobj.hpp ... Dealing with temporary objects efficiently using move semantic.

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
 Usage: In your class, do it like this:
 @code
 
 // In .h
 
 class SomeWrapper {
	 private:
		 expensive_type_t someExpensiveObject;
		 MEMORY_MANAGER_INTERFACE(SomeWrapper);		// <-- Use this to define the interface.
		  
	 public:
		SomeWrapper(int x, int y, int z); 
		SomeWrapper& operator+= (const SomeWrapper& other);
 };
 
 IMMEDIATE_OPERATOR_INTERFACE(SomeWrapper, +);		// <-- Use this to define an immediate operator, given the inplace one.
 
 // In .cpp
 
 MEMORY_MANAGER_IMPLEMENTATION(SomeWrapper,						// <-- This will automatically construct the copy/move constructors/assignment operators, and the destructor.
	 someExpensiveObject.copyFrom(other.someExpensiveObject),	// <-- This is the core statement which an object is deep-copied. 
	 someExpensiveObject = STD_MOVE(other.someExpensiveObject),	// <-- This is the core statement which an object's ownership is transferred. Always use STD_MOVE to move objects.
	 someExpensiveObject.destroy()								// <-- This is the core statement which an object is definitely need to be destroyed.
 );
 
 SomeWrapper::SomeWrapper(int x, int y, int z) : COMMON_INIT { 	// <-- Always add this to every constructor initialization list!
	 someExpensiveObject.assign(x, y, z);
 }
 
 SomeWrapper& SomeWrapper::operator+= (const SomeWrapper& other) {
	 someExpensiveObject.increase(other.someExpensiveObject);
	 return *this;
 }
 
 IMMEDIATE_OPERATOR_IMPLEMENTATION(SomeWrapper, +);				// <-- Use this the implement the immediate operator.
 
 @endcode
 
 Note: these macros currently are *not* designed for polymorphism. Classes using
       these should *not* have any derived classes.
 
 */


#ifndef IGRAPH_TEMPOBJ_HPP
#define IGRAPH_TEMPOBJ_HPP

#if __GXX_EXPERIMENTAL_CXX0X__ || __cplusplus > 199711L
#include <memory>

#define STD_MOVE std::move

#define MEMORY_MANAGER_INTERFACE(cls)                                           \
private:                                                                        \
	bool mm_dont_dealloc;                                                       \
	void mm_copy(const cls& other);                                             \
	void mm_dealloc();                                                          \
	void mm_move(cls&& other);                                                  \
public:                                                                         \
	cls(const cls& other);                                                      \
	cls(cls&& other);                                                           \
	~cls();                                                                     \
	cls& operator=(const cls& other);                                           \
	cls& operator=(cls&& other)

#define MEMORY_MANAGER_IMPLEMENTATION_ATTR(attrib, cls, copy_statement, move_statement, destroy_statement) \
attrib void cls::mm_copy(const cls& other) {                                    \
	mm_dont_dealloc = other.mm_dont_dealloc;                                    \
	copy_statement;                                                             \
}                                                                               \
attrib void cls::mm_dealloc() {                                                 \
	if (!mm_dont_dealloc) {                                                     \
		mm_dont_dealloc = true;                                                 \
		destroy_statement;                                                      \
	}                                                                           \
}                                                                               \
attrib void cls::mm_move(cls&& other) {                                         \
	mm_dont_dealloc = other.mm_dont_dealloc;                                    \
	move_statement;                                                             \
	other.mm_dont_dealloc = true;                                               \
}                                                                               \
attrib cls::cls(const cls& other) { mm_copy(other); }                           \
attrib cls::cls(cls&& other) { mm_move(other); }                                \
attrib cls::~cls() { mm_dealloc(); }                                            \
attrib cls& cls::operator=(const cls& other) {                                  \
	if (this != &other) {                                                       \
		mm_dealloc();                                                           \
		mm_copy(other);                                                         \
	}                                                                           \
	return *this;                                                               \
}                                                                               \
attrib cls& cls::operator=(cls&& other) {                                       \
	if (this != &other) {                                                       \
		mm_dealloc();                                                           \
		mm_move(other);                                                         \
	}                                                                           \
	return *this;                                                               \
}

#define MEMORY_MANAGER_IMPLEMENTATION(cls, copy_statement, move_statement, destroy_statement) MEMORY_MANAGER_IMPLEMENTATION_ATTR(, cls, copy_statement, move_statement, destroy_statement)


#define IMMEDIATE_OPERATOR_INTERFACE(cls, op)                                   \
cls operator op (const cls& self, const cls& other);                            \
cls&& operator op (cls&& self, const cls& other)

#define IMMEDIATE_OPERATOR_IMPLEMENTATION_ATTR(attrib, cls, op)                 \
attrib cls operator op (const cls& self, const cls& other) {                    \
	cls copy = self;                                                            \
	return std::move(copy op##= other);                                         \
}                                                                               \
attrib cls&& operator op (cls&& self, const cls& other) {                       \
	return std::move(self op##= other);                                         \
}
#define IMMEDIATE_OPERATOR_IMPLEMENTATION(cls, op) IMMEDIATE_OPERATOR_IMPLEMENTATION_ATTR(, cls, op)

#define COMMON_INIT mm_dont_dealloc(false)


#else

// TODO

#endif

#endif
