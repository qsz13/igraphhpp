/*

vertexiterator.hpp ... Vertex Iterators

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

#ifndef IGRAPH_EDGEITERATOR_HPP
#define IGRAPH_EDGEITERATOR_HPP

#include <igraph/igraph.h>
#include <igraph/cpp/exception.hpp>
#include <igraph/cpp/vector.hpp>
#include <igraph/cpp/graph.hpp>
#include <cstring>

namespace igraph {
	class Graph;
	class EdgeSelector;
	
	class EdgeIterator {
	private:
		igraph_eit_t _;
		
	public:
		EdgeIterator(const Graph& g, const EdgeSelector& vs) throw(Exception) {
			TRY(igraph_eit_create(&g._, vs._, &_));
		}
		~EdgeIterator() { igraph_eit_destroy(&_); }
		
		void next() throw() { IGRAPH_EIT_NEXT(_); }
		bool at_end() const throw() { return IGRAPH_EIT_END(_); }
		bool operator! () const throw() { return !IGRAPH_EIT_END(_); }
		void reset() throw() { IGRAPH_EIT_RESET(_); }
		Edge get() const throw() { return IGRAPH_EIT_GET(_); }
		long size() const throw() { return IGRAPH_EIT_SIZE(_); }
		
		bool operator==(const EdgeIterator& other) const throw() { !std::memcmp(&_, &other._, sizeof(_)); }
		bool operator!=(const EdgeIterator& other) const throw() { std::memcmp(&_, &other._, sizeof(_)); }
		
		Vector as_vector() const throw(Exception) {
			igraph_vector_t res;
			TRY(igraph_eit_as_vector(&_, &res));
			return Vector(res, OwnershipTransferMove);
		}
		
#pragma mark -
		
		// STL support
		class EdgeIteratorSTLWrapper  {
		private:
			EdgeIterator* iter;
		public:
			EdgeIteratorSTLWrapper(EdgeIterator* iter_) : iter(iter_) {}
			
			bool operator== (const EdgeIteratorSTLWrapper& other) {
				if (other.iter == NULL) {
					if (iter != NULL)
						return iter->at_end();
					else
						return true;
				} else if (iter == NULL)
					return other.iter->at_end();
				else
					return *iter == *other.iter;
			}
			bool operator!= (const EdgeIteratorSTLWrapper& other) { return !(*this == other); }
			
			Edge operator*() const { return iter->get(); }
			EdgeIteratorSTLWrapper& operator++() { iter->next(); return *this; }
			
#if IGRAPH_DEBUG
			__attribute__((error("Do not use post-increment for VertexIteratorSTLWrapper!")))
			VertexIterator& operator++(int);
#endif
		};
		
		
		typedef EdgeIteratorSTLWrapper iterator;
		typedef EdgeIteratorSTLWrapper const_iterator;
		typedef Edge value_type;
		
		iterator begin() { return EdgeIteratorSTLWrapper(this); }
		iterator end() { return EdgeIteratorSTLWrapper(NULL); }
		
	};	
}

#endif