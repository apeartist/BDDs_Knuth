#pragma once
#include <vector>
#include "Node.h"
#include <unordered_map>
#include <queue>
#include <iostream>

using namespace std;
typedef long long int64;

class ZDDBase
{
public:
	ZDDBase(int v);

	vector<Node> nodes;
	int maxval;

	enum funcs {
		AND = 1, BUTNOT = 2, OR = 7
	};

	vector<unordered_map<int64, int>> uniqueTables;
	unordered_map<int64, int> memo; // automatic chaining

	int meldAnd(int f, int g);
	int meldButNot(int f, int g);
	int meldOr(int f, int g);

	int basicFunction(int i);

	void increaseRef(int f);
	void decreaseRef(int f);
	void cleanupNodes();

	int64 countSolutions(int f);
	vector<int> findVisited(int f); // util function to get a vector of visited nodes from root

	int64 hash(int a, int b, funcs function);
	int64 hash(const Node& n, funcs function);
	int unique(int v, int l, int h, funcs function);
};

