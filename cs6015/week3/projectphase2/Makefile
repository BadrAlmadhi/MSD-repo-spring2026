CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra

# Main program (includes tests)
msdscript: main.o cmdline.o expr.o parse.o test.o
	$(CXX) $(CXXFLAGS) main.o cmdline.o expr.o parse.o test.o -o msdscript

main.o: main.cpp cmdline.h expr.hpp parse.hpp catch.h
	$(CXX) $(CXXFLAGS) -c main.cpp

cmdline.o: cmdline.cpp cmdline.h
	$(CXX) $(CXXFLAGS) -c cmdline.cpp

expr.o: expr.cpp expr.hpp
	$(CXX) $(CXXFLAGS) -c expr.cpp

parse.o: parse.cpp parse.hpp expr.hpp
	$(CXX) $(CXXFLAGS) -c parse.cpp


# -----------------------------
# Unit tests (existing)
# -----------------------------
test: test.o expr.o
	$(CXX) $(CXXFLAGS) test.o expr.o -o test

test.o: test.cpp catch.h expr.hpp
	$(CXX) $(CXXFLAGS) -c test.cpp


# -----------------------------
# Random testing program (NEW)
# -----------------------------
test_msdscript: test_msdscript.cpp exec.cpp exec.h
	$(CXX) $(CXXFLAGS) test_msdscript.cpp exec.cpp -o test_msdscript


# -----------------------------
clean:
	rm -f *.o msdscript test test_msdscript