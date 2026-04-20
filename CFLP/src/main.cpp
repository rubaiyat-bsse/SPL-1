#include "lagrangeCFLP.hpp"
#include <string>

int main(){
    std::string defaultPath = "data/495 UPAZILA BD WITH LAT LONG.csv";
    lagrange_RH_1(defaultPath);
    lagrange_RH_2(defaultPath);
    return 0;
}
