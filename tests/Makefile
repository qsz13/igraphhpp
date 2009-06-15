Compiler=g++-4.2
Options=-Wall -O2 -ligraph -I../

%.exe:	%.cpp
	$(Compiler) $(Options) -o $@ $^

all:	igraph_cliques.exe vector.exe
