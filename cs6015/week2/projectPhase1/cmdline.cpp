// connect .cpp to header file
// make sure declared function matches the one in .h file
// if not matched compiler error
#include "cmdline.h"

// needed for std::cout std::cerr
#include <iostream>
// needed for std::string
#include <string>
// needed for std::exit(0) or std::exit(1)
#include <cstdlib> // exit


// helper function 
// type void retruns nothing
// static means VERY IMPORTANT and this function only visible inside this file
// static mean other .cpp files cannot call it
// static means private help 

// this function only prints text
static void print_help() {
    // function print help text to standard output
    std::cout
    // << chaining one long print \n means new line \"\" quote
        << "msdscript usage:\n"
        << "  ./msdscript [--help] [--test]\n\n"
        << "Allowed arguments:\n"
        << "  --help   Print this help text and exit.\n"
        << "  --test   Print \"Tests passed\" (only once).\n";
}

// main function 
// re-declare function from .h file
// main calls this function through makefile 
void use_arguments(int argc, char **argv) {
    // set seen to false 
    bool seen_test = false;

    // loop 
    // start at 1 because real argument starts at 1 
    for (int i = 1; i < argc; i++) {
        // convert argument into string 
        std::string arg = argv[i];

        // if argument = --help
        if (arg == "--help") {
            print_help(); // print help text
            std::exit(0); // stop immediately
        } else if (arg == "--test") {
            if (seen_test) {
                std::cerr << "Error: --test was provided more than once.\n";
                std::exit(1); // stop failure found
            }
            seen_test = true;
            std::cout << "Tests passed\n";
        } else {
            std::cerr << "Error: unknown argument: " << arg << "\n";
            std::exit(1);
        }
    }
}
