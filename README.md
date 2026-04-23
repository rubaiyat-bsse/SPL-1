# SPL-1: Optimization Solvers (Knapsack & CFLP)

This project contains solvers for NP hard problems, specifically the **Capacitated Facility Location Problem (CFLP)** and the **0-1 Knapsack Problem**. It implements both using **Lagrangian Relaxation** heuristic and **IBM ILOG CPLEX** exact approaches . 

It builds into a unified command-line tool, `lagrange-cli`, to easily benchmark algorithms side-by-side using various datasets.

## Project Structure

A brief overview of the workspace structure:

*   **`CFLP/`**: Contains the main `Makefile`, the `lagrange-cli` entry point, data files, and implementations for the Capacitated Facility Location Problem (CPLEX exact solver, Lagrangian Relaxation, data processors, KML generators).
    *   `src/`: Main entry point and source files for CFLP.
    *   `data/`: Assorted datasets used for CFLP (such as BD cities coordinates, CSV inputs).
*   **`knapsack/`**: Contains exact (CPLEX) and heuristic (Lagrangian Relaxation) solvers for the Knapsack problem.
*   **`testing/`**: Assorted independent tests and manual algorithmic tests (like branch-and-bound).
*   **`resources/`**: Related Markdown files, context, and a Python-based implementation for comparative analysis.

## Prerequisites & Dependencies

To build and run the project, the following dependencies are required:

- **Compiler:** `g++` (Supports C++17)
- **Make:** Standard Unix make.
- **IBM ILOG CPLEX Studio 22.1.2:** Installed at `/opt/ibm/ILOG/CPLEX_Studio2212/` (Path configured in `CFLP/Makefile`).

> **Note:** If you do not have access to the CPLEX library, pre-compiled binaries are included in the `build/` directories (e.g., `CFLP/build/lagrange-cli`). You can run these directly without needing to compile the project yourself.
> 
> If you are compiling from source and using a different CPLEX installation, please update `CPLEX_INCDIR`, `CONCERT_INCDIR`, `CPLEX_LIBDIR`, and `CONCERT_LIBDIR` paths inside `CFLP/Makefile` accordingly.

## Building the CLI

To compile the `lagrange-cli` executable, simply run:

```bash
cd CFLP
make all
```

This will link and generate the executable at `CFLP/build/lagrange-cli`.

## Usage

The `lagrange-cli` program serves as the entry point for running your optimization scenarios. 

```bash
Usage: lagrange-cli <command> [options]

Commands:
  cflp     Solve Capacitated Facility Location Problem
  knapsack Solve Knapsack Problem (Placeholder)

Options for cflp:
  -i, --input <file>     Path to input CSV file
  -b, --benchmark        Run both CPLEX exact solver and Lagrangian side-by-side
  -h, --help             Show this help message
```

### Examples

**Run CFLP on a specific dataset using benchmark mode (Runs CPLEX first, then Lagrangian):**
```bash
./build/lagrange-cli cflp -i "data/bd_cities.csv" --benchmark
```

**Show CLI Help text:**
```bash
./build/lagrange-cli --help
```

## Features

*   **Capacitated Facility Location (CFLP):**
    *   Reads location data matrices via CSV inputs.
    *   `cflpCplex.cpp`: Exact mathematical formulation using CPLEX variables and constraints.
    *   `lagrangeCFLP.cpp`: Uses Lagrangian Relaxation on the constraints to establish a lower bound and derives feasible solutions.
    *   KML generation to visualize location problems geographically.
*   **Knapsack Problem:**
    *   Implemented via `cplexKnapsack.hpp` and `lagrangeKnapsack.hpp` within the `knapsack/` directory. Can handle typical knapsack problems.
