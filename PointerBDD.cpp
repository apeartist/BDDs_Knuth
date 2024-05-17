#include "PointerBDD.h"
#include <cmath>

using namespace std;

PointerBDD::PointerBDD(vector<vector<int>> arr) {
	// val, lo_index, hi_index
	rootIndex = -1;
	vmax = 0;
	for (int i = 0; i < arr.size(); i++) {
		nodes.push_back(PNode(arr[i], i));
		vmax = max(vmax, arr[i][0]);
		if (arr[i][0] == 1) {
			rootIndex = i;
		}
	}
	for (int i = 0; i < arr.size(); i++) {
		nodes[i].lo = &nodes[arr[i][1]];
		nodes[i].hi = &nodes[arr[i][2]];

	}
}


vector<int> PointerBDD::solutions() {
	vector<int> out = vector<int>(nodes.size());
	out[0] = 0;
	out[1] = 1;
	for (int i = 2; i < nodes.size(); i++) {
		PNode lo = *nodes[i].lo; // asterisk before points to the value the pointer points to
		PNode hi = *nodes[i].hi;
		int powlo = lo.val - nodes[i].val-1;
		int powhi = hi.val - nodes[i].val-1;
		int shiftlo = 1 << powlo;
		int shifthi = 1 << powhi;
		out[i] = (out[(*nodes[i].lo).index] * shiftlo) + (out[(*nodes[i].hi).index] * shifthi);
	}
	return out;
}

/*
void PointerBDD::reduce() {
	// returns avail, index of the start of linkedlist. connected through hi, deleted nodes.
	vector<int> aux(nodes.size());
	vector<int> head = vector<int>(); // indeces
	head.resize(vmax + 1);
	int avail = -1; // "null" pointer for linked list of nodes to delete

	//R1
	// what does ROOT<=1 mean? is it V?
	aux[0] = -1;
	aux[1] = -1;
	aux[rootIndex] = -1;
	for (int i = 0; i < head.size(); i++) {
		head[i] = -1;
	}
	int s = rootIndex;
	int p;
	while (s != 0) {
		p = s;
		s = ~(aux[p]); // next node
		aux[p] = head[nodes[p].val];
		head[nodes[p].val] = ~p;
		if (aux[nodes[p].lo] >= 0) {
			aux[nodes[p].lo] = ~s;
			s = nodes[p].lo;
		}
		if (aux[nodes[p].hi] >= 0) {
			aux[nodes[p].hi] = ~s;
			s = nodes[p].hi;
		}
	}

	// R2
	int q;
	aux[0] = 0;
	aux[1] = 0;
	for (int v = vmax; v >= nodes[rootIndex].val; v--) {
		// R3
		p = ~head[v];
		s = 0;
		while (p != 0) {
			int pp = ~aux[p];

			// checks if either hi or lo is deleted. if they are, switches to its lo, which is an equivalent node
			q = nodes[p].hi;
			if (nodes[q].lo < 0) {
				nodes[p].hi = ~nodes[q].lo;
			}

			q = nodes[p].lo;
			if (nodes[q].lo < 0) {
				nodes[p].lo = ~nodes[q].lo;
				q = nodes[p].lo;
			}

			if (q == nodes[p].hi) { // if this node is redundant (hi and lo are the same)
				nodes[p].lo = ~q;
				nodes[p].hi = avail;
				aux[p] = 0;
				avail = p;
			}
			else if (aux[q] >= 0) {
				aux[p] = s;
				s = ~q;
				aux[q] = ~p;
			}
			else {
				aux[p] = aux[~aux[q]];
				aux[~aux[q]] = p;
			}
			p = pp;
		}

		//R4
		int r = ~s;
		s = 0;
		while (r >= 0) {
			q = ~aux[r];
			aux[r] = 0;
			if (s == 0) { s = q; }
			else { aux[p] = q; }
			p = q;
			while (aux[p] > 0) {
				p = aux[p];
			}
			r = ~aux[p];
		}

		//R5
		p = s;
		/*if (p == 0) {
			// goto R9
		}
		else {
			q = p;
		}

		while (p != 0) { // R9
			//R6
			s = nodes[p].lo;

			//R7
			// check if needs to be run once first (dowhile)
			while (q != 0 && nodes[q].lo == s) {
				r = nodes[q].hi;
				if (aux[r] >= 0) {
					aux[r] = ~q;
				}
				else {
					nodes[q].lo = aux[r];
					nodes[q].hi = avail;
					avail = q;
				}
				q = aux[q];
			}

			//R8
			while (p != q) {
				if (nodes[p].lo >= 0) {
					aux[nodes[p].hi] = 0;
				}
				p = aux[p];
			}
		}

	}

	if (nodes[rootIndex].lo < 0) {
		rootIndex = ~nodes[rootIndex].lo; // if top node is removed, change to next valid
	}

	// remove extra nodes, labeled with a negative lo
	int count = 0;
	vector<int> newIndex = vector<int>();
	for (int i = 0; i < nodes.size(); i++) {
		if (nodes[i].lo < 0) {
			newIndex.push_back(-1);
		}
		else {
			newIndex.push_back(count);
			count++;
		}
	}
	vector<PNode> newNodes = vector<PNode>();
	for (int i = 0; i < nodes.size(); i++) {
		if (nodes[i].lo >= 0) {
			nodes[i].lo = newIndex[nodes[i].lo];
			nodes[i].hi = newIndex[nodes[i].hi];
			newNodes.push_back(nodes[i]);
		}
	}
	nodes = newNodes;

	return;
}
*/

/*
BDD BDD::synthesis(uint8_t op) {
	vector<int> lstart = vector<int>();
	lstart.resize(vmax);
}
*/



string PointerBDD::toString() {
	string out = "";
	for (const PNode n : nodes) {
		out += "{" + to_string(n.val) + "," + to_string((*n.lo).index) + "," + to_string((*n.hi).index) + "},";
	}
	return out;
}