#include <bits/stdc++.h>
#include "DataProcessor.hpp"
#include "ParameterGenerator.hpp"

using namespace std;

#define MAX_ITERATION 200
#define THRESHOLD 1e-4

struct RelaxationResult{
    double LB;
    double UB;
    vector<double> lambda;
    vector<int> bestY;
    vector<vector<double>> bestX;
};

double polyakStepSize(double UB, double LB, const vector<double>& g){
    double norm=0;
    for(double v:g) norm+=v*v;
    if(norm==0) return 0;
    return (UB-LB)/norm;
}

void writeCSV(const string& filename, const vector<string>& row, bool append = true){
    ofstream file;
    if(append){
        file.open(filename, ios::app);
    }
    else{
        file.open(filename, ios::trunc);
    }

    if(!file.is_open()) return;

    for(size_t i = 0; i < row.size(); i++) {
        file << row[i];
        if(i != row.size()-1) {
            file << ",";
        }
    }
    file << "\n";
    file.close();
}

void logIteration(
    int iter,
    double L,
    double primal,
    double LB,
    double UB,
    double gap,
    double step
){
    static bool headerPrinted = false;

    if(!headerPrinted){
        writeCSV("out/lagrange_log.csv", {"iter", "lagrangian", "primal", "LB", "UB", "gap", "step"}, false);
        headerPrinted = true;
    }

    writeCSV("out/lagrange_log.csv", {
        to_string(iter),
        to_string(L),
        to_string(primal),
        to_string(LB),
        to_string(UB),
        to_string(gap),
        to_string(step)
    });
}

vector<int> solveFacilitySubproblem(const vector<Facility>& facilities, const vector<double>& lambda){
    int m = facilities.size();
    vector<int> y(m, 0);

    for(int j=0; j<m; j++){
        if(facilities[j].f - lambda[j]*facilities[j].M < 0)
            y[j]=1;
    }

    return y;
}

vector<vector<double>> solveAssignmentSubproblem(
    const vector<Customer>& customers,
    const vector<vector<double>>& c,
    const vector<double>& lambda
){
    int n = customers.size();
    int m = lambda.size();

    vector<vector<double>> x(n, vector<double>(m, 0));

    for(int i=0; i<n; i++){
        int bestJ=0;
        double bestCost = DBL_MAX;

        for(int j=0; j<m; j++){
            double cbar = c[i][j] + lambda[j];

            if(cbar<bestCost){
                bestCost = cbar;
                bestJ=j;
            }
        }

        x[i][bestJ] = customers[i].d;
    }

    return x;
}

double calculateLagrangianValue(
    const vector<Facility>& facilities,
    const vector<vector<double>>& c,
    const vector<double>& lambda,
    const vector<int>& y,
    const vector<vector<double>>& x
){
    int n = c.size();
    int m = facilities.size();

    double L=0;

    for(int j=0; j<m; j++){
        L += (facilities[j].f - lambda[j]*facilities[j].M)*y[j];
    }

    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            L += (c[i][j] + lambda[j])*x[i][j];
        }
    }

    return L;
}

pair<vector<int>, vector<vector<double>>> repairHeuristic(
    const vector<Facility>& facilities,
    const vector<vector<double>>& c,
    const vector<int>& y,
    const vector<vector<double>>& x
){
    int n = x.size();
    int m = facilities.size();

    vector<int> yRepair = y;
    vector<vector<double>> xRepair = x;

    for(int j=0; j<m; j++){
        double load=0;

        for(int i=0; i<n; i++){
            load+=xRepair[i][j];
        }

        if(load>0){
            yRepair[j]=1;
        }
    }

    for(int j=0; j<m; j++){
        double load=0;

        for(int i=0; i<n; i++){
            load+=xRepair[i][j];
        }

        if(load>facilities[j].M){
            double excess = load - facilities[j].M;

            for(int i=0; i<n && excess>0; i++){
                if(xRepair[i][j]>0){
                    double move = min(excess, xRepair[i][j]);

                    xRepair[i][j]-=move;
                    excess-=move;

                    int bestJ=-1;
                    double bestCost=DBL_MAX;

                    for(int k=0; k<m; k++){
                        if(c[i][k] < bestCost){
                            bestCost=c[i][k];
                            bestJ=k;
                        }
                    }

                    if(bestJ!=-1) xRepair[i][bestJ]+=move;
                }
            }
        }
    }

    return {yRepair, xRepair};
}

double computePrimalValue(
    const vector<Facility>& facilities,
    const vector<vector<double>>& c,
    const vector<int>& yRepair,
    const vector<vector<double>>& xRepair
){
    int n = xRepair.size();
    int m = facilities.size();

    double primal=0;

    for(int j=0; j<m; j++)
        primal += facilities[j].f * yRepair[j];

    for(int i=0; i<n; i++)
        for(int j=0; j<m; j++)
            primal += c[i][j] * xRepair[i][j];

    return primal;
}

vector<double> computeSubgradient(
    const vector<Facility>& facilities,
    const vector<int>& y,
    const vector<vector<double>>& x
){
    int n = x.size();
    int m = facilities.size();
    vector<double> g(m, 0);

    for(int j=0; j<m; j++){
        double flow=0;

        for(int i=0; i<n; i++)
            flow+=x[i][j];

        g[j] = flow - facilities[j].M*y[j];
    }

    return g;
}

