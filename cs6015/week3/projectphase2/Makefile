CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra

#OBJS = main.o cmdline.o expr.o parse.o val.o test.o
# Main program (includes tests)
msdscript: main.o cmdline.o expr.o parse.o val.o
	$(CXX) $(CXXFLAGS) main.o cmdline.o expr.o parse.o val.o -o msdscript

main.o: main.cpp cmdline.h expr.hpp parse.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

cmdline.o: cmdline.cpp cmdline.h
	$(CXX) $(CXXFLAGS) -c cmdline.cpp

expr.o: expr.cpp expr.hpp
	$(CXX) $(CXXFLAGS) -c expr.cpp

val.o: val.cpp val.h expr.hpp
	$(CXX) $(CXXFLAGS) -c val.cpp

parse.o: parse.cpp parse.hpp expr.hpp
	$(CXX) $(CXXFLAGS) -c parse.cpp


# -----------------------------
# Unit tests (existing)
# -----------------------------
test: test.o test_val.o expr.o val.o
	$(CXX) $(CXXFLAGS) test.o test_val.o expr.o val.o -o test

test.o: test.cpp catch.h expr.hpp
	$(CXX) $(CXXFLAGS) -c test.cpp

test_val.o: test_val.cpp catch.h val.h expr.hpp
	$(CXX) $(CXXFLAGS) -c test_val.cpp


# -----------------------------
# Random testing program (NEW)
# -----------------------------
test_msdscript: test_msdscript.cpp exec.cpp exec.h
	$(CXX) $(CXXFLAGS) test_msdscript.cpp exec.cpp -o test_msdscript


# -----------------------------
clean:
	rm -f *.o msdscript test test_msdscript