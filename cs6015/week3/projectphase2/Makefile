CXX = clang++
# CXXFLAGS = -std=c++17 -Wall -Wextra

# detect undefined behavior
# stop if UB happens
CXXFLAGS = -std=c++17 -Wall -Wextra -fsanitize=undefined -fno-sanitize-recover=undefined

# Main program (includes tests)
msdscript: main.o cmdline.o expr.o parse.o val.o env.o
	$(CXX) $(CXXFLAGS) main.o cmdline.o expr.o parse.o val.o env.o -o msdscript

main.o: main.cpp cmdline.h expr.hpp parse.hpp val.h catch.h
	$(CXX) $(CXXFLAGS) -c main.cpp

cmdline.o: cmdline.cpp cmdline.h
	$(CXX) $(CXXFLAGS) -c cmdline.cpp

expr.o: expr.cpp expr.hpp val.h
	$(CXX) $(CXXFLAGS) -c expr.cpp

val.o: val.cpp val.h expr.hpp
	$(CXX) $(CXXFLAGS) -c val.cpp

parse.o: parse.cpp parse.hpp expr.hpp
	$(CXX) $(CXXFLAGS) -c parse.cpp


# -----------------------------
# Unit tests
# -----------------------------
test: test.o test_val.o expr.o val.o parse.o env.o
	$(CXX) $(CXXFLAGS) test.o test_val.o expr.o val.o parse.o env.o -o test

test.o: test.cpp catch.h expr.hpp val.h parse.hpp
	$(CXX) $(CXXFLAGS) -c test.cpp

test_val.o: test_val.cpp catch.h val.h expr.hpp
	$(CXX) $(CXXFLAGS) -c test_val.cpp


# -----------------------------
# Random testing program
# -----------------------------
test_msdscript: test_msdscript.cpp exec.cpp exec.h
	$(CXX) $(CXXFLAGS) test_msdscript.cpp exec.cpp -o test_msdscript


# -----------------------------
clean:
	rm -f *.o msdscript test test_msdscript