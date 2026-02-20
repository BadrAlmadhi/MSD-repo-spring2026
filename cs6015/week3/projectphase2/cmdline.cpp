

#include "cmdline.h"

#include <cstdlib> // allow us to use std:exit(n)
#include <cstring> // allow us to use std::strcmp(char*, char*) used for argv cmd
#include <iostream> // cout, cin


run_mode_t use_arguments(int argc, char **argv) {
    // We only accept exactly 1 flag after the program name.
    // Example: ./msdscript --interp
    if (argc != 2)
        throw std::runtime_error("must supply exactly one flag");

    if (std::strcmp(argv[1], "--test") == 0)
        return do_test;

    if (std::strcmp(argv[1], "--interp") == 0)
        return do_interp;

    if (std::strcmp(argv[1], "--print") == 0)
        return do_print;

    if (std::strcmp(argv[1], "--pretty-print") == 0)
        return do_pretty_print;

    throw std::runtime_error("unknown flag");
}

