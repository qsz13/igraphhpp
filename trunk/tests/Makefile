Compiler=g++-4.4
Options=-std=gnu++0x -Wall -Wno-unknown-pragmas -O2 -ligraph -lgsl -lgslcblas -I../ -I/opt/local/include -L/opt/local/lib -Wno-attributes -fno-strict-aliasing

%.exe:	%.cpp
	$(Compiler) $(Options) -o $@ $^

%.cov.exe:	%.cpp
	$(Compiler) $(Options) -fprofile-arcs -ftest-coverage -o $@ $^

all:
	for i in `ls *.cpp`; do make $${i/%cpp/exe}; done

cover:
	for i in `ls *.cpp`; do make $${i/%cpp/cov.exe}; done

clean:
	-rm *.gcov
	-rm *.gcno
	-rm *.gcda
