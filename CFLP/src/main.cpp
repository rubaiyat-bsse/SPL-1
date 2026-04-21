#include "lagrangeCFLP.hpp"
#include <string>

int main(){
    std::string defaultPath = "data/south-asia-cities.csv";
    lagrange_RH_1(defaultPath);
    lagrange_RH_2(defaultPath);
    return 0;
}
