#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

#include "lagrangeCFLP.hpp"
#include "cflpCplex.hpp"
#include "../../knapsack/lagrange/src/lagrangeKnapsack.hpp"
#include "../../knapsack/lagrange/src/cplexKnapsack.hpp"

using namespace std;

void printHelp(){
    cout << "Usage: lagrange-cli <command> [options]\n\n";
    cout << "Commands:\n";
    cout << "  cflp     Solve Capacitated Facility Location Problem\n";
    cout << "  knapsack Solve Knapsack Problem (Placeholder)\n\n";
    cout << "Options for cflp:\n";
    cout << "  -i, --input <file>     Path to input CSV file\n";
    cout << "  -b, --benchmark        Run both CPLEX exact solver and Lagrangian side-by-side\n";
    cout << "  -h, --help             Show this help message\n";
}

int main(int argc, char* argv[]){
    if (argc < 2){
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

    if (command == "cflp"){
        string inputFile = "";
        bool isBenchmark = false;

        // start from index 2 because 0 is the program name, and 1 is the command "cflp"
        for (int i = 2; i < argc; ++i) {
            string arg = argv[i];

            if ((arg == "-i" || arg == "--input") && i + 1 < argc) {
                inputFile = argv[++i];
            } else if (arg == "-b" || arg == "--benchmark") {
                isBenchmark = true;
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

        if (isBenchmark) {
            cout << "=========================================================\n";
            cout << "                 PERFORMANCE BENCHMARK                   \n";
            cout << "=========================================================\n";
            
            cout << "-> Running exact CPLEX solver...\n";
            auto cplexResult = runCplex(inputFile);
            
            cout << "-> Running Lagrangian RH 1...\n";
            auto lh1Result = lagrange_RH_1(inputFile);
            
            cout << "-> Running Lagrangian RH 2...\n";
            auto lh2Result = lagrange_RH_2(inputFile);

            cout << "-> Running Lagrangian RH 3 (CPLEX Middle-Ground)...\n";
            auto lh3Result = lagrange_RH_3(inputFile);

            double gap1 = ((lh1Result.first - cplexResult.first) / cplexResult.first) * 100.0;
            double gap2 = ((lh2Result.first - cplexResult.first) / cplexResult.first) * 100.0;
            double gap3 = ((lh3Result.first - cplexResult.first) / cplexResult.first) * 100.0;

            double speedup1 = (lh1Result.second > 1e-6) ? (cplexResult.second / lh1Result.second) : 0.0;
            double speedup2 = (lh2Result.second > 1e-6) ? (cplexResult.second / lh2Result.second) : 0.0;
            double speedup3 = (lh3Result.second > 1e-6) ? (cplexResult.second / lh3Result.second) : 0.0;

            cout << "\n=================================== RESULTS ===================================\n";
            cout << left << setw(15) << "Solver" 
                 << setw(20) << "Objective Value" 
                 << setw(15) << "Time (s)" 
                 << setw(15) << "Gap to Opt."
                 << "Speedup\n";
            cout << "-------------------------------------------------------------------------------\n";
            
            cout << left << setw(15) << "CPLEX (Exact)" 
                 << setw(20) << fixed << setprecision(2) << cplexResult.first 
                 << setw(15) << fixed << setprecision(6) << cplexResult.second 
                 << setw(15) << "0.00%"
                 << "1.0x\n";
                 
            cout << left << setw(15) << "Lagrange RH1" 
                 << setw(20) << fixed << setprecision(2) << lh1Result.first 
                 << setw(15) << fixed << setprecision(6) << lh1Result.second 
                 << "+" << fixed << setprecision(4) << gap1 << "%      ";
            if (speedup1 > 1.0) cout << fixed << setprecision(1) << speedup1 << "x faster\n";
            else cout << "-\n";

            cout << left << setw(15) << "Lagrange RH2" 
                 << setw(20) << fixed << setprecision(2) << lh2Result.first 
                 << setw(15) << fixed << setprecision(6) << lh2Result.second 
                 << "+" << fixed << setprecision(4) << gap2 << "%      ";
            if (speedup2 > 1.0) cout << fixed << setprecision(1) << speedup2 << "x faster\n";
            else cout << "-\n";

            cout << left << setw(15) << "Lagrange RH3" 
                 << setw(20) << fixed << setprecision(2) << lh3Result.first 
                 << setw(15) << fixed << setprecision(6) << lh3Result.second 
                 << "+" << fixed << setprecision(4) << gap3 << "%      ";
            if (speedup3 > 1.0) cout << fixed << setprecision(1) << speedup3 << "x faster\n";
            else cout << "-\n";

            cout << "===============================================================================\n";
        } else {
            // call the solver with the input file
            lagrange_RH_1(inputFile);
            lagrange_RH_2(inputFile);
            lagrange_RH_3(inputFile);
        }

    } else if (command == "knapsack") {
        string inputFile = "";
        bool isBenchmark = false;

        for (int i = 2; i < argc; ++i) {
            string arg = argv[i];

            if ((arg == "-i" || arg == "--input") && i + 1 < argc) {
                inputFile = argv[++i];
            } else if (arg == "-b" || arg == "--benchmark") {
                isBenchmark = true;
            } else {
                cerr << "Unknown or incomplete argument: " << arg << "\n";
                printHelp();
                return 1;
            }
        }

        if (inputFile.empty()) {
            cerr << "Error: --input is required for knapsack.\n";
            return 1;
        }

        cout << "Running Knapsack command...\n";
        cout << "  Input:  " << inputFile << "\n";
        cout << "  Outputs will be saved in the '/home/ratul/IIT/spl-1/CFLP/out/' directory.\n\n";

        if (isBenchmark) {
            cout << "=========================================================\n";
            cout << "                 PERFORMANCE BENCHMARK                   \n";
            cout << "=========================================================\n";
            
            cout << "-> Running exact CPLEX solver...\n";
            auto cplexResult = runKnapsackCplex(inputFile);
            
            cout << "-> Running Lagrangian Relaxation solver...\n";
            auto lagrangeResult = runKnapsackLagrange(inputFile);

            double gap = ((cplexResult.first - lagrangeResult.first) / cplexResult.first) * 100.0;
            double speedup = (lagrangeResult.second > 1e-6) ? (cplexResult.second / lagrangeResult.second) : 0.0;

            cout << "\n=================================== RESULTS ===================================\n";
            cout << left << setw(15) << "Solver" 
                 << setw(20) << "Objective Value" 
                 << setw(15) << "Time (s)" 
                 << setw(15) << "Gap to Opt."
                 << "Speedup\n";
            cout << "-------------------------------------------------------------------------------\n";
            
            cout << left << setw(15) << "CPLEX (Exact)" 
                 << setw(20) << fixed << setprecision(2) << cplexResult.first 
                 << setw(15) << fixed << setprecision(6) << cplexResult.second 
                 << setw(15) << "0.00%"
                 << "1.0x\n";
                 
            cout << left << setw(15) << "Lagrangian" 
                 << setw(20) << fixed << setprecision(2) << lagrangeResult.first 
                 << setw(15) << fixed << setprecision(6) << lagrangeResult.second 
                 << fixed << setprecision(4) << gap << "%      ";
            if (speedup > 1.0) cout << fixed << setprecision(1) << speedup << "x faster\n";
            else cout << "-\n";

            cout << "===============================================================================\n";
        } else {
            runKnapsackLagrange(inputFile);
        }
    } else {
        cerr << "Error: Unknown command '" << command << "'.\n";
        printHelp();
        return 1;
    }

    return 0;
}
