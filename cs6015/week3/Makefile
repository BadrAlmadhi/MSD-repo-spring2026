# Object files
OBJS = main.o expr.o

# Compiler
CXX = clang++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -g

# Header files
INCS = expr.h

# Final executable
msdscript: $(OBJS)
	$(CXX) $(CXXFLAGS) -o msdscript $(OBJS)

# Compile main.cpp -> main.o
main.o: main.cpp $(INCS)
	$(CXX) $(CXXFLAGS) -c main.cpp

# Compile expr.cpp -> expr.o
expr.o: expr.cpp $(INCS)
	$(CXX) $(CXXFLAGS) -c expr.cpp

# Clean build files
.PHONY: clean
clean:
	rm -rf *.o msdscript
