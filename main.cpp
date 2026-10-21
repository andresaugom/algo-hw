#include <bits/stdc++.h>
#include <cstdint>

using namespace std;

/**
 * Reference: www.geeksforgeeks.org/dsa/ford-fulkerson-algorithm-for-maximum-flow-problem/
 */

bool bfs(vector<array<int64_t, 3>> &rGraph,
	int64_t V,
	int64_t src,
	int64_t sink,
	vector<int64_t> &parent) {

	vector<bool> vis(V + 1, false);
	queue<int64_t> q;

	q.push(src);
	vis[src] = true;
	parent[src] = -1;

	while (!q.empty()) {
		int64_t u = q.front();
		q.pop();

		for (int64_t i = 0; i < (int64_t)rGraph.size(); i++) {
			int64_t v = rGraph[i][1];

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

int64_t findMaxFlow(int64_t V, int64_t src, int64_t sink, vector<array<int64_t, 3>> &edges) {
	vector<array<int64_t, 3>> rGraph;

	for (auto &edge : edges) {
		int64_t u = edge[0];
		int64_t v = edge[1];
		int64_t cap = edge[2];

		rGraph.push_back({u, v, cap});
		rGraph.push_back({v, u, 0});
	}

	vector<int64_t> parent(V + 1);
	int64_t maxFlow = 0;

	while (bfs(rGraph, V, src, sink, parent)) {
		int64_t pathFlow = INT64_MAX;

		for (int64_t v = sink; v != src; v = rGraph[parent[v]][0]) {
			int64_t i = parent[v];
			pathFlow = min(pathFlow, rGraph[i][2]);
		}

		for (int64_t v = sink; v != src; v = rGraph[parent[v]][0]) {
			int64_t i = parent[v];
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

	int64_t V = 0, src = 0, sink = 0;
	vector<array<int64_t, 3>> edges;

	string line;
	while (getline(*in, line)) {
		istringstream ss(line);
		char type;
		ss >> type;

		if (type == 'p') {
			string problem;
			int64_t E;
			ss >> problem >> V >> E;
		} else if (type == 'a') {
			int64_t u, v, cap;
			ss >> u >> v >> cap;
			edges.push_back({u, v, cap});
		}
		else if (type == 'n') {
			int64_t id; char st; ss >> id >> st; 
			if (st == 's') src = id; else if (st == 't') sink = id;
		}
	}

	cout << findMaxFlow(V, src, sink, edges) << endl;
	return 0;
}
