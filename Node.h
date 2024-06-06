#pragma once
#include <vector>
using namespace std;

class Node
{
public:  
	Node(int v, int l, int h);
	Node(vector<int> arr);
	int val; // which bit is this looking at
	
	int lo; // index of lo
	int hi; // index of hi

	int ref = 0;

	/*int aux; // extra storage vvv
	bool aux_bool;
	bool lo_bool;*/

	bool operator<(const Node& rhs) const;
};

