/*

tempobj.hpp ... Dealing with temporary objects efficiently using move semantic. Supports C++03 and C++0x.

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
 \file tempobj.hpp
 \brief Dealing with temporary objects efficiently using move semantic. Supports C++03 and C++0x.
 \author KennyTM~
 \date June 12th, 2009
 
 Usage: In your class, do it like this:
 
 \code
 
 // In .h
 
 class String {
	 private:
		 int length;
		 char* string;
 		  
	 public:
		MEMORY_MANAGER_INTERFACE(String);		// <-- Use this to define the interface.
 
		String(const char* src); 
		String& operator+= (const String& other);
 };
 
 IMMEDIATE_OPERATOR_INTERFACE(String, +);		// <-- Use this to define an immediate operator, given the inplace one.
 
 // In .cpp
 
 MEMORY_MANAGER_IMPLEMENTATION(String);                // <-- This will automatically construct the copy/move constructors/assignment operators, and the destructor.
 
 IMPLEMENT_COPY_METHOD(String) {                       // <-- This is the core method which an object is deep-copied.
	 string = new char[other.length+1];                //      - You can assume the old data has been invalidated.
	 strncpy(string, other.string, other.length);      //      - Use "other" to refer to the object being copied.
	 length = other.length;
 }
 
 IMPLEMENT_MOVE_METHOD(String) {						// <-- This is the core statement which an object's ownership is transferred.
	 string = std::move(other.string);            	    //      - Always use std::move to move objects.
	 length = std::move(other.length);
 }
 
 IMPLEMENT_DEALLOC_METHOD(String) {						// <-- This is the core statement which an object is definitely need to be destroyed.
 	delete[] string;
 }
 
 String::String(const char* src) : length(strlen(src)) {
 	string = new char[length+1];
	strncpy(string, src, length);
 }
 
 String& String::operator+= (const String& other) { //... append the string ...// }
 IMMEDIATE_OPERATOR_IMPLEMENTATION(String, +);			// <-- Use this to implement an immediate operator.
  
 \endcode
 
 Note: these macros currently are *not* designed for polymorphism. Classes using
       these should *not* have any derived classes.
 
 */


#ifndef IGRAPH_TEMPOBJ_HPP
#define IGRAPH_TEMPOBJ_HPP

#if __GXX_EXPERIMENTAL_CXX0X__ || __cplusplus > 199711L
#include <memory>
#define XXINTRNL_PARAMTYPE(xx_typnm, ...) __VA_ARGS__&&		/// \internal // = TMPOBJ
#define XXINTRNL_MOVETYPE(xx_typnm, ...) __VA_ARGS__&&		/// \internal // = MMMOBJ
#define XXINTRNL_RVALTYPE(xx_typnm, ...) __VA_ARGS__		/// \internal // = IMMOBJ
#define XXINTRNL_RRP2MT(xx_typnm, ...)						/// \internal // = CAST_FOR_MMMOVE
#define TMPOBJ_INTERFACE				/// \internal
#define TMPOBJ_IMPLEMENTATION(template_decl, cls, ...)	/// \internal
#else
/// \internal
namespace igraph {
	struct GetTemporaryClassImpl_A {char x[256];};
	template <typename U> GetTemporaryClassImpl_A GetTemporaryClassImpl_B (const typename U::Temporary* p);
	template <typename U> char                    GetTemporaryClassImpl_B (...);

	template <typename T, int> struct GetTemporaryClassImpl         { typedef          T            v; };
	template <typename T>      struct GetTemporaryClassImpl<T, 256> { typedef typename T::Temporary v; };
	
	template <typename T> struct RvalueRefMoveType  { typedef       typename GetTemporaryClassImpl<T,sizeof(GetTemporaryClassImpl_B<T>(0))>::v& Type; };
}
#define XXINTRNL_PARAMTYPE(xx_typnm, ...) const xx_typnm __VA_ARGS__::Temporary&
#define XXINTRNL_MOVETYPE(xx_typnm, ...) xx_typnm __VA_ARGS__::Temporary&
#define XXINTRNL_RVALTYPE(xx_typnm, ...) xx_typnm __VA_ARGS__::Temporary
#define XXINTRNL_RRP2MT(xx_typnm, ...) const_cast<xx_typnm __VA_ARGS__::Temporary&>
/// \internal
namespace std {
	template <typename T>
	static inline typename ::igraph::RvalueRefMoveType<T>::Type move(T& a) { return (typename ::igraph::RvalueRefMoveType<T>::Type)(a); }
}
/// \internal
#define TMPOBJ_INTERFACE class Temporary; friend class Temporary
/// \internal
#define TMPOBJ_IMPLEMENTATION(template_decl, cls, ...)                          \
template_decl class cls __VA_ARGS__::Temporary : public cls __VA_ARGS__ {       \
public:                                                                         \
	Temporary(const cls __VA_ARGS__& other) : cls(other) {}                     \
}
#endif

