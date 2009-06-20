/*

common.hpp ... Common definitions for igraph.hpp

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
 \file common.hpp
 \brief Common definitions for igraph.hpp
 \author KennyTM~
 \date June 12, 2009
 */

#ifndef IGRAPH_COMMON_HPP
#define IGRAPH_COMMON_HPP

#ifndef IGRAPH_DEBUG
#define IGRAPH_DEBUG _DEBUG || !__OPTIMIZE__
#endif

#if !(__GNUC__ >= 3)
#define __attribute__(x)
#endif

#if __GXX_EXPERIMENTAL_CXX0X__ || __cplusplus > 199711L
#define XXINTRNL_CXX0X 1
#else
#define XXINTRNL_CXX0X 0
#endif

#include <igraph/igraph.h>
#include <cstdio>

namespace igraph {
	
	// typedefs to clarify the role of each types
	/// Generic real value.
	typedef igraph_real_t Real;
	/// Generic integer.
	typedef igraph_integer_t Integer;
	/// Vertex ID.
	typedef igraph_integer_t Vertex;
	/// Edge ID.
	typedef igraph_integer_t Edge;
	/// Generic igraph boolean.
	typedef igraph_bool_t Boolean;
		
	// enums to prettify the constants.
	
	/**
	 \enum Directedness
	 \brief Directedness of graph.
	 Whether to treat the graph as directed or undirected.
	 */
	enum Directedness {
		Undirected = IGRAPH_UNDIRECTED,
		Directed = IGRAPH_DIRECTED
	};
	
	/**
	 \enum NeighboringMode
	 \brief How to pick neighbors of a node
	 */
	enum NeighboringMode {
		OutNeighbors = IGRAPH_OUT,
		InNeighbors = IGRAPH_IN,
		AllNeighbors = IGRAPH_ALL,
		TotalNeighbors = IGRAPH_TOTAL
	};
	
	/**
	 \enum EdgeOrderType
	 \brief How to order the edges
	 */
	enum EdgeOrderType {
		OrderByID = IGRAPH_EDGEORDER_ID,
		OrderByFromVertex = IGRAPH_EDGEORDER_FROM,
		OrderByToVertex = IGRAPH_EDGEORDER_TO
	};
	
	/**
	 \enum SelfLoops
	 \brief Whether the graph contains self loops
	 */
	enum SelfLoops {
		NoSelfLoops = IGRAPH_NO_LOOPS,
		ContainSelfLoops = IGRAPH_LOOPS
	};
	
	enum EdgeMultiplicity {
		EdgeMultiplicity_Simple,
		EdgeMultiplicity_Multiple
	};
	
	enum MutualConnections {
		MutualConnections_NotMutual,
		MutualConnections_Mutual,
	};

// We can't use XXINTRNL_ on the structs because the error is expected to be shown to the users.
#define XXINTRNL_PREPARE_UNDERLYING_TYPES(templname, origtype) \
template<typename T> struct XX_ERROR___##templname##_can_only_store_Real__long__Boolean_or_char__but_not_; \
template<> struct XX_ERROR___##templname##_can_only_store_Real__long__Boolean_or_char__but_not_<Real> { typedef igraph_##origtype##_t type; }; \
template<> struct XX_ERROR___##templname##_can_only_store_Real__long__Boolean_or_char__but_not_<long> { typedef igraph_##origtype##_long_t type; }; \
template<> struct XX_ERROR___##templname##_can_only_store_Real__long__Boolean_or_char__but_not_<char> { typedef igraph_##origtype##_char_t type; }; \
template<> struct XX_ERROR___##templname##_can_only_store_Real__long__Boolean_or_char__but_not_<Boolean> { typedef igraph_##origtype##_bool_t type; }

#define XXINTRNL_UNDERLYING_TYPE(templname) typename XX_ERROR___##templname##_can_only_store_Real__long__Boolean_or_char__but_not_<T>::type

	template<typename T> void XXINTRNL_fprintf(::std::FILE* f, const T input) throw();
	template<> void XXINTRNL_fprintf<Real>(::std::FILE* f, const Real input) throw() { ::std::fprintf(f, "%lg", input); }
	template<> void XXINTRNL_fprintf<long>(::std::FILE* f, const long input) throw() { ::std::fprintf(f, "%ld", input); }
	template<> void XXINTRNL_fprintf<char>(::std::FILE* f, const char input) throw() { ::std::fprintf(f, "%d", input); }	// this is for compatibility of igraph.
	template<> void XXINTRNL_fprintf<Boolean>(::std::FILE* f, const Boolean input) throw() { ::std::fprintf(f, "%d", input); }
	
#define XXINTRNL_DEFINE_SSCANF(type, intem_type, format) \
template<> bool XXINTRNL_sscanf(const char*& str_io, type& res) throw() { \
	int n; \
	intem_type r; \
	if(::std::sscanf(str_io, format " %n", &r, &n)) { \
		res = static_cast<type>(r); \
		str_io += n; \
		return true; \
	} else { \
		str_io += 1; \
		return false; \
	} \
}
	
	template<typename T> bool XXINTRNL_sscanf(const char*& str_io, T& res) throw();
	XXINTRNL_DEFINE_SSCANF(Real, double, "%lg");
	XXINTRNL_DEFINE_SSCANF(long, long, "%ld");
	XXINTRNL_DEFINE_SSCANF(char, int, "%d");
	XXINTRNL_DEFINE_SSCANF(Boolean, int, "%d");
	
#undef XXINTRNL_DEFINE_SSCANF
	
	void XXINTRNL_nop(...) {};
	
	
}

#include <tempobj.hpp>

#endif