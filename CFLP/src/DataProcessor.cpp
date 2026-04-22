#include "DataProcessor.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>

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

    srand(12345);

    while(getline(file, line)){
        if(line.empty()) continue;

        stringstream ss(line);
        string token;
        vector<string> tokens;

        while(getline(ss, token, ',')){
            tokens.push_back(token);
        }
        
        if (!line.empty() && line.back() == ',') {
            tokens.push_back("");
        }

        // city,city_ascii,lat,lng,country,iso2,iso3,admin_name,capital,population,id
        if(tokens.size() >= 11){
            City c;
            c.name = tokens[0];
            c.lat = stod(tokens[2]);
            c.lng = stod(tokens[3]);
            c.admin_name = tokens[7];
            c.capital = tokens[8];

            if (!c.capital.empty() && c.capital.back() == '\r') {
                c.capital.pop_back();
            }

            c.population = stoi(tokens[9]);
            c.id = tokens[10];

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

vector<Facility> selectFacilities(const vector<City>& allCities){
    vector<Facility> facilities;
    
    for(const auto& city : allCities){
        if (city.capital == "primary" || city.capital == "admin" || city.capital == "minor"){
            Facility f;
            f.city = city;
            f.f = 0.0; 
            f.M = 0.0; 
            facilities.push_back(f);
        }
    }

    return facilities;
}

vector<Customer> selectCustomers(const vector<City>& allCities){
    vector<Customer> customers;

    for(const auto& city : allCities){
        if (!(city.capital == "primary" || city.capital == "admin" || city.capital == "minor")){
            Customer c;
            c.city = city;
            c.d = 0.0; 
            customers.push_back(c);
        }
    }

    return customers;
}
