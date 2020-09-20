all: compile runtime

compile: compile.cpp compiler.h bits.h
	g++ -O3 compile.cpp -o compile

runtime: runtime.cpp bits.h
	g++ -O3 runtime.cpp -o runtime

clean:
	rm -f compile runtime
