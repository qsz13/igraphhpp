#undef NDEBUG

#include <cassert>
#include <igraph/igraph.hpp>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;
using namespace igraph;

int main () {
	Vector v = Vector("4., 5., 6., 9., 8., 7.");
		
	v.print();
	v.print(" .... ");
	v.print(" @@ ", stdout);
	
	assert(v == v);
	assert(v == Vector(" 4., 5., 6., 9., 8., 7."));
	assert(v != Vector(" 3., 4., 5."));
	assert(v.size() == 6);
	assert(v.sum() == 4 + 5 + 6 + 9 + 8 + 7);
	assert(v.prod() == 4 * 5 * 6 * 9 * 8 * 7);
	assert(v.max() == 9);
	assert(v.min() == 4);
	assert(v.which_max() == 3);
	assert(v.which_min() == 0);
	Real v_max, v_min;
	long which_max, which_min;
	v.minmax(v_min, v_max);
	assert(v_min == 4 && v_max == 9);
	v.which_minmax(which_min, which_max);
	assert(which_min == 0 && which_max == 3);
	v += 1;
	assert(v == Vector("5., 6., 7., 10., 9., 8."));
	v *= 2;
	assert(v == Vector("10., 12., 14., 20., 18., 16."));
	v.sort();
	assert(v == Vector("10., 12., 14., 16., 18., 20."));
	
	assert(v.pop_back() == 20.);
	assert(v == Vector(" 10., 12., 14., 16., 18."));
	v.push_back(12.);
	assert(v == Vector(" 10., 12., 14., 16., 18., 12."));
	
	{
		Vector x = v * (3 + v + 2) * v;
		assert(v == Vector("10., 12., 14., 16., 18., 12."));
		assert(x == Vector("1500., 2448., 3724., 5376., 7452., 2448."));
	}
	assert(v == Vector("10., 12., 14., 16., 18., 12."));
	
	Real temp[] = {2, 3, 5, 7, 11};
	{
		Vector w = Vector::view(temp, 5);
		assert(w == Vector("2., 3., 5., 7., 11."));
	}
	assert(temp[0] == 2 && temp[1] == 3 && temp[2] == 5 && temp[3] == 7 && temp[4] == 11);
	
	v.append(Vector("6 7"));
	assert(v == Vector(" 10., 12., 14., 16., 18., 12., 6., 7."));
	assert(v.contains(16.) && v.contains(6.));
	assert(!v.contains(2.));
	long pos = -1;
	assert(v.search(18., 3, pos));
	assert(pos == 4);
	assert(!v.search(18., 5, pos));
	assert(!v.search(9., 0));
	assert(!v.search(10., 1));
	v.sort();
	assert(v.binsearch(16., pos));
	assert(pos == 6);
	assert(!v.binsearch(4.));
	
	v.null();
	assert(v == Vector(" 0., 0., 0., 0., 0., 0., 0., 0."));
	assert(v.isnull());
	v.fill(-12.);
	assert(v == Vector(" -12., -12., -12., -12., -12., -12., -12., -12."));
	assert(!v.isnull());
	
	Vector x = Vector::seq(4, 10);
	assert(x == Vector(" 4., 5., 6., 7., 8., 9., 10."));
	assert(x[2] == 6.);
	x[2] = 13.;
	assert(x[2] == 13.);
	assert(x[3] == 7.);
	assert(x == Vector(" 4., 5., 13., 7., 8., 9., 10."));
	assert(x.e(2) == 13.);
	x.set(3, 14.);
	assert(x == Vector(" 4., 5., 13., 14., 8., 9., 10."));
	assert(*x.e_ptr(3) == 14.);
	*x.e_ptr(4) = 15.;
	assert(x == Vector(" 4., 5., 13., 14., 15., 9., 10."));
	assert(x.tail() == 10.);
	
	Real c[7];
	x.copy_to(c);
	assert(c[0] == 4. && c[1] == 5. && c[2] == 13. && c[3] == 14. && c[4] == 15. && c[5] == 9. && c[6] == 10.);
	x.resize(4);
	assert(x == Vector("4., 5., 13., 14."));
	Vector y = x;
	x.reverse();
	assert(x == Vector(" 14., 13., 5., 4."));
	assert(y == Vector("4., 5., 13., 14."));
	x.swap(y);
	assert(y == Vector("14., 13., 5., 4."));
	assert(x == Vector("4., 5., 13., 14."));
	y.remove(2);
	assert(y == Vector("14., 13., 4."));
	y.swap_elements(0, 1);
	assert(y == Vector("13., 14., 4."));
	x.resize(2);
	assert(x == Vector("4., 5."));
	y.insert(2, 64.);
	y.insert(1, 89.);
	assert(y == Vector("13., 89., 14., 64., 4."));
	{
		Vector u = x;
		assert(x == u);
		u.update(y);
		assert(u == y);
		assert(u == Vector("13., 89., 14., 64., 4."));
	}
	assert(x == Vector("4 5"));
	assert(y == Vector("13., 89., 14., 64., 4."));
	y.remove_section(2, 4);
	assert(y == Vector("13., 89., 4."));
	y.clear();
	assert(y.size() == 0);
	
	{
		igraph_vector_t v6;
		igraph_vector_init(&v6, 7);
		Vector v7(&v6);
		Vector v8=v7;
	}
	
	std::vector<int> sv;
	sv.clear();
	sv.push_back(4);
	sv.push_back(14);
	sv.push_back(5);
	sv.push_back(7);
	
	Vector r = Vector(sv.begin(), sv.end());
	assert(r == Vector("4 14 5 7"));
	sv[1] = 22;
	assert(sv.size() == 4 && sv[0] == 4 && sv[1] == 22 && sv[2] == 5 && sv[3] == 7);
	assert(r == Vector("4 14 5 7"));
	sv = std::vector<int>(r.begin(), r.end());
	assert(sv.size() == 4 && sv[0] == 4 && sv[1] == 14 && sv[2] == 5 && sv[3] == 7);
	r[1] = 22;
	assert(sv.size() == 4 && sv[0] == 4 && sv[1] == 14 && sv[2] == 5 && sv[3] == 7);
	assert(r == Vector("4 22 5 7"));
	std::sort(r.begin(), r.end());
	assert(r == Vector("4 5 7 22"));
	assert(std::inner_product(sv.begin(), sv.end(), r.begin(), 0) == 4*4+14*5+5*7+7*22);
	
	Real d[] = {1., 4., 9., 16., 25., 49.};
	r = Vector(d, 6);
	assert(r == Vector("1 4 9 16 25 49"));
	d[5] = 36;
	assert(r == Vector("1 4 9 16 25 49"));
	r[5] = 81;
	assert(d[5] == 36);
	r = Vector::view(d, 6);
	d[4] = 121;
	assert(r == Vector("1 4 9 16 121 36"));
	r[3] = 144;
	assert(d[3] == 144);
	
#if XXINTRNL_CXX0X
	assert(Vector("2 -4 6 12") == Vector({2, -4, 6, 12}));
#endif
	
	assert(Vector("1 1 1 1 1; 2 2 2; 3; 4 4 4 4; 5 5 5").distribution() == Vector("0, 0.3125, 0.1875, 0.0625, 0.25, 0.1875"));
	
	{
		Vector r ("1 4 6 12 11 4");
		Vector s ("6 -2 5 8 6 4");
		assert(r.maxdifference(s) == 6);
		
		r = Vector("1 4 6 12 11 4 124 4");
		r.remove_first_matching(4);
		assert(r == Vector("1 6 12 11 4 124 4"));
		r.remove_all_matching(4);
		assert(r == Vector("1 6 12 11 124"));
		r.sort();
		assert(r == Vector("1 6 11 12 124"));
		r.remove_first_matching_assume_sorted(11);
		assert(r == Vector("1 6 12 124"));
		r.remove_first_matching(4);
		assert(r == Vector("1 6 12 124"));
		r.remove_first_matching_assume_sorted(9);
		assert(r == Vector("1 6 12 124"));
	}
	
	assert(Vector("1 2 3 4 5 6 7").intersect_sorted(Vector("4 5 7 9 11")) == Vector("4 5 7"));
	assert(Vector("1 1 1 1 1 6 7 8").intersect_sorted(Vector("1 1 1 6 7 7 7")) == Vector("1 6 7"));
	assert(Vector("1 1 1 1 1 6 7 8").intersect_sorted(Vector("1 1 1 6 7 7 7"), Vector::NotUnique) == Vector("1 1 1 1, 1 1 1 1; 6 6; 7 7 7 7"));

	{
		Vector t ("1 8 2 7 3 6 4 5");
		t.move_interval(2, 4, 0);
		assert(t == Vector("2 7 2 7 3 6 4 5"));
		t.move_interval(3, 6, 4);
		assert(t == Vector("2 7 2 7 7 3 6 5"));
		assert(t.isininterval(1, 8));
		assert(!t.isininterval(3, 8));
		assert(t.isininterval(2, 7));
		assert(!t.isininterval(1, 5));
		assert(t.any_smaller(3));
		assert(!t.any_smaller(2));
		
		t -= t / 4;
		assert(t == "1.5,5.25,1.5,5.25,5.25,2.25,4.5,3.75");
		t /= Vector("1 2 4 8 -1 -2 -4 -8");
		assert(t == "1.5,2.625,0.375,0.65625,-5.25,-1.125,-1.125,-0.46875");
		t += t * 31;
		assert(t == "48.,84.,12.,21.,-168.,-36.,-36.,-15.");
		
		
		Vector u (12);
		assert(u.isnull());
		assert(u.size() == 12);
		assert(!v.empty());
		Vector v = Vector::n();
		assert(v.isnull());
		assert(v.empty());
		assert(v.size() == 0);
	}
	
	printf("vector.hpp is correct.\n");

	return 0;
}
