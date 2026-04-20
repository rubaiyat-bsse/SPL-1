#include <iostream>
#include <string>
#include <vector>

#include "lagrangeCFLP.hpp"

using namespace std;

void printHelp(){
    cout << "Usage: lagrange-cli <command> [options]\n\n";
    cout << "Commands:\n";
    cout << "  cflp     Solve Capacitated Facility Location Problem\n";
    cout << "  knapsack Solve Knapsack Problem (Placeholder)\n\n";
    cout << "Options for cflp:\n";
    cout << "  -i, --input <file>     Path to input CSV file\n";
    cout << "  -h, --help             Show this help message\n";
}

int main(int argc, char* argv[]){
    if (argc < 2) {
        cerr << "Error: No command provided.\n";
        printHelp();
        return 1;
    }

    // reading the first argument, (cflp or knapsack)
    string command = argv[1];

    if (command == "-h" || command == "--help") {
        printHelp();
        return 0;
    }

    if (command == "cflp") {
        string inputFile = "";

        // start from index 2 because 0 is the program name, and 1 is the command "cflp"
        for (int i = 2; i < argc; ++i) {
            string arg = argv[i];

            if ((arg == "-i" || arg == "--input") && i + 1 < argc) {
                inputFile = argv[++i];
            } else {
                cerr << "Unknown or incomplete argument: " << arg << "\n";
                printHelp();
                return 1;
            }
        }

        if (inputFile.empty()) {
            cerr << "Error: --input is required for cflp.\n";
            return 1;
        }

        cout << "Running CFLP command...\n";
        cout << "  Input:  " << inputFile << "\n";
        cout << "  Outputs will be saved in the '/home/ratul/IIT/spl-1/CFLP/out/' directory (logs, results, and KML).\n\n";

        // call the solver with the input file
        lagrange_RH_1(inputFile);
        lagrange_RH_2(inputFile);

    } else if (command == "knapsack") {
        cout << "Knapsack command is not yet implemented.\n";
    } else {
        cerr << "Error: Unknown command '" << command << "'.\n";
        printHelp();
        return 1;
    }

    return 0;
}
