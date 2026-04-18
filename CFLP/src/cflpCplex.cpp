#include<ilcplex/ilocplex.h>
#include<bits/stdc++.h>
#include "DataProcessor.hpp"
#include "ParameterGenerator.hpp"
#include "KMLGenerator.hpp"

ILOSTLBEGIN

int main(){
    freopen("out/cplexOut.txt", "w", stdout);
    srand(42); // using a fixed seed so CPLEX and Lagrangian get the exact same demands & capacities

    // load data 
    string csvPath = "data/495 UPAZILA BD WITH LAT LONG.csv";
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

    double costPerKm = 10.0;
    vector<vector<double>> c = generateCostMatrix(customers, facilities, costPerKm);

    int I = customers.size();
    int J = facilities.size();

    cout << "Loaded " << I << " customers and " << J << " facilities.\n";
    cout << "Running exact CPLEX Solver for Baseline Comparison...\n";

    IloEnv env;
    try{
        IloModel model(env);

        IloArray<IloNumVarArray> x(env, J);
        for(int j=0; j<J; j++)
            x[j] = IloNumVarArray(env, I, 0.0, IloInfinity, ILOFLOAT);

        IloBoolVarArray y(env, J);

        //objective function
        IloExpr obj(env);
        for(int j=0; j<J; j++){
            obj+=facilities[j].f*y[j];
            for(int i=0; i<I; i++){
                obj+=c[i][j]*x[j][i];
            }
        }
        model.add(IloMinimize(env, obj));
        obj.end();

        //constraints
        for(int i=0; i<I; i++){
            IloExpr expr(env);
            for(int j=0; j<J; j++){
                expr+=x[j][i];
            }
            model.add(expr==customers[i].d);
            expr.end();
        }

        for(int j=0; j<J; j++){
            IloExpr expr(env);
            for(int i=0; i<I; i++){
                expr+=x[j][i];
            }
            model.add(expr<=facilities[j].M*y[j]);
            expr.end();
        }

        IloCplex cplex(model);
        // cplex.setOut(env.getNullStream());

        if(!cplex.solve()){
            cout<<"No feasible solution"<<endl;
            return 0;
        }

        cout << "CPLEX Exact Optimal Objective Value = " << fixed << setprecision(6) << cplex.getObjValue() << endl;

        vector<int> bestY(J, 0);
        vector<vector<double>> bestX(I, vector<double>(J, 0));

        cout<< "Opened facilities:"<<endl;
        for(int j=0; j<J; j++){
            if(cplex.getValue(y[j])>0.5){
                bestY[j] = 1;
                cout<<"Facility "<<j<<" ("<<facilities[j].city.name<<") opened\n";
            }
        }

        for(int j=0; j<J; j++){
            for(int i=0; i<I; i++){
                double val = cplex.getValue(x[j][i]);
                if(val> 1e-6){
                    bestX[i][j] = val;
                }
            }
        }

        generateKML(facilities, customers, bestY, bestX, "out/optimized_cflp_cplex.kml");

    } catch(IloException &e){
        cerr << "CPLEX Error: "<<e<<endl;
    }
    env.end();
    return 0;
}