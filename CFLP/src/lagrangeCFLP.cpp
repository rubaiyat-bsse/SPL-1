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

double polyakStepSize(double UB, double LB, vector<double>&g){
    double norm=0;
    for(double v:g) norm+=v*v;
    if(norm==0) return 0;
    return (UB-LB)/norm;
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

    vector<double> lambda(m,0);

    double LB = DBL_MIN;
    double UB = DBL_MAX;

    vector<vector<double>> x(n, vector<double>(m,0));
    vector<int> y(m);

    for(int iter=0; iter<MAX_ITERATION; iter++){
        // facility subproblem

        for(int j=0; j<m; j++){
            if(facilities[j].f - lambda[j]*facilities[j].M < 0)
                y[j]=1;
            else 
                y[j]=0;
        }

        // assignment subproblem
        
        vector<vector<double>> x(n, vector<double>(m,0));

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

        // calculate Lagrangian value

        double L=0;

        for(int j=0; j<m; j++){
            L += (facilities[j].f - lambda[j]*facilities[j].M)*y[j];
        }

        for(int i=0; i<n; i++){
            for(int j=0; j<m; j++){
                L += (c[i][j] + lambda[j])*x[i][j];
            }
        }

        LB = max(LB, L);

        // simple repair heuristic
        vector<int> yRepair = y;
        vector<vector<double>> xRepair =x;

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

        // primal cost

        double primal=0;

        for(int j=0;j<m;j++)
            primal += facilities[j].f * yRepair[j];

        for(int i=0;i<n;i++)
            for(int j=0;j<m;j++)
                primal += c[i][j] * xRepair[i][j];

        UB=min(UB,primal);

        double gap = (UB-LB)/UB;

        if(gap < THRESHOLD) break;

        // subgradient

        vector<double> g(m);

        for(int j=0;j<m;j++){

            double flow=0;

            for(int i=0;i<n;i++)
                flow+=x[i][j];

            g[j] = flow - facilities[j].M*y[j];
        }

        // step size

        double step = polyakStepSize(UB,LB,g);

        //update lambda

        for(int j=0;j<m;j++)
            lambda[j] = max(0.0, lambda[j] + step*g[j]);

        cout<<"Lower Bound: "<<LB<<endl;
        cout<<"Upper Bound: "<<UB<<endl;

    }




}