void updateLambda(vector<double>& lambda, double step, const vector<double>& g){
    int m = lambda.size();

    for(int j=0; j<m; j++)
        lambda[j] = max(0.0, lambda[j] + step*g[j]);
}

RelaxationResult lagrangeRelaxation(
    const vector<Customer>& customers,
    const vector<Facility>& facilities,
    const vector<vector<double>>& c
){
    int n = customers.size();
    int m = facilities.size();
    vector<double> lambda(m, 0);

    double LB = DBL_MIN;
    double UB = DBL_MAX;

    vector<int> bestY(m, 0);
    vector<vector<double>> bestX(n, vector<double>(m, 0));

    // clear previous log file on new run
    ofstream("out/lagrange_log.csv", ios::trunc).close();

    double prevLB = LB;
    double prevUB = UB;
    int noChangeCount = 0;
    const int MAX_NO_CHANGE = 5;

    for(int iter=0; iter<MAX_ITERATION; iter++){
        vector<int> y = solveFacilitySubproblem(facilities, lambda);
        vector<vector<double>> x = solveAssignmentSubproblem(customers, c, lambda);

        double L = calculateLagrangianValue(facilities, c, lambda, y, x);
        LB = max(LB, L);

        auto repaired = repairHeuristic(facilities, c, y, x);
        const vector<int>& yRepair = repaired.first;
        const vector<vector<double>>& xRepair = repaired.second;

        double primal = computePrimalValue(facilities, c, yRepair, xRepair);
        if (primal < UB){
            UB = primal;
            bestY = yRepair;
            bestX = xRepair;
        }

        double gap = (UB-LB)/UB;

        vector<double> g = computeSubgradient(facilities, y, x);
        double step = polyakStepSize(UB, LB, g);
        
        logIteration(iter, L, primal, LB, UB, gap, step);

        if(gap < THRESHOLD) break;
        
        // early stopping check
        if(abs(LB - prevLB) < 1e-6 && abs(UB - prevUB) < 1e-6){
            noChangeCount++;
            if (noChangeCount >= MAX_NO_CHANGE) {
                writeCSV("out/lagrange_log.csv", {"Stopping early: No change in bounds for " + to_string(MAX_NO_CHANGE) + " consecutive iterations."});
                break;
            }
        }
        else{
            noChangeCount = 0;
        }
        
        prevLB = LB;
        prevUB = UB;

        updateLambda(lambda, step, g);
    }

    return {LB, UB, lambda, bestY, bestX};
}

void printFinalResult(const RelaxationResult& result, const vector<Customer>& customers, const vector<Facility>& facilities, const vector<vector<double>>& c) {
    int n = customers.size();
    int m = facilities.size();
    
    string outFile = "out/lagrangeOutput.csv";
    writeCSV(outFile, {"Optimal Objective Value (Upper Bound)", to_string(result.UB)}, false);
    writeCSV(outFile, {});

    writeCSV(outFile, {"Facility Load and Cost Details:"});
    writeCSV(outFile, {"Facility ID", "City", "Opened", "Load", "Capacity", "Total Cost (Fixed + Routing)"});
    for (int j = 0; j < m; j++) {
        if (result.bestY[j] > 0) {
            double load = 0;
            double cost = facilities[j].f;
            for (int i = 0; i < n; i++) {
                if (result.bestX[i][j] > 1e-6) {
                    load += result.bestX[i][j];
                    cost += c[i][j] * result.bestX[i][j];
                }
            }
            writeCSV(outFile, {to_string(j), facilities[j].city.name, "Yes", to_string(load), to_string(facilities[j].M), to_string(cost)});
        }
    }

    writeCSV(outFile, {});
    writeCSV(outFile, {"Assignments (x[j][i]):"});
    writeCSV(outFile, {"Facility ID (j)", "Facility City", "Customer ID (i)", "Customer City", "Assignment Value"});
    for (int j = 0; j < m; j++) {
        for (int i = 0; i < n; i++) {
            if (result.bestX[i][j] > 1e-6) {
                writeCSV(outFile, {to_string(j), facilities[j].city.name, to_string(i), customers[i].city.name, to_string(result.bestX[i][j])});
            }
        }
    }
    
    cout << "Final results successfully written to " << outFile << "\n";
}

int main(){
    srand(time(NULL));

    // process Data
    string csvPath = "data/bd_cities.csv";
    vector<City> allCities = parseCSV(csvPath);
    if(allCities.empty()) {
        cerr << "Failed to parse cities or file is empty.\n";
        return 1;
    }

    vector<Facility> facilities = selectFacilities(allCities);
    vector<Customer> customers = selectCustomers(allCities);

    // generate Parameters
    generateCustomerParameters(customers);
    generateFacilityParameters(facilities, customers);

    double costPerKm = 10.0;    // assume
    vector<vector<double>> c = generateCostMatrix(customers, facilities, costPerKm);

    int n = customers.size();
    int m = facilities.size();

    cout << "Loaded " << n << " customers and " << m << " facilities.\n";
    cout << "Running Lagrangian Relaxation Solver...\n";

    RelaxationResult result = lagrangeRelaxation(customers, facilities, c);
    
    printFinalResult(result, customers, facilities, c);

    return 0;
}