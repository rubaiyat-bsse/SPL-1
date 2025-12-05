
/*
    objective-> maximize sum(xi*vi)     | xi -> fraction of items 0<=xi<=1
    under the constrain sum(xi*wi)<=W       
*/


#include<bits/stdc++.h>
using namespace std;

typedef struct Item{
    int value;
    int weight;
    float ratio;
};

void sortByRatio(vector<Item>&items){
    //implement any sorting algorithm based on the ratio
}

double fractionalKnapsack(int W, vector<Item>&items){
    sortByRatio(items);

    int n ; items.size();
    double totalValue = 0.0, totalWeight = 0.0;
    int i=0;
    

    while(i<n && totalWeight<=W){
        if(items[i].weight + totalWeight <= W){
            totalValue+=items[i].value;
            totalWeight+=items[i].weight;
        }
        else{
            int remaining = W - totalWeight;
            
            totalValue += items[i].value * ((double)remaining/items[i].weight);
            totalWeight+=remaining;
        }
        i++;
    }
    return totalValue;
}
