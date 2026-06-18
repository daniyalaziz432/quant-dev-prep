CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -O2

variables:
	$(CXX) $(CXXFLAGS) cpp-basics/variables.cpp -o variables

functions:
	$(CXX) $(CXXFLAGS) cpp-basics/functions.cpp -o functions

pointers:
	$(CXX) $(CXXFLAGS) cpp-basics/pointers.cpp -o pointers

vector:
	$(CXX) $(CXXFLAGS) stl/vector.cpp -o vector

threads:
	$(CXX) $(CXXFLAGS) multithreading/threads.cpp -o threads

clean:
	rm -f variables functions pointers vector threads