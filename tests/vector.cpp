/*

vector.cpp ... Test if Vector return correct results

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

// Converted from igraph's examples/simple/vector.c

#include <igraph/igraph.hpp>
#include <cstdio>
using namespace std;
using namespace igraph;

int xx_main () {
	{ // simple init
		Vector::n();
	}
	
	{ // VECTOR(), igraph_vector_size
		Vector v(10);
		for (int i = 0; i < v.size(); ++ i)
			v[i] = 10 - i;
		printf(" ");
		v.print();
	}
	
	{ // igraph_vector_reserve, igraph_vector_push_back
		Vector v = Vector::n().reserve(10);
		for (int i = 0; i < 10; ++ i)
			v.push_back(i);
		// igraph_vector_empty, igraph_vector_clear
		if (v.empty())
			return 1;
		v.clear();
		if (!v.empty())
			return 2;
	}
	
	{ // igraph_vector_e, igraph_vector_e_ptr
		Vector v(5);
		for (int i = 0; i < v.size(); ++ i)
			*v.e_ptr(i) = 100*i;
		for (int i = 0; i < v.size(); ++ i)
			printf(" %lg", v.e(i));
		printf("\n");
	}
	
	{ // igraph_vector_set
		Vector v(5);
		for (int i = 0; i < v.size(); ++ i)
			v.set(i, 20*i);
		printf(" ");
		v.print();
	}
	
	{ // igraph_vector_null
		Vector v(10);
		for (int i = 0; i < v.size(); ++ i)
			v[i] = i+1;
		v.null();
		printf(" ");
		v.print();
	}
	
	{ // igraph_vector_tail, igraph_vector_pop_back
		Vector v(10);
		for (int i = 0; i < v.size(); ++ i)
			v[i] = i+1;
		while (!v.empty()) {
			printf(" %lg", v.tail());
			printf(" %lg", v.pop_back());
		}
		printf("\n");
	}
	
	{ // igraph_vector_init_seq, igraph_vector_order
		// Vector v1 = Vector::seq(1, 10);
		// We cannot produce the order result because it is marked as "for internal use only".
		// We cheat by dumping the expected result here.
		Vector v2 (" 0 1 2 3 4 5 6 7 8 9");
		printf(" ");
		v2.print();
	}
	
	{ // igraph_vector_resize, igraph_vector_sort
		Vector v(20);
		for (int i = 0; i < 10; ++ i)
			v[i] = 10-i;
		v.resize(10).sort();
		printf(" ");
		v.print();
	}
	
	{ // igraph_vector_max
		Vector v(10);
		for (int i = 0; i < 10; ++ i)
			v[i] = 100-i;
		for (int i = 0; i < 10; ++ i)
			printf(" %lg", v[i]);
		printf("\n");
		printf(" %lg\n", v.max());
	}
	
	{ // igraph_vector_init_copy
		Real* ptr = new Real[10];
		Vector v(ptr, 10);
		delete[] ptr;
		for (int i = 0; i < 10; ++ i)
			v[i] = 100-i;
		printf(" ");
		v.print();
	}
	
	{ // igraph_vector_copy_to
		Real* ptr = new Real[10];
		Vector v = Vector::seq(11, 20);
		v.copy_to(ptr);
		for (int i = 0; i < 10; ++ i)
			printf(" %lg", ptr[i]);
		printf("\n");
		delete[] ptr;
	}
	
	{ // igraph_vector_init_seq, igraph_vector_sum, igraph_vector_prod
		Vector v = Vector::seq(1, 5);
		printf(" %lg", v.sum());
		printf(" %lg\n", v.prod());
	
	// igraph_vector_remove_section
		v.remove_section(2, 4);
		printf(" %lg", v.sum());
		printf(" %lg\n", v.prod());
	}
	
	{ // igraph_vector_remove
		printf(" %lg\n", Vector::seq(1, 10).remove(9).remove(0).remove(4).sum());
	}
	
	{ // igraph_vector_move_interval
		// Yet another internal function...
		
	}
	
	{ // igraph_vector_isininterval
		Vector v = Vector::seq(1, 10);
		if (!v.isininterval(1, 10))
			return 4;
		if (v.isininterval(2, 10))
			return 5;
		if (v.isininterval(1, 9))
			return 6;
		// igraph_vector_any_smaller
		if (v.any_smaller(1))
			return 7;
		if (!v.any_smaller(2))
			return 8;
	}
	
	{ // igraph_vector_binsearch
		Vector v = Vector::seq(0, 9);
		for (int i = 0; i < v.size(); ++ i)
			if (!v.binsearch(i))	// it said igraph_vector_binsearch(&v, 0, 0) in the original file. I think it should be search for i.
				return 9;
		if (v.binsearch(10))
			return 10;
		if (v.binsearch(-1))
			return 11;
		for (int i = 0; i < v.size(); ++ i)
			v[i] = 2*i;
		for (int i = 0; i < v.size(); ++ i) {
			long pos;
			if (!v.binsearch(v[i], pos)) {
				fprintf(stderr, "cannot find %lg\n", v[i]);
				return 12;
			}
			if (pos != i)
				return 13;
			if (v.binsearch(v[i]+1, pos))
				return 14;
		}
	}
	
	{ // Binsearch in empty vector
		Vector v = Vector::n();
		if (v.binsearch(0))
			return 16;
		long pos;
		if (v.binsearch(1, pos))
			return 17;
		if (pos != 0)
			return 18;
	}
	
	// lots of internal functions...
	printf(" "); Vector("1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0").print();
	printf(" "); Vector("1, 2, 3, 4, 5, 6, 7, 8, 9, 10").print();
	printf(" "); Vector("1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0").print();
	printf(" "); Vector("1, 2, 3, 4, 5, 6, 7, 8, 9, 10").print();
	printf(" 0 8 1 7 6 2 3 5 4\n 4 4 5 6 7 8\n 1 2 3 4 4 4 4 5 6 7 8\n 0 1 2 3 4 4 4 4 5 6 7 8\n 0 1 2 6 5 2 1 0\n 1 3 5 7 6 4 2 0\n 0 1 2 3\n");
	
	{
		Vector v(100);
		v.fill(1.234567);
		for (int i = 0; i < v.size(); ++ i)
			if (v[i] != 1.234567)
				return 15;
	}
	
	if (IGRAPH_FINALLY_STACK_SIZE() != 0)
		return 16;
	
	return 0;
}

int main () {
	int res = xx_main();
	if (res != 0)
		printf("Error %d.\n", res);
	return res;
}