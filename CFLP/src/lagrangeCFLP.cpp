#include<bits/stdc++.h>
using namespace std;

#define MAX_ITERATION 200
#define THRESHOLD 1e-4

struct Facility{
    double f;
    double M;
};

struct Customer{
    double d;
};

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

    ofstream logFile("out/lagrange_log.txt");

    for(int iter=0; iter<MAX_ITERATION; iter++){
        vector<int> y = solveFacilitySubproblem(facilities, lambda);
        vector<vector<double>> x = solveAssignmentSubproblem(customers, c, lambda);

        double L = calculateLagrangianValue(facilities, c, lambda, y, x);
        LB = max(LB, L);

        auto repaired = repairHeuristic(facilities, c, y, x);
        const vector<int>& yRepair = repaired.first;
        const vector<vector<double>>& xRepair = repaired.second;

        double primal = computePrimalValue(facilities, c, yRepair, xRepair);
        if (primal < UB) {
            UB = primal;
            bestY = yRepair;
            bestX = xRepair;
        }

        if(logFile.is_open()){
            logFile << "Iteration " << iter << ": Lower Bound = " << LB << ", Upper Bound = " << UB << "\n";
        }

        double gap = (UB-LB)/UB;
        if(gap < THRESHOLD) break;

        vector<double> g = computeSubgradient(facilities, y, x);
        double step = polyakStepSize(UB, LB, g);
        updateLambda(lambda, step, g);
    }

    if (logFile.is_open()) logFile.close();

    return {LB, UB, lambda, bestY, bestX};
}

void printFinalResult(const RelaxationResult& result, int n, int m, const vector<Facility>& facilities, const vector<vector<double>>& c) {
    ofstream outFile("out/lagrangeOutput.txt");
    if (!outFile.is_open()) {
        cerr << "Error: Could not open out/lagrangeOutput.txt for writing.\n";
        return;
    }

    outFile << "Optimal Objective Value (Upper Bound) = " << result.UB << "\n";
    outFile << "Opened facilities:\n";
    for (int j = 0; j < m; j++) {
        if (result.bestY[j] > 0) {
            outFile << "Facility " << j << " opened\n";
        }
    }

    outFile << "\nFacility Load and Cost Details:\n";
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
            outFile << "Facility " << j << " -> Load: " << load << " / " << facilities[j].M << ", Total Cost (Fixed + Routing): " << cost << "\n";
        }
    }

    outFile << "\nAssignments (x[j][i]):\n";
    for (int j = 0; j < m; j++) {
        for (int i = 0; i < n; i++) {
            if (result.bestX[i][j] > 1e-6) {
                outFile << "x[" << j << "][" << i << "] = " << result.bestX[i][j] << "\n";
            }
        }
    }
    
    outFile.close();
    cout << "Final results successfully written to out/lagrangeOutput.txt\n";
}

int main(){

    int n, m;
    cin>>n>>m;

    vector<Customer> customers(n);
    vector<Facility> facilities(m);

    for(int i=0; i<n; i++) cin>>customers[i].d;

    for(int j=0; j<m; j++) cin>>facilities[j].f;

    for(int j=0; j<m; j++) cin>>facilities[j].M;

    vector<vector<double>> c(n, vector<double>(m));

    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            cin>>c[i][j];
        }
    }

    RelaxationResult result = lagrangeRelaxation(customers, facilities, c);
    printFinalResult(result, n, m, facilities, c);

    return 0;
}