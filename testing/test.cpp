#include <ilcplex/ilocplex.h>
using namespace std;

int main() {
    IloEnv env;
    IloModel model(env);

    IloNumVar y1(env, 0, IloInfinity, ILOFLOAT);
    IloNumVar y2(env, 0, IloInfinity, ILOFLOAT);
    IloNumVar y3(env, 0, IloInfinity, ILOFLOAT);

    model.add(IloMinimize(env, 3000*y1+4000*y2+5000*y3));
    model.add(y1+y3>=1.2);
    model.add(y2+y3>=1.7);

    IloCplex cplex(model);

    // cplex.setOut(env.getNullStream());

    if (!cplex.solve()) {
        cout << "Failed to optimize" << endl;
        return 1;
    }

    cout << "Dual Optimal Solution:" << endl;
    cout << "W = " << cplex.getObjValue()<< endl;
    cout << "y1 = " << cplex.getValue(y1) << endl;
    cout << "y2 = " << cplex.getValue(y2) << endl;
    cout << "y3 = " << cplex.getValue(y3) << endl;
    

    env.end();
}
