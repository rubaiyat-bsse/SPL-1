#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<string>

using namespace std;

int main(int argc, char *argv[]){
    if(argc!=3) return 1;

    int N = stoi(argv[1]);
    int W = stoi(argv[2]);

    //max & min weight of all the individual items
    int WEIGHT_MIN = 1;
    int WEIGHT_MAX = 1000; 

    //max & min value of all the individual items 
    int VALUE_MIN = 1;
    int VALUE_MAX = 1000;

    char* outputFilename = "knapsack_input.txt";

    srand(time(0));

    FILE* fp = fopen(outputFilename, "w");

    if(fp==NULL){
        cout<<"error writing to file"<<endl;
        return 1;
    }

    fprintf(fp, "%d %d\n", N, W);

    for(int i=0; i<N; i++){
        int weight = rand() % (WEIGHT_MAX-WEIGHT_MIN+1) + WEIGHT_MIN;
        int value = rand() % (VALUE_MAX-VALUE_MIN+1) + VALUE_MIN;

        fprintf(fp, "%d %d\n", weight, value);
    }

    fclose(fp);

    cout<<"Successfully generated input file"<<endl;
    cout<<"Items(N): "<<N<<", Capacity(W): "<<W<<endl;

    return 0;
}