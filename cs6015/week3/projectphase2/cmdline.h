


#ifndef CMDLINE_H // if this line has not been defined yet, continue 
#define CMDLINE_H // 

// The "mode" that main() should run in based on the flag.
typedef enum {
  do_test,
  do_interp,
  do_print,
  do_pretty_print
} run_mode_t;

// Takes command-line arguments and handles:
// --help : print help and exit(0)
// -- test : print "Test passed" once; error if repeated

// declare function. 
run_mode_t use_arguments(int argc, char **argv);


#endif