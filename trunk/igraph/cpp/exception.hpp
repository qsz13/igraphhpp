/*

exception.hpp ... igraph exceptions

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
 Usage: 
 
 On igraph functions that may return an int for error code, wrap the TRY(...)
 macro around it. This will convert the error code to an igraph::Exception in
 debug mode.
 
 */

#ifndef IGRAPH_EXCEPTIONS_HPP
#define IGRAPH_EXCEPTIONS_HPP

#include <igraph/cpp/common.hpp>
#include <exception>

#if IGRAPH_DEBUG
#define TRY(func) { int errcode = (func); if (errcode != IGRAPH_SUCCESS) throw Exception(errcode); }
#define MAY_THROW_EXCEPTION throw(Exception)
#else
#define TRY(func) func
#define MAY_THROW_EXCEPTION throw()
#endif

namespace igraph {
	
	class Exception : public std::exception {
		
	private:
		int errcode;
		
	public:
		virtual const char* what() const throw() { return igraph_strerror(errcode); }
		Exception (const int code) throw() : errcode(code) {};
	};
}

#endif