/// Group a comma-separated list. Using this avoids the extra parenthesis.
#define GROUP(...) __VA_ARGS__

//------------------------------------------------------------------------------
#pragma mark -

/// \internal
namespace igraph {
	/// \internal
	struct XXINTRNL_DONT_DEALLOC {
		bool value;
		operator bool() { return value; }
		XXINTRNL_DONT_DEALLOC(bool v = false) : value(v) {}
	};
}

/// \internal
#define XXINTRNL_MEMORY_MANAGER_INTERFACE(xx_typnm, cls, ...)                   \
public:                                                                         \
	TMPOBJ_INTERFACE;                                                           \
private:                                                                        \
	igraph::XXINTRNL_DONT_DEALLOC mm_dont_dealloc;                              \
protected:                                                                      \
	void mm_raw_copy(const cls __VA_ARGS__& other);                             \
	void mm_raw_dealloc();                                                      \
	void mm_raw_move(XXINTRNL_MOVETYPE(xx_typnm, cls __VA_ARGS__) other);       \
	void mm_copy(const cls __VA_ARGS__& other) { mm_dont_dealloc = other.mm_dont_dealloc; mm_raw_copy(other); } \
	void mm_dealloc() { if (!mm_dont_dealloc) { mm_dont_dealloc = true; mm_raw_dealloc(); } } \
	void mm_move(XXINTRNL_MOVETYPE(xx_typnm, cls __VA_ARGS__) other);           \
public:                                                                         \
	cls(const cls __VA_ARGS__& other);                                          \
	cls(XXINTRNL_PARAMTYPE(xx_typnm, cls __VA_ARGS__) other);                   \
	~cls();                                                                     \
	cls __VA_ARGS__& operator=(const cls __VA_ARGS__& other);                   \
	cls __VA_ARGS__& operator=(XXINTRNL_PARAMTYPE(xx_typnm, cls __VA_ARGS__) other)

/// \internal
#define XXINTRNL_MEMORY_MANAGER_IMPLEMENTATION(template_decl, attrib, xx_typnm, cls, ...) \
TMPOBJ_IMPLEMENTATION(GROUP(template_decl), cls, __VA_ARGS__);                  \
template_decl attrib void cls __VA_ARGS__::mm_move(XXINTRNL_MOVETYPE(xx_typnm, cls __VA_ARGS__) other) { \
	mm_dont_dealloc = other.mm_dont_dealloc; mm_raw_move(other); other.mm_dont_dealloc = true; \
}                                                                               \
template_decl attrib cls __VA_ARGS__::cls(const cls __VA_ARGS__& other) {       \
	mm_copy(other);                                                             \
}                                                                               \
template_decl attrib cls __VA_ARGS__::cls(XXINTRNL_PARAMTYPE(xx_typnm, cls __VA_ARGS__) other) {\
	mm_move(XXINTRNL_RRP2MT(xx_typnm, cls __VA_ARGS__)(other));                 \
}                                                                               \
template_decl attrib cls __VA_ARGS__::~cls() { mm_dealloc(); }                  \
template_decl attrib cls __VA_ARGS__& cls __VA_ARGS__::operator=(const cls __VA_ARGS__& other) { \
	if (this != &other) {                                                       \
		mm_dealloc();                                                           \
		mm_copy(other);                                                         \
	}                                                                           \
	return *this;                                                               \
}                                                                               \
template_decl attrib cls __VA_ARGS__& cls __VA_ARGS__::operator=(XXINTRNL_PARAMTYPE(xx_typnm, cls __VA_ARGS__) other) { \
	if (this != &other) {                                                       \
		mm_dealloc();                                                           \
		mm_move(XXINTRNL_RRP2MT(xx_typnm, cls __VA_ARGS__)(other));             \
	}                                                                           \
	return *this;                                                               \
}

