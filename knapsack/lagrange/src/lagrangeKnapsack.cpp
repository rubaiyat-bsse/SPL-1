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

static void logIteration(
    int iter,
    double lambda,
    double dual,
    double primal,
    double LB,
    double UB,
    double gap,
    double step,
    double subgrad
){
    static bool headerPrinted = false;

    ofstream logFile("/home/ratul/IIT/spl-1/CFLP/out/iterationLog_knapsack.txt", ios::app);
    if(!logFile) return;
    
    if(!headerPrinted){
        logFile << left
                << setw(6)  << "iter"
                << setw(15) << "lambda"
                << setw(15) << "dual"
                << setw(15) << "primal"
                << setw(15) << "LB"
                << setw(15) << "UB"
                << setw(15) << "gap"
                << setw(15) << "step"
                << setw(15) << "subgrad"
                << "\n";
        logFile << string(100, '-') << "\n";
        headerPrinted = true;
    }

    logFile << left
            << setw(6)  << iter
            << setw(15) << lambda
            << setw(15) << dual
            << setw(15) << primal
            << setw(15) << LB
            << setw(15) << UB
            << setw(15) << gap
            << setw(15) << step
            << setw(15) << subgrad
            << "\n";
    
    logFile.close(); 
}

std::pair<double, double> runKnapsackLagrange(const std::string& inputFile){
    ifstream ifs(inputFile);
    if (!ifs.is_open()){
        cerr << "Could not open " << inputFile << endl;
        return {-1.0, -1.0};
    }

    int n, C;
    if (!(ifs >> n >> C)) return {-1.0, -1.0};
    
    vector<Item> items(n);
    vector<Item> originalItems(n);
    for(int i=0; i<n; i++){
        int weight, value;
        ifs >> weight >> value;

        items[i].weight = weight;
        items[i].value = value;
        items[i].id = i;
        originalItems[i] = items[i];
    }
    ifs.close();

    // Reset log file
    ofstream("/home/ratul/IIT/spl-1/CFLP/out/iterationLog_knapsack.txt", ios::trunc).close();

    auto startTime = std::chrono::high_resolution_clock::now();

    double lambda =0.0;
    auto initialData = initialGreedyData(items, C);

    double bestLowerBound = initialData.first;
    double bestUpperBound = DBL_MAX;
    double stepSize = 0.0;

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
        if(primalConstraintValue<=C){
            if(primalValue>bestLowerBound){  
                bestLowerBound = primalValue;
                bestSolutionIndex = currentSolutionIndex;
            }
        }

        double dualityGap = bestUpperBound - bestLowerBound;
        double subgradient = C - primalConstraintValue;

        if(dualityGap<=THRESHOLD || abs(subgradient)==0){
            break;
        }

        stepSize = polyakStepSize(currentUpperBound, bestLowerBound, subgradient); 
        lambda = max(0.0, lambda-stepSize*subgradient);

        logIteration(I, lambda, currentUpperBound, primalValue, bestLowerBound, bestUpperBound, dualityGap, stepSize, subgradient);
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    double executionTime = std::chrono::duration<double>(endTime - startTime).count();

    sort(bestSolutionIndex.begin(), bestSolutionIndex.end());

    ofstream outLog("/home/ratul/IIT/spl-1/CFLP/out/lagrangeKnapsackOut.txt", ios::trunc);
    outLog << "Lagrange Knapsack Lower Bound (Best Feasible) = " << bestLowerBound << "\n";
    outLog << "\nSelected items:\nID      Weight    Value\n";
    outLog << "--------------------------\n";

    int totalWeight = 0;
    int totalValue = 0;
    for(int id : bestSolutionIndex){
        outLog << left << setw(8) << originalItems[id].id 
               << setw(10) << originalItems[id].weight 
               << setw(10) << originalItems[id].value << "\n";
        totalWeight += originalItems[id].weight;
        totalValue += originalItems[id].value;
    }
    outLog << "--------------------------\n";
    outLog << left << setw(8) << "TOTAL" << setw(10) << totalWeight << setw(10) << totalValue << "\n";
    outLog.close();

    return {bestLowerBound, executionTime};
}