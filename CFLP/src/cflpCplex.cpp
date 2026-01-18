#include<ilcplex/ilocplex.h>
#include<bits/stdc++.h>

ILOSTLBEGIN

int main(){
    int J, I;
    cin>>J>>I;

    vector<double> d(I);
    vector<double> M(J);
    vector<double> f(J);
    vector<vector<double>> c(J, vector<double>(I));

    for(int i=0; i<I; i++) cin>>d[i];
    for(int i=0; i<J; i++) cin>>M[i];   
    for(int i=0; i<J; i++) cin>>f[i];

    for(int j=0; j<J; j++)
        for(int i=0; i<I; i++)
            cin>>c[j][i];

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
            obj+=f[j]*y[j];
            for(int i=0; i<I; i++){
                obj+=c[j][i]*x[j][i];
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
            model.add(expr==d[i]);
            expr.end();
        }

        for(int j=0; j<J; j++){
            IloExpr expr(env);
            for(int i=0; i<I; i++){
                expr+=x[j][i];
            }
            model.add(expr<=M[j]*y[j]);
            expr.end();
        }

        IloCplex cplex(model);
        cplex.setOut(env.getNullStream());

        if(!cplex.solve()){
            cout<<"No feasible solution"<<endl;
            return 0;
        }

        cout<< "Optimal Objective Value = "<<cplex.getObjValue() <<endl;

        cout<< "Opened facilities:"<<endl;
        for(int j=0; j<J; j++){
            if(cplex.getValue(y[j])>0.5)
                cout<<"Facility "<<j<<" opened\n";
        }

        cout<<"\nAssignments (x[j][i]): "<<endl;

        for(int j=0; j<J; j++){
            for(int i=0; i<I; i++){
                double val = cplex.getValue(x[j][i]);
                if(val> 1e-6){
                    cout<< "x["<<j<<"]["<<i<<"] = "<<val<<endl;
                }
            }
        }

    } catch(IloException &e){
        cerr << "CPLEX Error: "<<e<<endl;
    }
    env.end();
    return 0;
}