/// \internal
#define XXINTRNL_IMPLEMENT_COPY_METHOD(xx_typnm, other, cls, ...)    void cls __VA_ARGS__::mm_raw_copy(const cls __VA_ARGS__& other)
#define XXINTRNL_IMPLEMENT_DEALLOC_METHOD(xx_typnm, other, cls, ...) void cls __VA_ARGS__::mm_raw_dealloc()
#define XXINTRNL_IMPLEMENT_MOVE_METHOD(xx_typnm, other, cls, ...)    void cls __VA_ARGS__::mm_raw_move(XXINTRNL_MOVETYPE(xx_typnm, cls __VA_ARGS__) other)

/// \internal
#define XXINTRNL_IMMEDIATE_OPERATOR_INTERFACE_RHS(attrib, xx_typnm, cls, rhs_type) \
attrib XXINTRNL_RVALTYPE(xx_typnm, cls) operator op (const cls& self, rhs_type other); \
attrib XXINTRNL_PARAMTYPE(xx_typnm, cls) operator op (XXINTRNL_PARAMTYPE(xx_typnm, cls) self, rhs_type other)

/// \internal
#define XXINTRNL_IMMEDIATE_OPERATOR_INTERFACE_LHS(attrib, xx_typnm, cls, lhs_type) \
attrib XXINTRNL_RVALTYPE(xx_typnm, cls) operator op (lhs_type other, const cls& self); \
attrib XXINTRNL_PARAMTYPE(xx_typnm, cls) operator op (rhs_type other, XXINTRNL_PARAMTYPE(xx_typnm, cls) self)

/// \internal
#define XXINTRNL_IMMEDIATE_OPERATOR_IMPLEMENTATION_RHS(attrib, xx_typnm, cls, op, rhs_type) \
attrib XXINTRNL_RVALTYPE(xx_typnm, cls) operator op (const cls& xx_self, rhs_type other) { \
	XXINTRNL_RVALTYPE(xx_typnm, cls) self = xx_self;                            \
	self op##= other;                                                           \
	return ::std::move(self);                                                   \
}                                                                               \
attrib XXINTRNL_PARAMTYPE(xx_typnm, cls) operator op (XXINTRNL_PARAMTYPE(xx_typnm, cls) xx_self, rhs_type other) { \
	XXINTRNL_RRP2MT(xx_typnm, cls)(xx_self) op##= other;                        \
	return ::std::move(XXINTRNL_RRP2MT(xx_typnm, cls)(xx_self));                \
}

/// \internal
#define XXINTRNL_IMMEDIATE_OPERATOR_IMPLEMENTATION_LHS(attrib, xx_typnm, cls, op, lhs_type) \
attrib XXINTRNL_RVALTYPE(xx_typnm, cls) operator op (lhs_type other, const cls& xx_self) { \
	XXINTRNL_RVALTYPE(xx_typnm, cls) self = xx_self;                  \
	self op##= other;                                                           \
	return ::std::move(self);                                                   \
}                                                                               \
attrib XXINTRNL_PARAMTYPE(xx_typnm, cls) operator op (lhs_type other, XXINTRNL_PARAMTYPE(xx_typnm, cls) xx_self) { \
	XXINTRNL_RRP2MT(xx_typnm, cls)(xx_self) op##= other;                        \
	return ::std::move(XXINTRNL_RRP2MT(xx_typnm, cls)(xx_self));                \
}

//------------------------------------------------------------------------------
#pragma mark -

/**
 \brief Memory manager interface.
 
 Insert the interface for memory management (copy constructor, move constructor, destructor and = operators) for the specified class.
 
 Example:
 \code
 // for normal class
 public:
 MEMORY_MANAGER_INTERFACE(Bitmap);
 ...
 // for template class
 public:
 MEMORY_MANAGER_INTERFACE_WITH_TEMPLATE(HashTable, <K,V>);
 \endcode
 
 \param cls Class name
 \param ... Template parameters, if any. You must supply the angle brackets.
 */
#define MEMORY_MANAGER_INTERFACE_WITH_TEMPLATE(cls, ...) XXINTRNL_MEMORY_MANAGER_INTERFACE(typename, cls, __VA_ARGS__)
#define MEMORY_MANAGER_INTERFACE(cls)                    XXINTRNL_MEMORY_MANAGER_INTERFACE(        , cls)

/**
 \brief Memory manager implementation with template and attributes.
 
 Insert the implementations for memory management (copy constructor, move constructor, destructor and = operators) for the specified template class.
 
 Example:
 \code
 MEMORY_MANAGER_IMPLEMENTATION_ATTR_WITH_TEMPLATE(
	GROUP(template<typename K, typename V>), , HashTable, <K,V>
 );
 \endcode 
 
 \param template_decl Template declaration. If there is more than 1 arguments, use the GROUP() macro to group them.
 \param attrib Attribute
 \param cls    Class name
 \param template_param Template parameters. You must supply the angle brackets.
 */ 
