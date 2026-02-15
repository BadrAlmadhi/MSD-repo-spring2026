CXX = clang++
# added -Iinclude in CXXFLAGS
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

# variables for LIBNAME = msdscript LIBFILE = libmsdscript.a 
# this will use in command to build .a file
LIBNAME = msdscript
LIBFILE = lib$(LIBNAME).a

# Implement files to go into the library (NO main.cpp)
LIBSRCS = src/expr.cpp src/cmdline.cpp
LIBOBJS = $(LIBSRCS:.cpp=.o)

all: msdscript

# Main program (includes tests)
msdscript: main.o $(LIBFILE)
	$(CXX) main.o -L. -l$(LIBNAME) -o msdscript

# compile main
main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o


# compile library objects
src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# create static library (.a) from objects
$(LIBFILE): $(LIBOBJS)
	ar rcs $(LIBFILE) $(LIBOBJS)

# rule required by lab step 4/6: build from external library location
fromlib: main.o
	$(CXX) main.o -L../Library_MSDScript -l$(LIBNAME) -o msdscript

clean:
	rm -f src/*.o *.o msdscript $(LIBFILE)