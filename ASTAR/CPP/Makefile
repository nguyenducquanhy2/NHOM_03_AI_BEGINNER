CXX ?= g++
CXXFLAGS ?= -std=c++11 -O2 -Wall

all: astar

astar: astar.cpp
	$(CXX) $(CXXFLAGS) astar.cpp -o astar

run: astar
	./astar input.txt output.txt

clean:
	rm -f astar astar.exe output.txt

