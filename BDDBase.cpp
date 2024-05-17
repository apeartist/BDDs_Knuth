#include "BDDBase.h"

BDDBase::BDDBase(int v) {
	// false, true
	nodes.push_back(Node(v + 1, 0, 0));
	nodes.push_back(Node(v + 1, 1, 1));

	uniqueTables.push_back(unordered_map<int64, int>());

	for (int i = 1; i <= v; i++) {
		nodes.push_back(Node(i, 0, 1));
		uniqueTables.push_back(unordered_map<int64, int>());
	}
}

int BDDBase::meldAnd(int f, int g) {
	if (f == 0 || g == 0) { return 0; }
	if (f == 1) { return g; }
	if (g == 1) { return f; }

	int64 key = hash(f, g);
	if (memo.find(key) != memo.end()) {
		return memo[key];
	}

	int v = min(nodes[f].val, nodes[g].val); // first val that they depend on
	int fl = nodes[f].val == v ? nodes[f].lo : f;
	int fh = nodes[f].val == v ? nodes[f].hi : f;
	int gl = nodes[g].val == v ? nodes[g].lo : g;
	int gh = nodes[g].val == v ? nodes[g].hi : g;

	int rl = meldAnd(fl, gl);
	int rh = meldAnd(fh, gh);

	int r = unique(v, rl, rh);
	memo[key] = r;
	return r;
}

int BDDBase::meldButNot(int f, int g) {
	if (f == 0 || g == 1) { return 0; }
	if (g == 0) { return f; }

	int64 key = hash(f, g);
	if (memo.find(key) != memo.end()) {
		return memo[key];
	}

	int v = min(nodes[f].val, nodes[g].val); // first val that they depend on
	int fl = nodes[f].val == v ? nodes[f].lo : f;
	int fh = nodes[f].val == v ? nodes[f].hi : f;
	int gl = nodes[g].val == v ? nodes[g].lo : g;
	int gh = nodes[g].val == v ? nodes[g].hi : g;

	int rl = meldButNot(fl, gl);
	int rh = meldButNot(fh, gh);

	int r = unique(v, rl, rh);
	memo[key] = r;
	return r;
}

int BDDBase::meldOr(int f, int g) {
	if (f == 1 || g == 1) { return 1; }
	if (f == 0) { return g; }
	if (g == 0) { return f; }

	int64 key = hash(f, g);
	if (memo.find(key) != memo.end()) {
		return memo[key];
	}

	int v = min(nodes[f].val, nodes[g].val); // first val that they depend on
	int fl = nodes[f].val == v ? nodes[f].lo : f;
	int fh = nodes[f].val == v ? nodes[f].hi : f;
	int gl = nodes[g].val == v ? nodes[g].lo : g;
	int gh = nodes[g].val == v ? nodes[g].hi : g;

	int rl = meldOr(fl, gl);
	int rh = meldOr(fh, gh);

	int r = unique(v, rl, rh);
	memo[key] = r;
	return r;
}

int BDDBase::basicFunction(int i) {
	return i + 1;
}

int64 BDDBase::countSolutions(int f) {
	// use findVisited to get hit nodes
	vector<int> hitNodes = findVisited(f);

	unordered_map<int, int> indexMap;
	for (int i = 0; i < hitNodes.size(); i++) { indexMap[hitNodes[i]] = i; }

	vector<int64> out = vector<int64>(hitNodes.size());
	out[0] = 0;
	out[1] = 1;
	for (int i = 2; i < hitNodes.size(); i++) {
		int lo = nodes[hitNodes[i]].lo; // asterisk before points to the value the pointer points to
		int hi = nodes[hitNodes[i]].hi;
		int powlo = nodes[lo].val - nodes[hitNodes[i]].val - 1;
		int powhi = nodes[hi].val - nodes[hitNodes[i]].val - 1;
		out[i] = (out[indexMap[lo]] << powlo) + (out[indexMap[hi]] << powhi); // check if this works
	}
	return out[out.size()-1];
}

vector<int> BDDBase::findVisited(int f) {
	vector<bool> visited = vector<bool>(nodes.size());
	// does this need to be improved
	for (int i = 0; i < nodes.size(); i++) {
		visited[i] = false;
	}
	queue<int> q = queue<int>();
	q.push(f);
	visited[f] = true;
	while (!q.empty()) {
		int cur = q.front();
		q.pop();

		if (!visited[nodes[cur].lo]) {
			q.push(nodes[cur].lo);
			visited[nodes[cur].lo] = true;
		}
		if (!visited[nodes[cur].hi]) {
			q.push(nodes[cur].hi);
			visited[nodes[cur].hi] = true;
		}
	}

	vector<int> out;
	for (int i = nodes.size()-1; i >= 0 ; i--)  {
		if (visited[i]) {
			out.push_back(i);
		}
	}

	sort(out.begin(), out.end()); // sort by value
	return out;
}

inline int64 BDDBase::hash(int a, int b) {
	return (((int64)b) << 32) | a;
}

inline int64 BDDBase::hash(const Node& n) {
	return (((int64)n.hi) << 32) | n.lo;
}

int BDDBase::unique(int v, int l, int h) {
	if (l == h) { return l; }
	int64 key = hash(l, h);
	if (uniqueTables[v].find(key) != uniqueTables[v].end()) {
		return uniqueTables[v][key];
	}
	else {
		nodes.push_back(Node(v, l, h));
		uniqueTables[v][key] = nodes.size() - 1;
		return nodes.size() - 1;
	}
}
