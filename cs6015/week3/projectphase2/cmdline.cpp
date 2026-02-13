

#include "cmdline.h"

#include <cstdlib> // allow us to use std:exit(n)
#include <cstring> // allow us to use std::strcmp(char*, char*) used for argv cmd
#include <iostream> // cout, cin


void use_arguments(int argc, char* argv[]) {
    bool seen_test = false; // we will use to check test

    // start from 1 because 0 is program name
    for (int i = 1; i < argc; i++) {
        
        // point to stored variables each time loop runs
        // arg values changes every time loop runs
        const char* arg = argv[i]; 
      // test  //std::cout << "arguments: " << arg << "\n";

      // if user typed --help after arg0 print help args
      if (std::strcmp(arg, "--help") == 0) {
        std::cout << "msdscript program uses:\n" 
                  << " --help \n"
                  << " --test \n";
                  std::exit(0);
      } else if (std::strcmp(arg, "--test") == 0) {
        if (seen_test) {
            std::cerr << "Error: --test runs multiple times\n";
            std::exit(1);
        } 
        seen_test = true;
        std::cout << "Test passed\n";
      } else {
        std::cerr << "Unkown arguments: " << arg << "\n";
        std::exit(1);
      }
       
        return;
    }

return;
}

