#ifndef CPLEX_KNAPSACK_HPP
#define CPLEX_KNAPSACK_HPP

#include <string>
#include <utility>

std::pair<double, double> runKnapsackCplex(const std::string& inputFile);

#endif