#include<ilcplex/ilocplex.h>
#include<bits/stdc++.h>
#include <chrono>

using namespace std;

ILOSTLBEGIN

pair<double, double> runKnapsackCplex(const string& inputFile){
    ifstream ifs(inputFile);
    if(!ifs.is_open()){
        cerr << "Could not open " << inputFile << endl;
        return {-1.0, -1.0};
    }
    int n, W;
    if(!(ifs>>n>>W)) return {-1.0, -1.0};

    vector<int> weight(n), value(n);

    for(int i=0; i<n; i++){
        ifs>>weight[i]>>value[i];
    }
    ifs.close();

    IloEnv env;
    double objVal = -1.0;
    double executionTime = -1.0;

    try{
        IloModel model(env);
        IloBoolVarArray x(env, n);

        IloExpr obj(env);
        for(int i=0; i<n; i++) {
            obj += value[i]*x[i];
        }
        model.add(IloMaximize(env, obj));
        obj.end();

        IloExpr w(env);
        for(int i=0; i<n; i++){
            w+=weight[i]*x[i];
        }
        model.add(w<=W);
        w.end();

        IloCplex cplex(model);
        cplex.setOut(env.getNullStream());

        auto startTime = std::chrono::high_resolution_clock::now();

        if (cplex.solve()) {
            auto endTime = std::chrono::high_resolution_clock::now();
            executionTime = std::chrono::duration<double>(endTime - startTime).count();
            objVal = cplex.getObjValue();

            // Write output to log instead of stdout
            string logBaseDir = "/home/ratul/IIT/spl-1/CFLP/out/";
            ofstream cplexLog(logBaseDir + "cplexKnapsackOut.txt", ios::trunc);

            cplexLog << "CPLEX Knapsack Exact Optimal Objective Value = " << fixed << setprecision(6) << objVal << endl;
            cplexLog << "MIP gap: " << cplex.getMIPRelativeGap() << endl;
            cplexLog << "Weight Limit: " << W << "\n\n";
            
            cplexLog << "Selected items:\n";
            cplexLog << left << setw(8) << "ID" << setw(10) << "Weight" << setw(10) << "Value" << "\n";
            cplexLog << "--------------------------\n";

            double totalWeight=0;
            for(int i=0; i<n; i++){
                if(cplex.getValue(x[i])>0.5){
                    cplexLog << left << setw(8) << i << setw(10) << weight[i] << setw(10) << value[i] << endl;                
                    totalWeight += weight[i];
                } 
            }
            cplexLog << "--------------------------\n";
            cplexLog << left << setw(8) << "TOTAL" << setw(10) << totalWeight << setw(10) << objVal << "\n";
            cplexLog.close();
        } else {
            cout << "No feasible solution found by CPLEX for Knapsack.\n";
        }
        
    }
    catch(IloException& e){
        cerr<< "CPLEX Error: " << e <<endl;
    }

    env.end();
    return {objVal, executionTime};

}

