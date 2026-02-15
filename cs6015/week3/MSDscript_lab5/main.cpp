#define CATCH_CONFIG_RUNNER
#include "catch.h"

#include <cstring>
#include <iostream>
#include <string>
#include "cmdline.h"
#include "expr.hpp"

int main (int argc, char* argv[]) {

   // detect --test
   bool run_test = false;
   for (int i = 0; i < argc; i++) {
      if (std::strcmp(argv[i], "--test") == 0) {
         run_test = true;
         break;
      }
   }

   // if --test run Catch and Exit
   if (run_test) {
      int result = Catch::Session().run();
      if (result != 0) exit(1);
      return 0;
   }


   // otherwise normal program
   use_arguments(argc, argv);
   


   return 0;
}


