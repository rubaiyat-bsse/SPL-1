
/*
    objective-> maximize sum(xi*vi)     | xi -> fraction of items 0<=xi<=1
    under the constrain sum(xi*wi)<=W       
*/


#include<bits/stdc++.h>
using namespace std;

typedef struct Item{
    double value;
    double weight;
};

void swap(Item *a, Item *b){
    Item temp= *a;
    *a=*b;
    *b=temp;
}

bool compareRatio(Item& a, Item& b){
    return (double)a.value*b.weight > (double) b.value*a.weight;
}

int doPartition(vector<Item>& items, int low, int high){
    Item pivot = items[low];
    int i=low, j=high;
    while(i<j){
        while(i<=high && compareRatio(items[i], pivot)) i++;
        while(j>=low && !compareRatio(items[j], pivot)) j--;

        if(i<j) swap(items[i], items[j]);
    }
    swap(items[low], items[j]);
    return j;
}

void quicksort(vector<Item>& items, int low, int high){
    if(low<high){
        int partitionIndx = doPartition(items, low, high);
        quicksort(items, low, partitionIndx-1);
        quicksort(items, partitionIndx+1, high);
    }
    
}

void sortByRatio(vector<Item>&items){
    if(!items.empty()){
        quicksort(items, 0, items.size()-1);
    }
}

double fractionalKnapsack(int W, vector<Item>&items){
    sortByRatio(items);

    int n = items.size();
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
