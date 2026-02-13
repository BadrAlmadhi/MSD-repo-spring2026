CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra

# Main program (includes tests)
msdscript: main.o cmdline.o expr.o test.o
	$(CXX) $(CXXFLAGS) main.o cmdline.o expr.o test.o -o msdscript

main.o: main.cpp cmdline.h expr.hpp catch.h
	$(CXX) $(CXXFLAGS) -c main.cpp

cmdline.o: cmdline.cpp cmdline.h
	$(CXX) $(CXXFLAGS) -c cmdline.cpp

expr.o: expr.cpp expr.hpp
	$(CXX) $(CXXFLAGS) -c expr.cpp

# Tests
test: test.o expr.o
	$(CXX) $(CXXFLAGS) test.o expr.o -o test

test.o: test.cpp catch.h expr.hpp
	$(CXX) $(CXXFLAGS) -c test.cpp

clean:
	rm -f *.o msdscript test