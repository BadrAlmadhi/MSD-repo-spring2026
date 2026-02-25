#include <iostream>
#include "exec.h"

int main (int argc, char** argv) {
    if (argc !=2 ) {
        std::cerr << "usage: " << argv[0] << "<msdscript-path>\n";
        return 1;
    }
    const char* msd = argv[1];

    // run msdscript --interp
    const char* args[3];
    args[0] = msd;
    args[1] = "--interp";
    args[2] = nullptr;

    ExecResult r = exec_program(2, args, "1+2");

    std::cout << "Exit code: " << r.exit_code << "\n";
    std::cout << "Output: " << r.out << "\n";
    std::cout << "Error: " << r.err << "\n";

    return 0;
}