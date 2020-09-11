div: bits.h compiler.cpp compiler.h main.cpp Makefile README.md utils.cpp utils.h
	g++ compiler.cpp main.cpp utils.cpp -o div

clean:
	rm -f div
