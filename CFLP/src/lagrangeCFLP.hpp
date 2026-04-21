#pragma once
#include <string>
#include <utility>

std::pair<double, double> lagrange_RH_1(const std::string& csvPath);
std::pair<double, double> lagrange_RH_2(const std::string& csvPath);
std::pair<double, double> lagrange_RH_3(const std::string& csvPath);
std::pair<double, double> runCplex(const std::string& csvPath);
