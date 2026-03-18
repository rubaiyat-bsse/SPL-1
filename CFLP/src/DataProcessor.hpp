#ifndef DATA_PROCESSOR_HPP
#define DATA_PROCESSOR_HPP

#include <string>
#include <vector>

struct City {
    std::string name;
    double lat;
    double lng;
    std::string admin_name;
    std::string capital;
    double population;
    std::string id;
};

struct Facility {
    City city;
    double f; // Fixed cost
    double M; // Capacity
};

struct Customer {
    City city;
    double d; // Demand
};

std::vector<City> parseCSV(const std::string& filename);

std::vector<Facility> selectFacilities(const std::vector<City>& allCities);

std::vector<Customer> selectCustomers(const std::vector<City>& allCities);

#endif
