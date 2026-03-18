#ifndef PARAMETER_GENERATOR_HPP
#define PARAMETER_GENERATOR_HPP

#include <vector>
#include "DataProcessor.hpp"

double calculateHaversine(double lat1, double lon1, double lat2, double lon2);

// Generate demands for customers
void generateCustomerParameters(std::vector<Customer>& customers);

// Generate capacities and fixed costs for facilities based on customer demands
void generateFacilityParameters(std::vector<Facility>& facilities, const std::vector<Customer>& customers);

// Create the transportation cost matrix (c_ij) 
// costPerKm multiplier determines the transportation cost scale
std::vector<std::vector<double>> generateCostMatrix(const std::vector<Customer>& customers, const std::vector<Facility>& facilities, double costPerKm);

#endif // PARAMETER_GENERATOR_HPP
