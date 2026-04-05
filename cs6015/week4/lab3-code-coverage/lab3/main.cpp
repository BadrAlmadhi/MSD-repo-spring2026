#include <cstring>   // strcmp
#include <cstdlib>   // exit

#define CATCH_CONFIG_RUNNER
#include "catch.h"


// command in terminal
int main(int argc, char** argv) {
    // Check for --test
    if (argc > 1 && std::strcmp(argv[1], "--test") == 0) {
        // Run Catch tests WITHOUT the --test argument
        int result = Catch::Session().run(argc - 1, argv + 1);

        if (result != 0) {
            exit(1);   // assignment requirement
        }
        return 0;
    }

    // Normal program mode (no tests)
    return 0;
}




