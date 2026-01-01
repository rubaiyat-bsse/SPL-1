#include<bits/stdc++.h>
#include<ilcplex/ilocplex.h>

ILOSTLBEGIN

double z_best = -IloInfinity;
double x_best = 0;
double y_best = 0;

bool isInteger(double x){
    return fabs(x-round(x))<1e-6;
} 

void branchAndBound(IloModel &model, IloEnv &env, IloNumVar &x, IloNumVar &y){
    IloCplex cplex(model);
    cplex.setOut(env.getNullStream());
    cplex.setWarning(env.getNullStream());

    if(!cplex.solve()) return; //infeasible

    double xVal = cplex.getValue(x);
    double yVal = cplex.getValue(y);
    double zVal = cplex.getObjValue();

    if(zVal<=z_best) return;

    if(isInteger(xVal) && isInteger(yVal)){
        z_best = zVal;
        x_best = xVal;
        y_best = yVal;

        return;
    }

    double fracX = fabs(xVal - floor(xVal));
    double fracY = fabs(yVal - floor(yVal));
    
    if(!isInteger(xVal) && !isInteger(yVal)){
        if(fracX>=fracY){
            //branch on x
            IloModel left(env); 
            left.add(model);
            left.add(x<=floor(xVal));
            branchAndBound(left, env, x, y);

            IloModel right(env);  
            right.add(model);
            right.add(x>=ceil(xVal));
            branchAndBound(right, env, x, y);
        }
        else{
            //branch on y
            IloModel left(env); 
            left.add(model);
            left.add(y<=floor(yVal));
            branchAndBound(left, env, x, y);

            IloModel right(env);  
            right.add(model);
            right.add(y>=ceil(yVal));
            branchAndBound(right, env, x, y);
        }
    }

    else if(!isInteger(xVal) && isInteger(yVal)){
            IloModel left(env); 
            left.add(model);
            left.add(x<=floor(xVal));
            branchAndBound(left, env, x, y);

            IloModel right(env);  
            right.add(model);
            right.add(x>=ceil(xVal));
            branchAndBound(right, env, x, y);
    }

    else if(isInteger(xVal) && !isInteger(yVal)){
            IloModel left(env); 
            left.add(model);
            left.add(y<=floor(yVal));
            branchAndBound(left, env, x, y);

            IloModel right(env);  
            right.add(model);
            right.add(y>=ceil(yVal));
            branchAndBound(right, env, x, y);
    }
    else return;

}  

int main(){
     IloEnv env;
    try {
        IloModel model(env);
        IloNumVar x(env, 0, IloInfinity, ILOFLOAT);
        IloNumVar y(env, 0, IloInfinity, ILOFLOAT);

        model.add(IloMaximize(env, 5*x + 4*y));

        model.add(x + y <= 5);
        model.add(10*x + 6*y <= 45);

        branchAndBound(model, env, x, y);

        cout << "Optimal integer solution:" << endl;
        cout << "z = " << z_best << endl;
        cout << "x = " << x_best << endl;
        cout << "y = " << y_best << endl;
    }
    catch (...) {
        cerr << "Exception occurred" << endl;
    }

    env.end();
    return 0;
}

