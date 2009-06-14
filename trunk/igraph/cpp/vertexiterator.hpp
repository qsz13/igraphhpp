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

#ifndef IGRAPH_VERTEXITERATOR_HPP
#define IGRAPH_VERTEXITERATOR_HPP

#include <igraph/igraph.h>
#include <igraph/cpp/exception.hpp>
#include <igraph/cpp/vector.hpp>
#include <cstring>

namespace igraph {
	class Graph;
	class VertexSelector;
	
	class VertexIterator {
	private:
		igraph_vit_t _;
		
	public:
		VertexIterator(const Graph& g, const VertexSelector& vs) MAY_THROW_EXCEPTION {
			TRY(igraph_vit_create(&g._, vs._, &_));
		}
		~VertexIterator() throw() { igraph_vit_destroy(&_); }
		
		void next() throw() { IGRAPH_VIT_NEXT(_); }
		bool at_end() const throw() { return IGRAPH_VIT_END(_); }
		bool operator! () const throw() { return !IGRAPH_VIT_END(_); }
		void reset() throw() { IGRAPH_VIT_RESET(_); }
		Vertex get() const throw() { return IGRAPH_VIT_GET(_); }
		long size() throw() { return IGRAPH_VIT_SIZE(_); }
		
		bool operator==(const VertexIterator& other) const throw() { !std::memcmp(&_, &other._, sizeof(_)); }
		bool operator!=(const VertexIterator& other) const throw() { std::memcmp(&_, &other._, sizeof(_)); }
		
		::tempobj::temporary_class<VertexVector>::type as_vector() const MAY_THROW_EXCEPTION;
		
#pragma mark -
		
		// STL support
		class VertexIteratorSTLWrapper  {
		private:
			VertexIterator* iter;
		public:
			VertexIteratorSTLWrapper(VertexIterator* iter_) throw() : iter(iter_) {}
			
			bool operator== (const VertexIteratorSTLWrapper& other) const throw() {
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
			bool operator!= (const VertexIteratorSTLWrapper& other) const throw() { return !(*this == other); }
			
			Vertex operator*() const throw() { return iter->get(); }
			VertexIteratorSTLWrapper& operator++() throw() { iter->next(); return *this; }
			
#if IGRAPH_DEBUG
			__attribute__((error("Do not use post-increment for VertexIteratorSTLWrapper!")))
			VertexIterator& operator++(int);
#endif
		};
		
		typedef VertexIteratorSTLWrapper iterator;
		typedef VertexIteratorSTLWrapper const_iterator;
		typedef Vertex value_type;
		
		iterator begin() throw() { return VertexIteratorSTLWrapper(this); }
		iterator end() throw() { return VertexIteratorSTLWrapper(NULL); }
	};	
}

#endif