#define MEMORY_MANAGER_IMPLEMENTATION_ATTR_WITH_TEMPLATE(template_decl, attrib, cls, ...) XXINTRNL_MEMORY_MANAGER_IMPLEMENTATION(GROUP(template_decl), attrib, typename, cls, __VA_ARGS__)

/**
 \brief Memory manager implementation with attributes.
 
 Insert the implementations for memory management (copy constructor, move constructor, destructor and = operators) for the specified class.

 \param attrib Attribute
 \param cls    Class name
  */
#define MEMORY_MANAGER_IMPLEMENTATION_ATTR(attrib, cls) XXINTRNL_MEMORY_MANAGER_IMPLEMENTATION(, attrib, , cls)

/**
 \brief Memory manager implementation.
 
 Insert the implementations for memory management (copy constructor, move constructor, destructor and = operators) for the specified class.
 
 \param cls    Class name
 */
#define MEMORY_MANAGER_IMPLEMENTATION(cls) MEMORY_MANAGER_IMPLEMENTATION_ATTR(, cls)

/// TODO: Documentation
#define IMPLEMENT_COPY_METHOD_WITH_TEMPLATE_AND_CUSTOM_ARG_NAME(argname, cls, ...) XXINTRNL_IMPLEMENT_COPY_METHOD(typename, argname, cls, __VA_ARGS__)
#define IMPLEMENT_COPY_METHOD_WITH_TEMPLATE(cls, ...)                              XXINTRNL_IMPLEMENT_COPY_METHOD(typename, other, cls, __VA_ARGS__)
#define IMPLEMENT_COPY_METHOD_WITH_CUSTOM_ARG_NAME(argname, cls)                   XXINTRNL_IMPLEMENT_COPY_METHOD( , argname, cls)
#define IMPLEMENT_COPY_METHOD(cls)                                                 XXINTRNL_IMPLEMENT_COPY_METHOD( , other, cls)
#define IMPLEMENT_DEALLOC_METHOD_WITH_TEMPLATE_AND_CUSTOM_ARG_NAME(argname, cls, ...) XXINTRNL_IMPLEMENT_DEALLOC_METHOD(typename, argname, cls, __VA_ARGS__)
#define IMPLEMENT_DEALLOC_METHOD_WITH_TEMPLATE(cls, ...)                              XXINTRNL_IMPLEMENT_DEALLOC_METHOD(typename, other, cls, __VA_ARGS__)
#define IMPLEMENT_DEALLOC_METHOD_WITH_CUSTOM_ARG_NAME(argname, cls)                   XXINTRNL_IMPLEMENT_DEALLOC_METHOD(, argname, cls)
#define IMPLEMENT_DEALLOC_METHOD(cls)                                                 XXINTRNL_IMPLEMENT_DEALLOC_METHOD(, other, cls)
#define IMPLEMENT_MOVE_METHOD_WITH_TEMPLATE_AND_CUSTOM_ARG_NAME(argname, cls, ...) XXINTRNL_IMPLEMENT_MOVE_METHOD(typename, argname, cls, __VA_ARGS__)
#define IMPLEMENT_MOVE_METHOD_WITH_TEMPLATE(cls, ...)                              XXINTRNL_IMPLEMENT_MOVE_METHOD(typename, other, cls, __VA_ARGS__)
#define IMPLEMENT_MOVE_METHOD_WITH_CUSTOM_ARG_NAME(argname, cls)                   XXINTRNL_IMPLEMENT_MOVE_METHOD( , argname, cls)
#define IMPLEMENT_MOVE_METHOD(cls)                                                 XXINTRNL_IMPLEMENT_MOVE_METHOD( , other, cls)

#pragma mark -

/**
 \brief Operator overloading interface that returns a temporary object with custom RHS type, using templates.
 
 Insert the interface for efficient operator overloading that requires returning a temporary object without modifying the input parameters.
 Using this assumes an efficient in-place assignment operator (+=, -=, etc.) has been defined.
 
 Note: Place this outside the class definition.
 
 \param template_decl_and_attrib Template declaration and attribute. If there is more than 1 arguments, use the GROUP() macro to group them.
 \param cls      Class name, with template parameters (e.g. Vector<T>, GROUP(HashTable<K,V>))
 \param op       Operator (e.g. +, -, *, /)
 \param rhs_type Type of the RHS value.
 */
