#include <igraph/igraph.hpp>
#include <cassert>

using namespace std;
using namespace igraph;

int main () {
// Test equality, creation from string.
	Matrix m ("1, 2, 5; 4, 6, 8");
	assert(m == m);
	assert(m == Matrix("1 2 5; 4 6 8"));
	assert(m == Matrix("1 2 5| 4 6 8|", "|"));
	assert(m != Matrix("1 2; 5 4 6 8"));
	m.print();
	m.print(" # ");
	m.print(" # ", " @ ");
	m.print(" # ", " @ ", stdout);
	
// null, fill, isnull, empty, size, nrow, ncol
	assert(!m.isnull());
	assert(!m.empty());
	assert(m.nrow() == 2);
	assert(m.ncol() == 3);
	assert(m.size() == 6);
	
	m.null();
	assert(m.isnull());
	assert(!m.empty());
	assert(m.nrow() == 2);
	assert(m.ncol() == 3);
	assert(m.size() == 6);
	assert(m == Matrix("0,0,0;0,0,0"));
	
	m.fill(-4);
	assert(m == Matrix("-4 -4 -4; -4 -4 -4"));
	
// access elements
	m = Matrix("4, -5, -6; 7, 9, -2");
	assert(m(0,0)==4 && m(0,1)==-5 && m(0,2)==-6 && m(1,0)==7 && m(1,1)==9 && m(1,2)==-2);
	assert((m[pair<long,long>(0,1)] == -5));
	m(0,0)=21;
	m(1,2)=24;
	m[pair<long,long>(1,0)]=-53;
	assert(m == Matrix("21,-5,-6; -53,9,24"));
	
	assert(m.e(0, 2) == -6);
	*m.e_ptr(0, 1) = 1;
	assert(m(0,1) == 1);
	m.set(1, 0, 4);
	assert(m(1,0) == 4);

// copy_to
	Real store[6];
	m.copy_to(store);
	assert(store[0] == 21 && store[2] == 1 && store[4] == -6 && store[1] == 4 && store[3] == 9 && store[5] == 24);
	
// n() and alike
	
	Matrix m2 = Matrix::n();
	assert(m2.isnull());
	assert(m2.nrow() == 0 && m2.ncol() == 0);
	assert(m2.empty());
	
	m2 = Matrix(2, 3);
	assert(m2.isnull());
	assert(m2.nrow() == 2 && m2.ncol() == 3);
	assert(!m2.empty());
	
// swap, update
	m2.swap(m);
	assert(m.isnull() && m2 == Matrix("21, 1, -6; 4, 9, 24"));
	m.update(m2);
	assert(m == m2);
	m2(1, 2) = 4;
	assert(m(1, 2) == 24);
	assert(m != m2);
	
// get_row, get_col, etc.
	assert(m.get_row(1) == Vector("4 9 24"));
	assert(m.get_col(1) == Vector("1 9"));
	m.set_row(1, Vector("6 -2 4")).set_col(1, Vector("5 7"));
	assert(m == Matrix("21 5 -6; 6 7 4"));
	m.swap_rows(0, 1);
	assert(m == Matrix("6 7 4; 21 5 -6"));
	m.swap_cols(1, 2);
	assert(m == Matrix("6 4 7; 21 -6 5"));
	assert(m.select_cols(Vector("0 2")) == Matrix("6 7; 21 5"));
	assert(m.select_rows(Vector("0 1 0")) == Matrix("6 4 7; 21 -6 5; 6 4 7"));
	
// +=, -=, etc.
	assert(Matrix("6, 7, 2; 44, -4, 6") + 2 * Matrix("2, 2, 5; -1, 4, 3") - 4 - (m+1)/2 == Matrix("2.5, 4.5, 4; 27, 2.5, 5"));
	m.mul_elements(m);
	assert(m == Matrix("36 16 49; 441 36 25"));
	m.div_elements(Matrix("1 2 4; -8 -4 -2"));
	assert(m == Matrix("36 8 12.25; -55.125 -9 -12.5"));
	
// sum, prod, etc.
	assert(m.sum() == -20.375);
	assert(m.prod() == -43758225./2);
	assert(m.colsum() == Vector("-19.125 -1 -0.25"));
	assert(m.rowsum() == Vector("56.25 -76.625"));
	m.transpose();
	assert(m == Matrix("36 -55.125; 8 -9; 12.25 -12.5"));
	
// rbind, cbind
	m.cbind(m*8);
	assert(m == Matrix("36 -55.125 288 -441; 8 -9 64 -72; 12.25 -12.5 98 -100"));
	m.rbind(Matrix("1 2 3 4"));
	assert(m == Matrix("36 -55.125 288 -441; 8 -9 64 -72; 12.25 -12.5 98 -100; 1 2 3 4"));
	
// min, max, etc.
	assert(m.min() == -441);
	assert(m.max() == 288);
	assert((m.which_min() == pair<long,long>(0, 3)));
	assert((m.which_max() == pair<long,long>(0, 2)));
	Real minstore, maxstore;
	m.minmax(minstore, maxstore);
	assert(minstore == -441 && maxstore == 288);
	pair<long,long> minloc, maxloc;
	m.which_minmax(minloc, maxloc);
	assert((minloc == pair<long,long>(0,3) && maxloc==pair<long,long>(0,2)));
	
// is_symmetric
	assert(!m.is_symmetric());
	assert(Matrix("2 5 5 1; 5 18 -3 -1; 5 -3 16 -2; 1 -1 -2 12").is_symmetric());
	
// maxdifference
	assert(m.maxdifference(Matrix(m).transpose()) == 442);
	
// contains, etc.
	assert(m.contains(-441));
	assert(!m.contains(-101));
	pair<long,long> pos;
	assert(m.search(12.25, 0, pos));
	assert(pos.first == 2 && pos.second == 0);
	long linpos;
	assert(m.search(-72, 0, pos, linpos));
	assert(linpos == 13);
	
// resize, etc.
	m.resize(2, 3);
	assert(m.nrow() == 2 && m.ncol() == 3);
	m(0,0)=36; m(0,1)=-55.125; m(0,2)=288; m(1,0)=8; m(1,1)=-9; m(1,2)=64;
	assert(m.nrow() == 2 && m.ncol() == 3);
	m.remove_row(0);
	assert(m == Matrix("8 -9 64"));
	m.remove_col(1);
	assert(m == Matrix("8 64"));
	m.add_rows(2);
	assert(m.nrow() == 3 && m.ncol() == 2);
	m.add_cols(3);
	assert(m.nrow() == 3 && m.ncol() == 5);
	m.add_row();
	assert(m.nrow() == 4 && m.ncol() == 5);
	m.add_col();
	assert(m.nrow() == 4 && m.ncol() == 6);
	assert(m(0,0)==8 && m(0,1)==64);
	m.fill(7);
	assert(m == Matrix("7 7 7 7 7 7; 7 7 7 7 7 7; 7 7 7 7 7 7; 7 7 7 7 7 7"));
	m.resize(8, 9);
	assert(m.nrow() == 8 && m.ncol() == 9);
	
	printf("matrix.hpp is correct.\n");
	
	return 0;
	
}