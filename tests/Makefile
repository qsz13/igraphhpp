Compiler=g++-4.4
Options=-std=gnu++0x -O2 -ligraph -I../

%.exe:	%.cpp
	$(Compiler) $(Options) -o $@ $^

all:	igraph_cliques.exe vector.exe