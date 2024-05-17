#pragma once
#include <string>
#include "Node.h"

class BDD
{
public:
	BDD(vector<vector<int>> arr);
	BDD(std::string bitstring);
	vector<Node> nodes;
	int rootIndex;
	int vmax;

	//vector<int> solutions();
	void reduce();
	//BDD synthesis(uint8_t op);
	

	string toString();
};

