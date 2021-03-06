#include <iostream>
#include <iosfwd>
#include <iomanip>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cassert>
#include <cctype>
#include <vector>
#include <bitset>
#include <set>
#include <queue>
#include <stack>
#include <map>
#include <deque>
#include <string>
#include <list>
#include <iterator>
#include <sstream>
#include <complex>
#include <fstream>
#include <functional>
#include <numeric>
#include <utility>
#include <algorithm>

using namespace std;

template <class T> inline int size(const T& c) {return (int) c.size();}
int rInt() {int nT = -1; scanf("%d", &nT); return nT;}

const int maxBufSize = (1 << 24);
const int maxStrSize = (1 << 20);

// Faster I/O
struct Input
{
	int bufSize, bufEnd, bufPos;
	char buffer[maxBufSize];
	void grabBuffer() {
		bufSize = (maxBufSize); bufPos = 0;
		bufEnd = fread(buffer, sizeof (char), bufSize, stdin);
		buffer[bufEnd] = '\0';
	}
	bool bufEof() {return bufPos == bufEnd;}
	int getChar() {return buffer[bufPos++];}
	void skipWS() {
		while ((buffer[bufPos] == '\n' ||
			buffer[bufPos] == ' ' || buffer[bufPos] == '\t')) 
			bufPos++;
	}
	int rInt() {
		int n = 0, x; skipWS();
		for (x = getChar(); x <= '9' && x >= '0'; x = getChar())
			n = (n << 3) + (n << 1) + (x - '0');
		return n;
	}
	inline bool isWhiteSpace(char x) {
		return x == ' ' || x == '\n' || x == '\t';
	}
	char rChar() {skipWS(); return getChar();}
};

struct Output
{
	int bufSize, bufEnd, bufPos;
	char buffer[maxBufSize];
	Output() {bufPos = bufEnd = 0; bufSize = maxBufSize;}
	void print(int x) {
		if (x < 0) {
			print('-'); print(-x); return;
		}
		char temp[1 << 4]; int idx = 0;
		do {
			temp[idx++] = '0' + (x % 10);
			x /= 10;
		} while (x != 0);
		temp[idx] = '\0';
		reverse(temp, temp + idx);
		print(temp);
	}
	inline void print(char ch) {buffer[bufEnd++] = ch;}
	inline void print(const char* s) {while (*s) buffer[bufEnd++] = *s++;}
	inline void print(const string& s) {print(s.c_str());}
	inline void flushBuffer() {buffer[bufEnd] = '\0'; printf("%s", buffer); bufEnd = 0;}
	template <class A, class B> 
	void print(const A& a, const B& b) {print(a); print(b);}
	template <class A, class B, class C> 
	void print(const A& a, const B& b, const C& c) {print(a); print(b); print(c);}
};

Input input; Output output;

const int INF  = (1 << 30);
const int base = 15;


struct Edge {
	int u, v;
	int weight;
	Edge(int u, int v, int weight) {
		this -> u = u, this -> v = v;
		this -> weight = weight;
	}
};

struct UnionFind
{
	vector <int> parent;
	vector <int> rank;
	UnionFind(int V) {
		parent.resize(V);
		rank.resize(V, 0);
	}
	void makeSet(int x) {
		parent[x] = x;
	}
	int find(int u) {
		return u == parent[u]? u : (parent[u] = find(parent[u]));
	}
	void merge(int u, int v) {
		if (rank[u] > rank[v])
			parent[v] = u;
		else if (rank[u] < rank[v])
			parent[u] = v;
		else {
			parent[v] = u;
			++rank[u];
		}
	}
};

struct Graph
{
	vector < vector <int> > adj;
	vector < vector <int> > cost;
	Graph(int V) {
		adj.resize(V);
		cost.resize(V, vector <int> (V, 0));
		for (int u = 0; u < V; ++u)
			cost[u][u] = INF;
	}
	void addEdge(int u, int v, int weight) {
		adj[u].push_back(v);
		adj[v].push_back(u);
		cost[u][v] = cost[v][u] = weight;
	}
	void solveBFS(vector <int>& dist, int source) {
		queue <int> q;
		dist[source] = (1 << 29);
		q.push(source);
		while (!q.empty()) {
			int u = q.front(); q.pop();
			for (int i = 0; i < size(adj[u]); ++i)
				if (dist[adj[u][i]] == INF) {
					dist[adj[u][i]] = min(dist[u], cost[u][adj[u][i]]);
					q.push(adj[u][i]);
				}
		}
	}
};

struct Solver
{
	int V, E;
	vector < vector <int> > graph;
	vector < vector <int> > score;
	inline int remainder(int x) {return (x & ((1 << base) - 1));}
	inline int quotient(int x) {return (x >> base);}
	void solveMST(vector <Edge*>& mst, vector <Edge*>& edges) {
		vector <Edge*> sortedEdges;
		vector < vector <Edge*> > firstPass(1 << base), secondPass(1 << base);
		for (int i = 0; i < size(edges); ++i)
			firstPass[remainder(edges[i] -> weight)].push_back(edges[i]);
		for (int i = size(firstPass) - 1; i >= 0; --i)
			for (int j = 0; j < size(firstPass[i]); ++j)
				secondPass[quotient(firstPass[i][j] -> weight)].push_back(firstPass[i][j]);
		for (int i = size(secondPass) - 1; i >= 0; --i) {
			cout << "i = " << i << ", size = " << secondPass[i].size() << ", Here2" << endl;
			for (int j = 0; j < size(secondPass[i]) - 1; ++j)
				sortedEdges.push_back(secondPass[i][j]);
		}
		UnionFind unionFind(V);
		for (int i = 0; i < V; ++i)
			unionFind.makeSet(i);
		for (int i = 0; i < size(sortedEdges); ++i) {
			int P = unionFind.find(sortedEdges[i] -> u);
			int Q = unionFind.find(sortedEdges[i] -> v);
			if (P != Q) {
				unionFind.merge(P, Q);
				mst.push_back(sortedEdges[i]);
			}
		}
	}
	Solver(int V, vector <Edge*>& edges) {
		this -> V = V;
		this -> E = size(edges);
		vector <Edge*> mst;
		graph.resize(V, vector <int> (V, INF));
		for (int i = 0; i < size(edges); ++i) {
			graph[edges[i] -> u][edges[i] -> v] = edges[i] -> weight;
			graph[edges[i] -> v][edges[i] -> u] = edges[i] -> weight;
		}
		solveMST(mst, edges);
		Graph G(V);
		for (int i = 0; i < size(mst); ++i)
			G.addEdge(mst[i] -> u, mst[i] -> v, mst[i] -> weight);
		score.resize(V, vector <int> (V, INF));
		for (int i = 0; i < size(score); ++i)
			G.solveBFS(score[i], i);
	}
	int query(int u, int v) {return score[u][v];}
};

int main()
{
	input.grabBuffer();
	int nTest = 1;
	for (int test = 1; test <= nTest; ++test) {
		int V = input.rInt(), E = input.rInt();
		vector <Edge*> edges;
		edges.reserve((V * (V - 1)) >> 1);
		for (int i = 0; i < E; ++i) {
			int u = input.rInt(), v = input.rInt(), weight = input.rInt();
			edges.push_back(new Edge(u, v, weight));
		}
		Solver solver(V, edges);
		for (int u = 0; u < V; ++u) {
			for (int v = 0; v < V; ++v)
				if (u == v) output.print('0', ' ');
				else output.print(solver.query(u, v), ' ');
			output.print('\n');
		}
	}
	output.flushBuffer();
	return 0;
}

// Powered by PhoenixAI
