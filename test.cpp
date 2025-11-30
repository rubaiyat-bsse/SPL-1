#include <ilcplex/ilocplex.h>
using namespace std;

int main() {
    IloEnv env;
    IloModel model(env);

    IloNumVar x(env, 0, IloInfinity, ILOFLOAT);
    IloNumVar y(env, 0, IloInfinity, ILOFLOAT);

    model.add(IloMaximize(env, x + y));
    model.add(x + 2*y <= 6);
    model.add(3*x + y <= 8);

    IloCplex cplex(model);

    if (!cplex.solve()) {
        cout << "Failed to optimize" << endl;
        return 1;
    }

    cout << "Optimal Solution:" << endl;
    cout << "x = " << cplex.getValue(x) << endl;
    cout << "y = " << cplex.getValue(y) << endl;

    env.end();
}
