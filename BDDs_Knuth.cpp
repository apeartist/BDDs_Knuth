// BDDs_Knuth.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "BDD.h"
#include "ZDD.h"

using namespace std;

int main()
{
    vector<vector<int>> nodes = {{4,0,0},{4,1,1},{3,1,1},{3,0,1},{3,0,1},{3,0,0},{2,3,2},{2,5,4},{1,7,6}};
    BDD tree = BDD(nodes);
    /*vector<int> sol = tree.solutions();
    for (int i = 0; i < sol.size(); i++) {
        cout << sol[i] << endl;
    }*/
    tree.reduce();

    /*
    vector<vector<int>> nodes = { {4,0,0},{4,1,1},{3,1,1},{3,0,1},{3,0,1},{3,0,0},{2,3,2},{2,5,4},{1,7,6} };
    ZDD tree = ZDD(nodes);
    cout << tree.solutions() << endl;
    tree.reduce();
    */

    cout << tree.toString() << endl;

}