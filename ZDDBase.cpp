#include "ZDDBase.h"

ZDDBase::ZDDBase(int v) {
	// false, true
	nodes.push_back(Node(v + 1, 0, 0));
	nodes.push_back(Node(v + 1, 1, 1));

	uniqueTables.push_back(unordered_map<int64, int>());

	for (int i = 1; i <= v; i++) {
		nodes.push_back(Node(i, 0, 1));
		uniqueTables.push_back(unordered_map<int64, int>());
	}
}


int ZDDBase::meldAnd(int f, int g) {
	// i assume that f and g are reduced, so i don't check if the root node's val has a hi of false

	int fv = f == 0 || f == 1 ? INT_MAX : nodes[f].val; // first variable that f supports
	int gv = g == 0 || g == 1 ? INT_MAX : nodes[g].val; // first variable that g supports

	while (fv != gv) {
		if (f == 0 || g == 0) {return 0;}

		if (fv < gv) { f = nodes[f].lo; }
		else { g = nodes[g].lo; }

	} // when does while loop end

	if (f > g) {
		int temp = f;
		f = g;
		g = temp;
	}

	if (f == g || f == 0) { return f; }

	int64 key = hash(f, g, AND);
	if (memo.find(key) != memo.end()) {
		return memo[key];
	}

	int rl = meldAnd(nodes[f].lo, nodes[g].lo);
	int rh = meldAnd(nodes[f].hi, nodes[g].hi);
	int r = unique(max(nodes[f].val, nodes[g].val), rl, rh, AND);
	memo[key] = r;
	return r;
}

int ZDDBase::meldButNot(int f, int g) {
	int fv = f == 0 || f == 1 ? INT_MAX : nodes[f].val; // first variable that f supports
	int gv = g == 0 || g == 1 ? INT_MAX : nodes[g].val; // first variable that g supports

	while (gv < fv) {
		if (f == 0 || f == g) { return 0; }
		if (g == 0) { return f; }
		if (gv < fv) {
			g = nodes[g].lo;
			int gv = g == 0 || g == 1 ? INT_MAX : nodes[g].val;
		}
	}

	int64 key = hash(f, g, BUTNOT);
	if (memo.find(key) != memo.end()) {
		return memo[key];
	}

	int rl, rh;
	if (fv == gv) {
		rl = meldButNot(nodes[f].lo, nodes[g].lo);
		rh = meldButNot(nodes[f].hi, nodes[g].hi);
	}
	else { // gv>fv
		rl = meldButNot(nodes[f].lo, g);
		rh = nodes[f].hi;
		increaseRef(nodes[f].hi);
	}
	int r = unique(fv, rl, rh, BUTNOT);
	memo[key] = r;
	return r;
}

int ZDDBase::meldOr(int f, int g) {
	if (f == g) { return f; }

	if (f > g) {
		int temp = f;
		f = g;
		g = temp;
	}

	if (f == 0) { return g; }

	int64 key = hash(f, g, OR);
	if (memo.find(key) != memo.end()) {
		return memo[key];
	}

	int fv = f == 0 || f == 1 ? INT_MAX : nodes[f].val; // first variable that f supports
	int gv = g == 0 || g == 1 ? INT_MAX : nodes[g].val; // first variable that g supports

	int v, rl, rh;
	if (fv == gv) {
		v = fv;
		rl = meldOr(nodes[f].lo, nodes[g].lo);
		rh = meldOr(nodes[f].hi, nodes[g].hi);
	}
	else if (fv < gv) {
		v = fv;
		rl = meldOr(nodes[f].lo, g);
		rh = nodes[f].hi;
		increaseRef(nodes[g].hi);
	}
	else {
		v = gv;
		rl = meldOr(f, nodes[g].lo);
		rh = nodes[g].hi;
		increaseRef(nodes[g].hi);
	}

	int r = unique(v, rl, rh, OR);
	memo[key] = r;
	return r;
}

int ZDDBase::basicFunction(int i) {
	return i + 1;
}

// GARBAGE COLLECTION

void ZDDBase::increaseRef(int f) {
	nodes[f].ref++;
	if (nodes[f].ref == 1) {
		// just came alive
		increaseRef(nodes[f].lo);
		increaseRef(nodes[f].hi);
	}
}

void ZDDBase::decreaseRef(int f) {
	nodes[f].ref--;
	if (nodes[f].ref == 0) {
		// just died
		decreaseRef(nodes[f].lo);
		decreaseRef(nodes[f].hi);
	}
}

void ZDDBase::cleanupNodes() {
	// create list of the shuffled alive node locations
	vector<int> indeces = vector<int>();
	vector<Node> newNodes = vector<Node>();
	int count = 0;
	for (int i = 0; i < nodes.size();i++) {
		if (nodes[i].ref <= 0) {
			// delete this node
			indeces.push_back(-1);
		}
		else {
			indeces.push_back(count);
			count++;
			newNodes.push_back(nodes[i]);
		}
	}
	
	// fix node references
	for (int i = 0; i < newNodes.size(); i++) {
		newNodes[i].lo = indeces[newNodes[i].lo];
		newNodes[i].hi = indeces[newNodes[i].hi];
	}

	// cleanup memo (put afterwards)
	for (auto val : memo) {
		int64 key = val.first;
		int64 a = key >> 34;
		int64 func = (key && !(a << 34)) >> 30;
		int64 b = (key && !(a << 34) && !(func << 30));
		if (nodes[a].ref <= 0 || nodes[b].ref <= 0) {
			memo.erase(key);
		}
		else {
			int val = memo[key];
			memo.erase(key);
			memo[hash(indeces[a], indeces[b], (funcs)key)] = indeces[val];
		}
	}

	nodes = newNodes;
}

// SOLUTIONS

int64 ZDDBase::countSolutions(int f) {
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
	return out[out.size() - 1];
}

vector<int> ZDDBase::findVisited(int f) {
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
	for (int i = nodes.size() - 1; i >= 0; i--) {
		if (visited[i]) {
			out.push_back(i);
		}
	}

	sort(out.begin(), out.end()); // sort by value
	return out;
}

// MEMO UTILITY

inline int64 ZDDBase::hash(int a, int b, funcs function) {
	//return (((int64)b) << 32) | a;
	return (((int64)b) << 34) | ((int64)function << 30) | a;
}

inline int64 ZDDBase::hash(const Node& n, funcs function) {
	//return (((int64)n.hi) << 32) | n.lo;
	return (((int64)n.hi) << 34) | ((int64)function << 30) | n.lo;
}

int ZDDBase::unique(int v, int l, int h, funcs function) {
	if (l == h) { return l; }
	int64 key = hash(l, h, function);
	if (uniqueTables[v].find(key) != uniqueTables[v].end()) {
		return uniqueTables[v][key];
	}
	else {
		nodes.push_back(Node(v, l, h));
		uniqueTables[v][key] = nodes.size() - 1;
		return nodes.size() - 1;
	}
}
