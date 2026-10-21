#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;

/**
 * Reference: www.geeksforgeeks.org/dsa/ford-fulkerson-algorithm-for-maximum-flow-problem/
 */

bool bfs(vector<array<int, 3>> &rGraph,
	int V,
	int src,
	int sink,
	vector<int> &parent) {

	vector<bool> vis(V + 1, false);
	queue<int> q;

	q.push(src);
	vis[src] = true;
	parent[src] = -1;

	while (!q.empty()) {
		int u = q.front();
		q.pop();

		for (int i = 0; i < (int)rGraph.size(); i++) {
			int v = rGraph[i][1];

			if (rGraph[i][0] == u && !vis[v] && rGraph[i][2] > 0) {
				vis[v] = true;
				parent[v] = i;

				if (v == sink) {
					return true;
				}

				q.push(v);
			}
		}
	}

	return false;
}

int findMaxFlow(int V, vector<array<int, 3>> &edges) {
	vector<array<int, 3>> rGraph;

	for (auto &edge : edges) {
		int u = edge[0];
		int v = edge[1];
		int cap = edge[2];

		rGraph.push_back({u, v, cap});
		rGraph.push_back({v, u, 0});
	}

	int src = 1;
	int sink = V;

	vector<int> parent(V + 1);
	int maxFlow = 0;

	while (bfs(rGraph, V, src, sink, parent)) {
		int pathFlow = INT_MAX;

		for (int v = sink; v != src; v = rGraph[parent[v]][0]) {
			int i = parent[v];
			pathFlow = min(pathFlow, rGraph[i][2]);
		}

		for (int v = sink; v != src; v = rGraph[parent[v]][0]) {
			int i = parent[v];
			rGraph[i][2] -= pathFlow;
			rGraph[i ^ 1][2] += pathFlow;
		}

		maxFlow += pathFlow;
	}

	return maxFlow;
}

int main() {
	ifstream file;
	istream *in = &cin;

	if (isatty(STDIN_FILENO)) {
		file.open("maxflow.dimacs");
		in = &file;
	}

	int V = 0;
	vector<array<int, 3>> edges;

	string line;
	while (getline(*in, line)) {
		istringstream ss(line);
		char type;
		ss >> type;

		if (type == 'p') {
			string problem;
			int E;
			ss >> problem >> V >> E;
		} else if (type == 'a') {
			int u, v, cap;
			ss >> u >> v >> cap;
			edges.push_back({u, v, cap});
		}
		else if (type == 'n') {
			int id, char st; ss >> id >> st; 
			if (st == 's') src = id; else if (st == 't') sink = id;
		}
	}

	cout << findMaxFlow(V, edges) << endl;
	return 0;
}
