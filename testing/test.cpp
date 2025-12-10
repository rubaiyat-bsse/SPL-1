#include <ilcplex/ilocplex.h>
using namespace std;

int main() {
    IloEnv env;
    IloModel model(env);

    IloNumVar x(env, 0, IloInfinity, ILOFLOAT);
    IloNumVar y(env, 0, IloInfinity, ILOFLOAT);

    model.add(IloMaximize(env, 5*x+4*y));
    model.add(x + y<=5);
    model.add(10*x+6*y<=45);
    model.add(x>=4);
    model.add(y<=0);

    IloCplex cplex(model);

    // cplex.setOut(env.getNullStream());

    if (!cplex.solve()) {
        cout << "Failed to optimize" << endl;
        return 1;
    }

    cout << "Optimal Solution:" << endl;
    cout << "z = " << cplex.getObjValue()<< endl;
    cout << "x = " << cplex.getValue(x) << endl;
    cout << "y = " << cplex.getValue(y) << endl;
    

    env.end();
}
