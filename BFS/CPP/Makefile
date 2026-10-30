CXX ?= g++
CXXFLAGS ?= -std=c++11 -O2

all: bfs

bfs: bfs.cpp
	$(CXX) $(CXXFLAGS) bfs.cpp -o bfs

run: bfs
	./bfs input.txt output.txt

clean:
	rm -f bfs bfs.exe output.txt
