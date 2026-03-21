#ifndef KML_GENERATOR_HPP
#define KML_GENERATOR_HPP

#include "DataProcessor.hpp"
#include <vector>
#include <string>

void generateKML(const std::vector<Facility>& facilities, 
                 const std::vector<Customer>& customers, 
                 const std::vector<int>& bestY, 
                 const std::vector<std::vector<double>>& bestX, 
                 const std::string& filename);

#endif
