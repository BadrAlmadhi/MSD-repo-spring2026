#include <iostream>
#include <stdexcept>
#include <random>
#include "exec.h"

// random generator
static std::mt19937 gen(1234);

static int rand_int(int lo, int hi) {
    std::uniform_int_distribution<int> dist(lo,hi);
    return dist(gen);
}

// helper function run msd
static std::string random_expr_string(){
    int a = rand_int(0,10);
    int b = rand_int(0,10);

    if (rand_int(0,1) == 0) {
        return "(" + std::to_string(a) + " + " + std::to_string(b) + ")";
    } else {
        return "(" + std::to_string(a) + " * " + std::to_string(b) + ")";
    }
}

// run helper
static ExecResult run(const char* prog, const char* mode, const std::string& input) {
    const char* argv[3];
    argv[0] = prog;
    argv[1] = mode;
    argv[2] = nullptr;
    return exec_program(2, argv, input);
}

// check if not correct exit code
static void ensure_ok(const ExecResult& r, const std::string& where) {
    // Must be 0 or 1
    if (!(r.exit_code == 0 || r.exit_code == 1)) {
        throw std::runtime_error(where + ": exit code not 0/1");
    }
    // If exit_code is 1, treat as failure
    if (r.exit_code != 0) {
        throw std::runtime_error(where + " failed:\n" + r.err);
    }
}

int main (int argc, char** argv) {

    try {
        
        if (argc != 2 && argc != 3) {
             std::cerr << "usage:\n";
             std::cerr << "  " << argv[0] << " <path-to-msdscript>\n";
             std::cerr << "  " << argv[0] << " <msdA> <msdB>\n";
             return 1;
        }

        if (argc == 3) {
            const char* msdA = argv[1];
            const char* msdB = argv[2];
            const char* modes[] = {"--interp", "--print", "--pretty-print"};

            for (int i = 0; i < 100; i++) {
                // generate random
                std::string in = random_expr_string();
                const char* mode = modes[rand_int(0,2)];

                // assign to run
                ExecResult a = run(msdA, mode, in);
                ExecResult b = run(msdB, mode, in);

                // check
                ensure_ok(a, std::string("A ") + mode);
                ensure_ok(b, std::string("B ") + mode);

                // check if they are not the same
                if (a.out != b.out) {
                    std::cerr << "DISAGREEMENT FOUND\n";
                    std::cerr << "Mode: " << mode << "\n";
                    std::cerr << "Input: " << in << "\n\n";
                    std::cerr << "A output:\n" << a.out << "\n";
                    std::cerr << "B output:\n" << b.out << "\n";
                    return 1;

                }
            }
            std::cout << "No disagreements found\n";
            return 0;
        }

        // argument 1 mode
        const char* msd = argv[1];

        for (int i = 0; i < 1000; i++) {
            std::string in = random_expr_string();
            std::cout << "Trying " << in << "\n";
            
            ExecResult interp_result = run(msd, "--interp", in);
            ensure_ok(interp_result, "interp");

            ExecResult print_result = run(msd, "--print", in);
            ensure_ok(print_result, "print");

            ExecResult interp_again_result = run(msd, "--interp", print_result.out);
            ensure_ok(interp_again_result, "interp(print)");

            if (interp_again_result.out != interp_result.out) {
                std::cerr << "Mismatch!\n";
                std::cerr << "Input: " << in << "\n";
                std::cerr << "Printed: " << print_result.out << "\n";
                std::cerr << "interp(input): " << interp_result.out;
                std::cerr << "interp(print): " << interp_again_result.out;
                return 1;
            }
        }

        std::cout << "PASS\n";
        return 0;


    } catch (std::exception const& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
}