# declared variable CXX = c++
CXX      = c++
# declared CXXFLAGS = line below
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -O2

# name of final program so when building output file should be called msdscript
TARGET = msdscript
# object file convert to object file .o
OBJS   = main.o cmdline.o

# rule starts here 
#build madscript when run make 
all: $(TARGET)

# madscript : main.o cmdline.o 
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

main.o: main.cpp cmdline.h
	$(CXX) $(CXXFLAGS) -c main.cpp

cmdline.o: cmdline.cpp cmdline.h
	$(CXX) $(CXXFLAGS) -c cmdline.cpp

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean
