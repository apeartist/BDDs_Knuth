#pragma once
#include <vector>
using namespace std;

class PNode
{
public:  
	//PNode(int v, int l, int h);
	PNode(vector<int> arr, int i);
	int val; // which bit is this looking at

	PNode* lo;
	PNode* hi;
	int index; // index in BDD
};

