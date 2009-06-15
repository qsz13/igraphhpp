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
	
	enum PeriodicLattice {
		PeriodicLattice_NotPeriodic,
		PeriodicLattice_Periodic,
	};
	
	enum BarabasiOutPref {
		BarabasiOutPref_InDegreeOnly,
		BarabasiOutPref_TotalDegree,
	};
	
	enum ToUndirectedMode {
		EachArcToEdge = IGRAPH_TO_UNDIRECTED_EACH,
		CollapseArcs = IGRAPH_TO_UNDIRECTED_COLLAPSE
	};
	
	enum ToDirectedMode {
		/// the number of edges in the graph stays the same, an arbitrarily directed edge is created for each undirected edge
		EachEdgeToArc = IGRAPH_TO_DIRECTED_ARBITRARY,
		/// two directed edges are created for each undirected edge, one in each direction.
		SplitEdges = IGRAPH_TO_DIRECTED_MUTUAL
	};
}

#include <tempobj.hpp>

#endif