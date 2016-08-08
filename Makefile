exact:
	g++ exact_synthesis.cpp -o exact -lz3 -O3
clean:
	rm -f exact
