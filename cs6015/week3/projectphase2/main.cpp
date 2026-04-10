
#include <cstring>
#include <iostream>
#include <string>
#include "cmdline.h"
#include "expr.hpp"
#include "parse.hpp"
#include "val.h"
#include "env.h"

int main(int argc, char **argv) {
    try {
        // Decide what to do based on the command-line flag
        run_mode_t mode = use_arguments(argc, argv);

        switch (mode) {
            case do_test: {
            std::cerr << "Run tests using the test executable\n";
            return 1;
        }

        // my code used to work only when using echo 
        // fixed bug
        case do_interp: {
            // we need to make sure C++ know we are done typing
            // using echo immediately sends EOF tells C++ line if finished
            std::string input;
            std::getline(std::cin, input); // C++ now reads only one line, after this line input is done

            PTR(Expr) e = parse_str(input); // convert stream input into string
            std::cout << e->interp(Env::empty)->to_string() << "\n"; // then interp
            return 0;
        }

        case do_print: {
            std::string input;
            std::getline(std::cin, input);

            PTR(Expr) e = parse_str(input);
            std::cout << e->to_string() << "\n";
            return 0;
        }

        case do_pretty_print: {
            // Parse expression from stdin, then pretty print it.
            std::string input;
            std::getline(std::cin, input);

            PTR(Expr) e = parse_str(input);
            std::cout << e->to_pretty_string() << "\n";
            return 0;
        }
        }

        // Should never get here, but keeps compiler happy
        return 1;
    }
    catch (std::exception const& ex) {
        std::cerr << ex.what() << "\n";
        return 1;
    }
    catch (...) {
        std::cerr << "unknown error\n";
        return 1;
    }
}



