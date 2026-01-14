#include<ilcplex/ilocplex.h>
#include<bits/stdc++.h>

using namespace std;

ILOSTLBEGIN

int main(){
    int n, W;
    cin>>n>>W;

    vector<int> weight(n), value(n);

    for(int i=0; i<n; i++){
        cin>>weight[i]>>value[i];
    }

    IloEnv env;
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

        cplex.solve();
        
        auto status = cplex.getStatus();

        cout<<"Cplex Status: "<<status<<endl;
        cout<<"MIP gap: "<<cplex.getMIPRelativeGap()<<endl;
        
        cout<<"\nSelected items: "<<endl;
        cout << endl
         << left
         << setw(8)  << "ID"
         << setw(10) << "Weight"
         << setw(10) << "Value"
         << "\n";
        cout<<"--------------------------"<<endl;

        double totalWeight=0;

        for(int i=0; i<n; i++){
            if(cplex.getValue(x[i])>0.5){
                cout << left
                     << setw(8) << i
                     << setw(10) << weight[i]
                     << setw(10) << value[i]
                     << endl;                
                totalWeight+=weight[i];
            } 
        }
        cout<<"--------------------------"<<endl;
        cout << left
         << setw(8) << "TOTAL"
         << setw(10) << totalWeight
         << setw(10) << cplex.getObjValue()
         << "\n";
        
    }
    catch(IloException& e){
        cerr<< e <<endl;
    }

    env.end();
    return 0;

}

