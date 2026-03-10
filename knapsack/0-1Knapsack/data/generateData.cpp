#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<string>

using namespace std;

#define WEIGHT_MIN 1      //min weight of each individual item
#define WEIGHT_MAX 100
#define VALUE_MIN 1
#define VALUE_MAX 100

int main(int argc, char *argv[]){
    if(argc!=3) return 1;

    int N = stoi(argv[1]);
    int W = stoi(argv[2]);

    char* outputFilename = "knapsack_input.txt"; 

    srand(time(0));

    FILE* fp = fopen(outputFilename, "w");

    if(fp==NULL){
        cout<<"error writing to file"<<endl;
        return 1;
    }

    fprintf(fp, "%d %d\n", N, W);

    for(int i=0; i<N; i++){
        int weight = rand()%(WEIGHT_MAX-WEIGHT_MIN+1)+WEIGHT_MIN;
        int value = rand()%(VALUE_MAX-VALUE_MIN+1)+VALUE_MIN;

        fprintf(fp, "%d %d\n", weight, value);
    }

    fclose(fp);

    cout<<"Successfully generated input file"<<endl;
    cout<<"Items(N): "<<N<<", Capacity(W): "<<W<<endl;

    return 0;
}