#include "Node.h"


Node::Node(int v, int l, int h) {
	val = v;
	lo = l;
	hi = h;
}

Node::Node(vector<int> arr) {
	val = arr[0];
}

bool Node::operator<(const Node& o) const { 
	return val < o.val;
}

