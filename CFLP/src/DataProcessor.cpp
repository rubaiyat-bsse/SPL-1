#include "DataProcessor.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>

#define CSV_COL 6  // bd_cities.csv has 11 columns , new one has 6
#define FRACTION_WAREHOUSES 0.1
#define FRACTION_CUSTOMERS 0.9

using namespace std;

// OLD DATASET: bd_cities.csv
// NEW DATASET: 495 UPAZILA BD WITH LAT LONG.csv
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

    srand(time(NULL));

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
            c.name = tokens[0];
            c.lat = stod(tokens[1]);
            c.lng = stod(tokens[2]);
            c.admin_name = tokens[4];
            c.capital = "";

            // synthetic population: between 200000 and 450000 -> average population of bd upazila
            c.population = 200000 + (rand() % 250001); 
            c.id = "";

            cities.push_back(c);
        }
    }
    file.close();
    return cities;
}

void writeCSV(const string& filename, const vector<string>& row, bool append){
    ofstream file;
    if(append){
        file.open(filename, ios::app);
    }
    else{
        file.open(filename, ios::trunc);
    }

    if(!file.is_open()) return;

    for(size_t i = 0; i < row.size(); i++){
        file << row[i];
        if(i != row.size()-1){
            file << ",";
        }
    }
    file << endl;
    file.close();
}

/*
// OLD SELECTION: filter by capital, 80% facilities. from all cities 30% customers
vector<Facility> selectFacilities(const vector<City>& allCities){
    vector<City> potentials;
    
    // filter by capital status
    for(const auto& city : allCities){
        if (city.capital == "primary" || city.capital == "admin" || city.capital == "minor"){
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
*/

// NEW SELECTION: 10% of total data for facilities, the rest exactly (90%) for customers
vector<Facility> selectFacilities(const vector<City>& allCities){
    vector<City> potentials = allCities;

    /*
     using a fixed predictable seed so that both selectFacility and selectCustomer functions shuffle the city array
     the same way. so we take the first 10% for facility from front and the remaining 90% for cutomer from back
    */
    srand(12345);

    // fisher-yates
    for(int i = potentials.size() - 1; i > 0; i--){
        int j = rand() % (i + 1);
        swap(potentials[i], potentials[j]);
    }

    int k = potentials.size() * FRACTION_WAREHOUSES;
    vector<Facility> facilities;
    
    for(int i = 0; i < k; i++){
        Facility f;
        f.city = potentials[i];
        f.f = 0.0; 
        f.M = 0.0; 
        facilities.push_back(f);
    }

    return facilities;
}

vector<Customer> selectCustomers(const vector<City>& allCities){
    vector<City> potentials = allCities;
    
    srand(12345);

    for(int i = potentials.size() - 1; i > 0; i--){
        int j = rand() % (i + 1);
        swap(potentials[i], potentials[j]);
    }

    int k = potentials.size() * FRACTION_WAREHOUSES; 
    vector<Customer> customers;

    // start taking from k through the remaining 90%
    for(int i = k; i < potentials.size(); i++){
        Customer c;
        c.city = potentials[i];
        c.d = 0.0; 
        customers.push_back(c);
    }

    return customers;
}
