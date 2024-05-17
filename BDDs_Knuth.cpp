// BDDs_Knuth.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "BDDBase.h"
//#include "BDD.h"
//#include "ZDD.h"
//#include "PointerBDD.h"

using namespace std;

int pos(int x, int y, int n) {
    return x + y * n;
}

int combine(int a, int b, int c, int n, int m) {
    int tc = max(a, max(b, c));
    int ta = min(a, min(b, c));
    int tb = a + b + c - ta - tc;
    return ta * (m * n * m * n) + tb * m * n + tc;
}


int squareCovered(const vector<int>& tris, BDDBase& base) {
    int root = 0;
    for (int i = 0; i < tris.size(); i++) {
        int r = base.basicFunction(tris[i] + 1);
        for (int j = 1; j < tris.size(); j++) {
            r = base.meldButNot(r, base.basicFunction((tris[(i + j) % tris.size()]) + 1));
        }
        root = base.meldOr(root, r);
    }
    return root;
}


int triominoes(int n, int m, BDDBase& base) {
    // n width, m height
    // position is x + y*n

    vector<int> out;

    // horizontal
    for (int x = 0; x < n - 2; x++) {
        for (int y = 0; y < m; y++) {
            out.push_back(combine(pos(x, y, n), pos(x + 1, y, n), pos(x + 2, y, n), n, m));
        }
    }

    for (int x = 0; x<n; x++) {
        for (int y = 0; y < m-2; y++) {
            out.push_back(combine(pos(x, y, n), pos(x, y + 1, n), pos(x, y + 2, n), n, m));
        }
    }

    for (int x = 0; x < n - 1; x++) {
        for (int y = 0; y < m-1; y++) {
            out.push_back(combine(pos(x, y, n), pos(x + 1, y, n), pos(x, y + 1, n), n, m));
            out.push_back(combine(pos(x, y, n), pos(x + 1, y, n), pos(x+1, y + 1, n), n, m));
            out.push_back(combine(pos(x + 1, y, n), pos(x, y + 1, n), pos(x + 1, y + 1, n), n, m));
            out.push_back(combine(pos(x, y, n), pos(x + 1, y + 1, n), pos(x, y + 1, n), n, m));
        }
    }

    sort(out.begin(), out.end());

    vector<vector<int>> triominoTable = vector<vector<int>>(m * n);

    for (int i = 0; i < out.size(); i++) {
        int t = out[i];
        int c = t % (m * n);
        t /= (m * n);
        int b = t % (m * n);
        int a = t / (m * n);
        triominoTable[a].push_back(i);
        triominoTable[b].push_back(i);
        triominoTable[c].push_back(i);
    }

    base = BDDBase(out.size());
    int root = 1;
    for (int i = 0; i < n * m; i++) {
        root = base.meldAnd(root,squareCovered(triominoTable[i], base));
    }
    
  
    return root;
 }

int main()
{
    vector<vector<int>> nodes = {{4,0,0},{4,1,1},{3,1,1},{3,0,1},{3,0,1},{3,0,0},{2,3,2},{2,5,4},{1,7,6}};
    BDDBase base = BDDBase(4);
    /*int root = base.meldButNot(base.basicFunction(1), base.basicFunction(2));
    root = base.meldButNot(root, base.basicFunction(3));
    root = base.meldButNot(root, base.basicFunction(4));
    */

    /*
    // only one true out of four
    int root = 0;
    for (int i = 1; i <= 4; i++) {
        int r = i+1;
        for (int j = 1; j < 4; j++) {
            r = base.meldButNot(r, base.basicFunction((i + j - 1) % 4 + 1));
        }
        root = base.meldOr(root, r);
    }
    */

    /*
    int root = 1;
    for (int i = 1; i < 4; i++) {
        int r = base.meldAnd(base.basicFunction(i), base.basicFunction(i + 1));
        root = base.meldButNot(root, r);
    }

    cout << root << endl;
    cout << base.countSolutions(root);
    */

    int coverings = triominoes(2, 3,base);
    cout << base.countSolutions(coverings) << endl;

    

    /*vector<int> sol = tree.solutions();
    for (int i = 0; i < sol.size(); i++) {
        cout << sol[i] << endl;
    }*/
    //tree.reduce();

    /*
    ZDD tree = ZDD(nodes);
    cout << tree.solutions() << endl;
    tree.reduce();
    */

    //cout << tree.toString() << endl;

}