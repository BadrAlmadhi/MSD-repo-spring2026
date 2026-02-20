#define CATCH_CONFIG_RUNNER
#include "catch.h"

#include <cstring>
#include <iostream>
#include <string>
#include "cmdline.h"
#include "expr.hpp"
#include "parse.hpp"

int main(int argc, char **argv) {
    try {
        // Decide what to do based on the command-line flag
        run_mode_t mode = use_arguments(argc, argv);

        switch (mode) {
        case do_test:
            // Run Catch2 tests.
            // Return 0 if all pass, non-zero otherwise.
            return Catch::Session().run();

        case do_interp: {
            // Read an expression from stdin, parse it,
            // interpret it, and print the resulting value.
            Expr* e = parse(std::cin);
            std::cout << e->interp() << "\n";
            return 0;
        }

        case do_print: {
            // Parse expression from stdin, then print it back using printExpr
            Expr* e = parse(std::cin);
            e->printExpr(std::cout);
            std::cout << "\n";
            return 0;
        }

        case do_pretty_print: {
            // Parse expression from stdin, then pretty print it.
            Expr* e = parse(std::cin);
            e->pretty_print(std::cout);
            std::cout << "\n";
            return 0;
        }
        }

        // Should never get here, but keeps compiler happy
        return 1;
    }
    catch (std::exception &ex) {
        // Assignment says:
        // - print error to STANDARD OUTPUT (std::cout)
        // - exit with non-zero
        std::cout << "Error: " << ex.what() << "\n";
        return 1;
    }
}


