#define CATCH_CONFIG_RUNNER
#include "catch.h"

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
        case do_test:{
            // Run Catch2 tests.
            // Return 0 if all pass, non-zero otherwise.
            int rc = Catch::Session().run();
            return (rc = 0) ? 0 : 1; // this now enforce 0/1 only
        }

        case do_interp: {
            Expr* e = parse(std::cin);
            std::cout << e->interp()->to_string() << "\n";
            return 0;
        }

        case do_print: {
            Expr* e = parse(std::cin);
            std::cout << e->to_string() << "\n";
            return 0;
        }

        case do_pretty_print: {
            // Parse expression from stdin, then pretty print it.
            Expr* e = parse(std::cin);
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


