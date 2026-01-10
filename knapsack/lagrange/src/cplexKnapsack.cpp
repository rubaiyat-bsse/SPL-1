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

        cout<<"Optimal value: "<<fixed<<setprecision(0) <<cplex.getObjValue() <<endl;
        cout<<"Selected items: "<<endl;
        cout<<"ID\tWeight\tValue\n";

        double totalWeight=0;

        for(int i=0; i<n; i++){
            if(cplex.getValue(x[i])>0.5){
                cout<<i<<"\t"<<weight[i]<<"\t"<<value[i]<<"\n";
                totalWeight+=weight[i];
            } 
        }
        cout<<"Total weight: "<<totalWeight<<endl;
    }
    catch(IloException& e){
        cerr<< e <<endl;
    }

    env.end();
    return 0;

}

