#include<bits/stdc++.h>
using namespace std;

typedef struct Item{
    int weight, value;
};

vector<vector<int>> zeroOneKnapsack(vector<Item> &items, int W){

    int n = items.size();
    vector<vector<int>> dp(n+1, vector(W+1, 0));
    for(int i=1; i<=n; i++){
        for(int w=1; w<=W; w++){
            if(items[i].weight>w) 
                dp[i][w]=dp[i-1][w];
            else{
                dp[i][w]=max(dp[i-1][w], 
                            items[i].value+dp[i-1][w-items[i].weight]);
            }
        }
    }
    return dp;
}

vector<int> backtrack(vector<vector<int>>&dp, vector<Item>&items){
    vector<int> chosen;

    int n = dp.size()-1;
    int w = dp[0].size()-1;


    for(int i=n; i>0; i--){
        if(dp[i][w]!= dp[i-1][w]){
            chosen.push_back(1);
            w-=items[i].weight;
        }
        else chosen.push_back(0);
    }

    reverse(chosen.begin(), chosen.end());
    return chosen;
}

int main(){
    int n,W;
    cin>>n>>W;
    vector<Item> items(n+1);

    items[0]={0,0};

    for(int i=1; i<=n; i++){
        int weight, value;
        cin>>weight>>value;

        items[i].weight = weight;
        items[i].value = value;
    }

    vector<vector<int>> dp = zeroOneKnapsack(items, W);

    // for(int i=0; i<=n; i++){
    //     for(int w=0; w<=W; w++){
    //         cout<<dp[i][w]<<" ";
    //     }
    //     cout<<endl;
    // }

    // cout<<endl<<endl;

    vector<int> backTrack= backtrack(dp, items);

    for(int i=0; i<n; i++) cout<<backTrack[i]<<" ";
    return 0;
}