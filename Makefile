main:
	g++ exact_synthesis.cpp -o gen_subgraphs -lz3 -O3
clean:
	rm -f gen_subgraphs
