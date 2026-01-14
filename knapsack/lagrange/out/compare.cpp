#include <bits/stdc++.h>
using namespace std;

int main() {
    ifstream f1("output.txt"), f2("cplexOutput.txt");

    unordered_set<int> A, B;
    int x;

    while (f1 >> x) A.insert(x);
    while (f2 >> x) B.insert(x);

    cout << "In file1 but not file2:\n";
    for (int v : A)
        if (!B.count(v)) cout << v << " ";
    cout << "\n\nIn file2 but not file1:\n";
    for (int v : B)
        if (!A.count(v)) cout << v << " ";
}
