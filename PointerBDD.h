#pragma once
#include <string>
#include "PNode.h"

class PointerBDD
{
public:
	PointerBDD(vector<vector<int>> arr);
	//PointerBDD(std::string bitstring);
	vector<PNode> nodes;
	int rootIndex;
	int vmax;

	vector<int> solutions();
	//void reduce();
	//BDD synthesis(uint8_t op);


	string toString();
};