#define IMMEDIATE_OPERATOR_INTERFACE_RHS_ATTR_WITH_TEMPLATE(template_decl_and_attrib, cls, op, rhs_type) XXINTRNL_IMMEDIATE_OPERATOR_INTERFACE_RHS(GROUP(template_decl_and_attrib), typename, GROUP(cls), op, GROUP(rhs_type))

/**
 \brief Operator overloading interface that returns a temporary object with custom RHS type.
 
 Insert the interface for efficient operator overloading that requires returning a temporary object without modifying the input parameters.
 Using this assumes an efficient in-place assignment operator (+=, -=, etc.) has been defined.
 
 Note: Place this outside the class definition.
 
 \param attrib   Attribute
 \param cls      Class name
 \param op       Operator (e.g. +, -, *, /)
 \param rhs_type Type of the RHS value.
 */
#define IMMEDIATE_OPERATOR_INTERFACE_RHS_ATTR(attrib, cls, op, rhs_type) XXINTRNL_IMMEDIATE_OPERATOR_INTERFACE_RHS(attrib, , cls, op, rhs_type)

/**
 \brief Operator overloading interface that returns a temporary object using templates.
 
 Insert the interface for efficient operator overloading that requires returning a temporary object without modifying the input parameters.
 
 Note: Place this outside the class definition.
 
 \param template_decl_and_attrib Template declaration and attribute. If there is more than 1 arguments, use the GROUP() macro to group them.
 \param cls      Class name, with template parameters (e.g. Vector<T>, GROUP(HashTable<K,V>))
 \param op       Operator (e.g. +, -, *, /)
 */
#define IMMEDIATE_OPERATOR_INTERFACE_ATTR_WITH_TEMPLATE(template_decl_and_attrib, cls, op) XXINTRNL_IMMEDIATE_OPERATOR_INTERFACE_RHS(GROUP(template_decl_and_attrib), typename, GROUP(cls), op, GROUP(const cls&))

/**
 \brief Operator overloading interface that returns a temporary object.
 
 Insert the interface for efficient operator overloading that requires returning a temporary object without modifying the input parameters.
 
 Note: Place this outside the class definition.
 
 \param attrib   Attribute
 \param cls      Class name
 \param op       Operator (e.g. +, -, *, /)
 */
#define IMMEDIATE_OPERATOR_INTERFACE_ATTR(attrib, cls, op) IMMEDIATE_OPERATOR_INTERFACE_RHS_ATTR(attrib, cls, op, const cls&)

/**
 \brief Operator overloading interface that returns a temporary object.
 
 Insert the interface for efficient operator overloading that requires returning a temporary object without modifying the input parameters.
 
 Note: Place this outside the class definition.
 
 \param cls      Class name
 \param op       Operator (e.g. +, -, *, /)
 */
#define IMMEDIATE_OPERATOR_INTERFACE(cls, op) IMMEDIATE_OPERATOR_INTERFACE_ATTR(, cls, op)

/**
 \brief Operator overloading interface that returns a temporary object for a commutative operator with custom RHS type and templates
 
 Insert the interface for efficient operator overloading that requires returning a temporary object without modifying the input parameters.
 
 Note: Place this outside the class definition.
 
 \param template_decl_and_attrib Template declaration and attribute. If there is more than 1 arguments, use the GROUP() macro to group them.
 \param cls      Class name, with template parameters (e.g. Vector<T>, GROUP(HashTable<K,V>))
 \param op       Operator (e.g. +, -, *, /)
 \param rhs_type Type of the other value.
 */
#define IMMEDIATE_OPERATOR_INTERFACE_COMMUTATIVE_ATTR_WITH_TEMPLATE(template_decl_and_attrib, cls, op, rhs_type) \
	XXINTRNL_IMMEDIATE_OPERATOR_INTERFACE_RHS(GROUP(template_decl_and_attrib), typename, GROUP(cls), op, GROUP(rhs_type)); \
	XXINTRNL_IMMEDIATE_OPERATOR_INTERFACE_LHS(GROUP(template_decl_and_attrib), typename, GROUP(cls), op, GROUP(rhs_type))

/**
 \brief Operator overloading interface that returns a temporary object for a commutative operator with custom RHS type.
 
 Insert the interface for efficient operator overloading that requires returning a temporary object without modifying the input parameters.
 
 Note: Place this outside the class definition.
 
 \param attrib   Attribute
 \param cls      Class name
 \param op       Operator (e.g. +, -, *, /)
 \param rhs_type Type of the other value.
 */
