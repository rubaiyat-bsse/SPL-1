#include "ParameterGenerator.hpp"
#include <cmath>
#include <cstdlib>
#include <map>
#include <string>

using namespace std;

#define PI 3.14159265358979323846
#define COST_PER_WAREHOUSE 1000000.0
#define SUPPLY_FACTOR_PER_WAREHOUSE 3

static double toRadians(double degrees){
    return degrees * PI / 180.0;
}

double calculateHaversine(double lat1, double lon1, double lat2, double lon2){
    const double R = 6371.0; // earth radius in km

    double dLat = toRadians(lat2 - lat1);
    double dLon = toRadians(lon2 - lon1);

    double a = sin(dLat / 2) * sin(dLat / 2) +
               cos(toRadians(lat1)) * cos(toRadians(lat2)) *
               sin(dLon / 2) * sin(dLon / 2);
    
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return R*c;
}

void generateCustomerParameters(vector<Customer>& customers){
    
    // demand = round(0.02 * population) + uniform_random(-10, 10)
    for(int i = 0; i < customers.size(); ++i){
        
        int baseDemand = (int)(0.02 * customers[i].city.population);
        
        int randomOffset = (rand() % 21) - 10; // random offset between -10 and 10
        
        int finalDemand = baseDemand + randomOffset;
        if(finalDemand < 0) finalDemand = 0; // ignore if demands are negative
        
        customers[i].d = finalDemand;
    }
}

void generateFacilityParameters(vector<Facility>& facilities, const vector<Customer>& customers){
    map<string, double> regionTotalDemand;
    map<string, int> regionTotalCustomer;

    for (int i = 0; i < customers.size(); i++){
        regionTotalDemand[customers[i].city.admin_name] += customers[i].d;
        regionTotalCustomer[customers[i].city.admin_name]++;
    }

    for(int j = 0; j < facilities.size(); j++){
        
        facilities[j].f = COST_PER_WAREHOUSE; 
        
        string regionName = facilities[j].city.admin_name;
        double averageDemand = 0.0;
        
        if(regionTotalCustomer[regionName] > 0){
            averageDemand = regionTotalDemand[regionName] / (double)regionTotalCustomer[regionName];
        }
        
        // facility capacity = 3 * division_average_demand
        facilities[j].M = SUPPLY_FACTOR_PER_WAREHOUSE * averageDemand;
    }
}

vector<vector<double>> generateCostMatrix(const vector<Customer>& customers, const vector<Facility>& facilities, double costPerKm){
    vector<vector<double>> costMatrix;
    costMatrix.assign(customers.size(), vector<double>(facilities.size(), 0.0));
    
    for(int i = 0; i < customers.size(); i++){
        for(int j = 0; j < facilities.size(); j++){
            double dist = calculateHaversine(customers[i].city.lat, customers[i].city.lng,facilities[j].city.lat, facilities[j].city.lng);
            
            costMatrix[i][j] = dist * costPerKm;
        }
    }
    
    return costMatrix;
}
