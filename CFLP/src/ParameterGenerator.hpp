#ifndef PARAMETER_GENERATOR_HPP
#define PARAMETER_GENERATOR_HPP

#include <vector>
#include "DataProcessor.hpp"

double calculateHaversine(double lat1, double lon1, double lat2, double lon2);

void generateCustomerParameters(std::vector<Customer>& customers);

void generateFacilityParameters(std::vector<Facility>& facilities, const std::vector<Customer>& customers);

std::vector<std::vector<double>> generateCostMatrix(const std::vector<Customer>& customers, const std::vector<Facility>& facilities, double costPerKm);

#endif