#define IMMEDIATE_OPERATOR_INTERFACE_COMMUTATIVE_ATTR(attrib, cls, op, rhs_type) \
	XXINTRNL_IMMEDIATE_OPERATOR_INTERFACE_RHS(attrib, , cls, op, rhs_type); \
	XXINTRNL_IMMEDIATE_OPERATOR_INTERFACE_LHS(attrib, , cls, op, rhs_type)

/// TODO: Documentation.
#define IMMEDIATE_OPERATOR_INTERFACE_COMMUTATIVE(cls, op, rhs_type) IMMEDIATE_OPERATOR_INTERFACE_COMMUTATIVE_ATTR(, cls, op, rhs_type);

#define IMMEDIATE_OPERATOR_IMPLEMENTATION_RHS_ATTR_WITH_TEMPLATE(template_decl_and_attrib, cls, op, rhs_type) XXINTRNL_IMMEDIATE_OPERATOR_IMPLEMENTATION_RHS(GROUP(template_decl_and_attrib), typename, GROUP(cls), op, GROUP(rhs_type))
#define IMMEDIATE_OPERATOR_IMPLEMENTATION_RHS_ATTR(attrib, cls, op, rhs_type) XXINTRNL_IMMEDIATE_OPERATOR_IMPLEMENTATION_RHS(attrib, , cls, op, rhs_type)
#define IMMEDIATE_OPERATOR_IMPLEMENTATION_ATTR_WITH_TEMPLATE(template_decl_and_attrib, cls, op) XXINTRNL_IMMEDIATE_OPERATOR_IMPLEMENTATION_RHS(GROUP(template_decl_and_attrib), typename, GROUP(cls), op, GROUP(const cls&))
#define IMMEDIATE_OPERATOR_IMPLEMENTATION_ATTR(attrib, cls, op) IMMEDIATE_OPERATOR_IMPLEMENTATION_RHS_ATTR(attrib, cls, op, const cls&)
#define IMMEDIATE_OPERATOR_IMPLEMENTATION(cls, op) IMMEDIATE_OPERATOR_IMPLEMENTATION_ATTR(, cls, op)
#define IMMEDIATE_OPERATOR_IMPLEMENTATION_COMMUTATIVE_ATTR_WITH_TEMPLATE(template_decl_and_attrib, cls, op, rhs_type) \
	XXINTRNL_IMMEDIATE_OPERATOR_IMPLEMENTATION_RHS(GROUP(template_decl_and_attrib), typename, GROUP(cls), op, GROUP(rhs_type)); \
	XXINTRNL_IMMEDIATE_OPERATOR_IMPLEMENTATION_LHS(GROUP(template_decl_and_attrib), typename, GROUP(cls), op, GROUP(rhs_type))
#define IMMEDIATE_OPERATOR_IMPLEMENTATION_COMMUTATIVE_ATTR(attrib, cls, op, rhs_type) \
	XXINTRNL_IMMEDIATE_OPERATOR_IMPLEMENTATION_RHS(attrib, , cls, op, rhs_type); \
	XXINTRNL_IMMEDIATE_OPERATOR_IMPLEMENTATION_LHS(attrib, , cls, op, rhs_type)
#define IMMEDIATE_OPERATOR_IMPLEMENTATION_COMMUTATIVE(cls, op, rhs_type) IMMEDIATE_OPERATOR_IMPLEMENTATION_COMMUTATIVE_ATTR(, cls, op, rhs_type);


namespace igraph {
	/**
	 \enum OwnershipTransfer
	 \brief How ownership of an unmanaged object should be transferred.
	 
	 This enum is used in custom code when the ownership of an object is to be transferred but cannot tracked by itself.
	 */
	enum OwnershipTransfer {
		/// Don't transfer the transfer to the recipient. 
		/// The recipient's lifetime must be *shorter* than the previous owner's lifetime in order to use this mode.
		OwnershipTransferKeepOriginal,
		/// Move the ownership to the recipient.
		/// The previous owner must cease to be referred immediately after using this mode.
		OwnershipTransferMove,
		/// Copy the object to the recipient, therefore ensuring both the previous owner and the recipient have ownership to the same value (but different object).
		OwnershipTransferCopy,
	};
}

/**
 \brief Common initialization for memory manager with ownership transfer
 */
#define COMMON_INIT_WITH(transfer_mode) mm_dont_dealloc(transfer_mode != ::igraph::OwnershipTransferKeepOriginal)

#endif
