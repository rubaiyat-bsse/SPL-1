#include "DataProcessor.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>

#define CSV_COL 11  // bd_cities.csv has 11 columns
#define FRACTION_WAREHOUSES 0.8
#define FRACTION_CUSTOMERS 0.3

using namespace std;

vector<City> parseCSV(const string& filename){
    vector<City> cities;
    ifstream file(filename);
    
    if(!file.is_open()){
        cerr << "Error: Could not open file " << filename << endl;
        return cities;
    }

    string line;
    // skip header 
    getline(file, line);

    while(getline(file, line)){
        if(line.empty()) continue;

        stringstream ss(line);
        string token;
        vector<string> tokens;

        while(getline(ss, token, ',')){
            tokens.push_back(token);
        }

        if(tokens.size() == CSV_COL){
            City c;
            c.name = tokens[1];
            c.lat = stod(tokens[2]);
            c.lng = stod(tokens[3]);
            c.admin_name = tokens[7];
            c.capital = tokens[8];
            c.population = stod(tokens[9]);
            c.id = tokens[10];

            cities.push_back(c);
        }
    }
    file.close();
    return cities;
}

vector<Facility> selectFacilities(const vector<City>& allCities){
    vector<City> potentials;
    
    // filter by capital status
    for(const auto& city : allCities){
        if (city.capital == "primary" || city.capital == "admin" || city.capital == "minor") {
            potentials.push_back(city);
        }
    }

    srand(time(NULL));

    // fisher-yates shuffle
    for(int i = potentials.size() - 1; i > 0; i--){
        int j = rand() % (i + 1);
        swap(potentials[i], potentials[j]);
    }

    // take what wirehouse percentage is
    int k = potentials.size() * FRACTION_WAREHOUSES;
    vector<Facility> facilities;
    
    for(int i = 0; i < k; i++){
        Facility f;
        f.city = potentials[i];
        f.f = 0.0; // to be populated later
        f.M = 0.0; // to be populated later
        facilities.push_back(f);
    }

    return facilities;
}

vector<Customer> selectCustomers(const vector<City>& allCities){
    vector<City> potentials = allCities;
    
    // useing a different seed offset to avoid identical shuffles if called in the same second
    srand(time(NULL) + 12345);

    for(int i = potentials.size() - 1; i > 0; i--){
        int j = rand() % (i + 1);
        swap(potentials[i], potentials[j]);
    }

    int k = potentials.size() * FRACTION_CUSTOMERS;
    vector<Customer> customers;

    for(int i = 0; i < k; i++){
        Customer c;
        c.city = potentials[i];
        c.d = 0.0; // to be populated later
        customers.push_back(c);
    }

    return customers;
}
