#include<bits/stdc++.h>
using namespace std;

#define MAX_ITERATION 200
#define THRESHOLD 1.5

typedef struct Item{
    int weight;
    int value;
    int id; //had to use id because sort() messes up the order
};

bool compareRatio(const Item& a, const Item& b){
    return (double)a.value*b.weight > (double) b.value*a.weight;
}

void sortByRatio(vector<Item>&items){
    sort(items.begin(), items.end(), compareRatio);
}

pair<double, vector<int>> initialGreedyData( vector<Item>&items, int W){
    sortByRatio(items);

    int n = items.size();
    double totalValue = 0.0, totalWeight = 0.0;
    int i=0;
    
    vector<int> solution;

    while(i<n && totalWeight<=W){
        if(items[i].weight + totalWeight <= W){
            totalValue+=items[i].value;
            totalWeight+=items[i].weight;
            solution.push_back(items[i].id);
        }
        i++;
    }
    return {totalValue, solution};
}

double polyakStepSize(double currentDualValue, double lowerBound, double subgradient){
    if(abs(subgradient)<1e-9) return 0.0;
    return (currentDualValue-lowerBound)/(subgradient*subgradient);
    
}

vector<int> lagrangian(vector<Item>items, int C){

    double lambda =0.0;

    //initalize best lower bound with greedy
    auto initialData = initialGreedyData(items, C);

    double bestLowerBound = initialData.first;
    double bestUpperBound = DBL_MAX;
    double stepSize;
    vector<int> bestSolutionIndex=initialData.second;

    for(int I=0; I<MAX_ITERATION; I++){

        vector<int> currentSolutionIndex;
        double changingTermDual=0;
        double primalConstraintValue=0;
        double primalValue=0;

        for(int i=0; i<items.size(); i++){
        //solving subproblems
            
            if((items[i].value-lambda*items[i].weight)>=0){
                currentSolutionIndex.push_back(items[i].id);
                
                changingTermDual+=items[i].value-lambda*items[i].weight;

                primalConstraintValue+=items[i].weight;
                primalValue+=items[i].value;
            }
        }

        //calculate dual value a.k.a upper bound
        double currentUpperBound = changingTermDual + lambda*C;

        if(currentUpperBound < bestUpperBound){
            bestUpperBound = currentUpperBound;
        }

        //primal check a.k.a lower bound

        double currentLowerBound;

        if(primalConstraintValue<=C){
            if(primalValue>bestLowerBound){  //only update bounds and save solutions if it fits in the bag
                bestLowerBound = primalValue;
                bestSolutionIndex = currentSolutionIndex;
            }
        }

        double dualityGap = bestUpperBound - bestLowerBound;
        double subgradient = C - primalConstraintValue;

        //STOPPING CONDITION
        if(dualityGap<=THRESHOLD || abs(subgradient)==0){
            break;
        }

        //update multiplier
        stepSize = polyakStepSize(currentUpperBound, bestLowerBound, subgradient); 
        
        lambda = max(0.0, lambda-stepSize*subgradient);

    }

    return bestSolutionIndex;

}

void printSolution(const vector<int>& solution, const vector<Item>& items){
    
    cout << endl
         << left
         << setw(8)  << "ID"
         << setw(10) << "Weight"
         << setw(10) << "Value"
         << "\n";
    cout<<"--------------------------"<<endl;

    int totalWeight = 0;
    int totalValue = 0;

    for(int id:solution){
        cout << left
             << setw(8) << items[id].id
             << setw(10) << items[id].weight
             << setw(10) << items[id].value
             << "\n";

        totalWeight += items[id].weight;
        totalValue += items[id].value;
    }
    cout<<"--------------------------"<<endl;
    cout << left
         << setw(8) << "TOTAL"
         << setw(10) << totalWeight
         << setw(10) << totalValue
         << "\n";
}

int main(){
    int n,C;
    cin>>n>>C;
    vector<Item> items(n);
    
    for(int i=0; i<n; i++){
        int weight, value;
        cin>>weight>>value;

        items[i].weight = weight;
        items[i].value = value;
        items[i].id = i;
    }

    vector<int> solutionVector = lagrangian(items, C);
    sort(solutionVector.begin(), solutionVector.end()); //for cleaner output

    printSolution(solutionVector, items);
    cout<<endl;
    return 0;

}