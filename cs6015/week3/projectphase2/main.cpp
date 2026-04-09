
#include <cstring>
#include <iostream>
#include <string>
#include "cmdline.h"
#include "expr.hpp"
#include "parse.hpp"
#include "val.h"

int main(int argc, char **argv) {
    try {
        // Decide what to do based on the command-line flag
        run_mode_t mode = use_arguments(argc, argv);

        switch (mode) {
        
            case do_test: {
            std::cerr << "Run tests using the test executable\n";
            return 1;
        }

        case do_interp: {
            PTR(Expr) e = parse(std::cin);
            std::cout << e->interp()->to_string() << "\n";
            return 0;
        }

        case do_print: {
            PTR(Expr) e = parse(std::cin);
            std::cout << e->to_string() << "\n";
            return 0;
        }

        case do_pretty_print: {
            // Parse expression from stdin, then pretty print it.
            PTR(Expr) e = parse(std::cin);
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



