#include<bits/stdc++.h>
using namespace std;

#define MaxIteration 100
#define Threshold 1.5

typedef struct Item{
    int weight, value;
};

//greedy lower bound for a good start
pair<double, vector<int>> getInitialSolution(vector<Item> items, int C){
    for()
}

vector<int> lagrangian(vector<Item> items, int C){

    double lambda =0.0;
    double bestLowerBound = 0;
    double bestUpperBound = DBL_MAX;
    double stepSize;
    vector<int> bestSolutionIndex;

    for(int I=0; I<MaxIteration; I++){

        vector<int> currentSolutionIndex;
        double changingTermDual=0;
        double primalConstraintValue=0;
        double primalValue=0;

        for(int i=0; i<items.size(); i++){
        //solving subproblems
            
            if((items[i].value-lambda*items[i].weight)>=0){
                currentSolutionIndex.push_back(i);
                
                changingTermDual+=items[i].value-lambda*items[i].weight;

                primalConstraintValue+=items[i].weight;
                primalValue+=items[i].value;
            }
        }

        //calculate dual value a.k.a upper bound
        double currentUpperBound = changingTermDual + lambda*C;
        bestUpperBound = min(bestUpperBound, currentUpperBound);

        //primal check a.k.a lower bound

        double currentLowerBound;

        if(primalConstraintValue<=C){
            if(primalValue>bestLowerBound){  //only update bounds and save solutions if it fits in the bag
                bestLowerBound = primalValue;
                bestSolutionIndex = currentSolutionIndex;
            }
        }

        double dualityGap = bestUpperBound - bestLowerBound;

        if(dualityGap<=Threshold){
            break;
        }

        //calculate subgradient
        double subgradient = C - primalConstraintValue;

        //update multiplier
        stepSize = 1/sqrt(I+1); //if i dont +1 then it's division by zero and trauma
        
        lambda = max(0.0, lambda-stepSize*subgradient);

    }

    return bestSolutionIndex;

}


int main(){
    // int N, C;
    // cin>>N>>C;   
    // vector<Item> items(N);
    // for(int i=0; i<N; i++){
    //     int value, weight;
    //     cin>>value>>weight;
    //     items[i].value = value;
    //     items[i].weight = weight;
    // }

    vector<Item> items = {
        {2, 10}, // Index 0
        {4, 18}, // Index 1
        {3, 12}, // Index 2
        {5, 15}, // Index 3
        {1, 2}   // Index 4
    };
    double C = 10;

    vector<int> solutionVector = lagrangian(items, C);

    for(int i=0; i<solutionVector.size(); i++){
        cout<< solutionVector[i] <<" ";
    }
    cout<<endl;
    return 0;

}