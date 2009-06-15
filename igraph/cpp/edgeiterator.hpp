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
#include <cstring>

namespace igraph {
	class Graph;
	class EdgeSelector;
	
	class EdgeIterator {
	private:
		igraph_eit_t _;
		
	public:
		EdgeIterator(const Graph& g, const EdgeSelector& vs) MAY_THROW_EXCEPTION {
			TRY(igraph_eit_create(&g._, vs._, &_));
		}
		~EdgeIterator() throw() { igraph_eit_destroy(&_); }
		
		void next() throw() { IGRAPH_EIT_NEXT(_); }
		bool at_end() const throw() { return IGRAPH_EIT_END(_); }
		bool operator! () const throw() { return !IGRAPH_EIT_END(_); }
		void reset() throw() { IGRAPH_EIT_RESET(_); }
		Edge get() const throw() { return IGRAPH_EIT_GET(_); }
		long size() const throw() { return IGRAPH_EIT_SIZE(_); }
		
		bool operator==(const EdgeIterator& other) const throw() { !std::memcmp(&_, &other._, sizeof(_)); }
		bool operator!=(const EdgeIterator& other) const throw() { std::memcmp(&_, &other._, sizeof(_)); }
		
		RETRIEVE_TEMPORARY_CLASS(EdgeVector) as_vector() const MAY_THROW_EXCEPTION;
		
#pragma mark -
		
		// STL support
		class EdgeIteratorSTLWrapper  {
		private:
			EdgeIterator* iter;
		public:
			EdgeIteratorSTLWrapper(EdgeIterator* iter_) throw() : iter(iter_) {}
			
			bool operator== (const EdgeIteratorSTLWrapper& other) const throw() {
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
			bool operator!= (const EdgeIteratorSTLWrapper& other) const throw() { return !(*this == other); }
			
			Edge operator*() const throw() { return iter->get(); }
			EdgeIteratorSTLWrapper& operator++() throw() { iter->next(); return *this; }
			
#if IGRAPH_DEBUG
			__attribute__((error("Do not use post-increment for VertexIteratorSTLWrapper!")))
			VertexIterator& operator++(int);
#endif
		};
		
		
		typedef EdgeIteratorSTLWrapper iterator;
		typedef EdgeIteratorSTLWrapper const_iterator;
		typedef Edge value_type;
		
		iterator begin() throw() { return EdgeIteratorSTLWrapper(this); }
		iterator end() throw() { return EdgeIteratorSTLWrapper(NULL); }
		
	};	
}

#endif