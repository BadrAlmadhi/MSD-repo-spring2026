// this is the source file 

// tells C++ i need function declaration from this header
#include "cmdline.h"

// main receives the command-line arguments (argc, argv) from the operating system
// argc = argument count 
// argv = argument value an array of strings
int main(int argc, char **argv) {
    // all logic lives inside use_argument function
    use_arguments(argc, argv);
    return 0;
}