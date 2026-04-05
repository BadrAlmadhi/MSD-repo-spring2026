OBJS = main.o expr.o tests.o

CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -g

INCS = expr.h catch.h

msdscript: $(OBJS)
	$(CXX) $(CXXFLAGS) -o msdscript $(OBJS)

main.o: main.cpp $(INCS)
	$(CXX) $(CXXFLAGS) -c main.cpp

expr.o: expr.cpp $(INCS)
	$(CXX) $(CXXFLAGS) -c expr.cpp

tests.o: tests.cpp $(INCS)
	$(CXX) $(CXXFLAGS) -c tests.cpp

.PHONY: clean
clean:
	rm -rf *.o msdscript *.dSYM
