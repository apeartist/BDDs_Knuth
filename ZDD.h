#pragma once
#include <string>
#include "Node.h"

// ZDD: For any connection between values x and y, all values from x+1, x+2... y-1 should be false.
// Effectively counts all valid sets, or solutions
class ZDD
{
public:
	ZDD(vector<vector<int>> arr);
	vector<Node> nodes;
	int rootIndex;
	int vmax;

	int solutions();
	void reduce();

	string toString();
};