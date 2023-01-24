// Use Lengauer-Tarjan Algorithm to solve the dominator tree.

#include <stdio.h>
#include <string.h>

#include <algorithm>
#include <functional>
#include <iostream>
#include <set>
#include <vector>

const int kNumNodes = 200003;
const int kNumEdges = 600003;

typedef struct Edge {
  int node;
  int next;
} Edge;

struct Graph {
  Graph() {
    count = 0;

    bzero(predecessors, sizeof(predecessors));
    bzero(successors, sizeof(successors));
    bzero(edge, sizeof(edge));

    bzero(dfn, sizeof(dfn));

    bzero(ancestor, sizeof(ancestor));
    bzero(best, sizeof(best));

    bzero(rdom, sizeof(rdom));
    bzero(idom, sizeof(idom));
  }

  void addEdge(int from, int to) {
    // add directed edge from -> to.
    count++;
    edge[count].node = to;
    edge[count].next = successors[from];
    successors[from] = count;

    // add directed edge to -> from.
    count++;
    edge[count].node = from;
    edge[count].next = predecessors[to];
    predecessors[to] = count;
  }

  void solveDFN(int root) {
    int curDFN = 1;

    std::function<void(int)> dfnSolver = [&curDFN, &dfnSolver, this](int v) {
      dfn[v] = curDFN++;
      dfnToNode[dfn[v]] = v;

      for (int e = successors[v]; e; e = edge[e].next) {
        int child = edge[e].node;
        if (!dfn[child]) {
          dfsTreeParent[child] = v;
          dfnSolver(child);
        }
      }
    };

    dfnSolver(root);
  }

  void solveSemiDominator(int root) {
    std::function<int(int)> ancestorWithLowestSemi = [&ancestorWithLowestSemi,
                                                      this](int v) {
      int a = ancestor[v];
      if (ancestor[a]) {
        int parentbest = ancestorWithLowestSemi(a);
        if (dfn[semi[parentbest]] < dfn[semi[best[v]]]) {
          best[v] = parentbest;
        }
        ancestor[v] = ancestor[a];
      }
      return best[v];
    };

    for (int i = numNode; i >= 1; i--) {
      int vertex = dfnToNode[i];
      int p = dfsTreeParent[vertex];
      int s = p;

      for (int e = predecessors[vertex]; e; e = edge[e].next) {
        int sPrime;
        int node = edge[e].node;

        if (dfn[node] <= dfn[vertex]) {
          sPrime = node;
        } else {
          sPrime = semi[ancestorWithLowestSemi(node)];
        }

        if (dfn[s] > dfn[sPrime]) {
          s = sPrime;
        }
      }

      semi[vertex] = s;
      bucket[s].push_back(vertex);

      ancestor[vertex] = p;
      best[vertex] = vertex;

      for (auto v : bucket[p]) {
        int bv = ancestorWithLowestSemi(v);
        if (semi[bv] == semi[v]) {
          idom[v] = p;
        } else {
          rdom[v] = bv;
        }
      }

      bucket[p].clear();
    }
  }

  void solveIDominator() {
    for (int i = 1; i <= numNode; i++) {
      int v = dfnToNode[i];
      if (rdom[v]) {
        idom[v] = idom[rdom[v]];
      }
    }
  }

  int numNode;
  int numEdge;
  int count;

  Edge edge[kNumEdges << 1];
  int predecessors[kNumNodes];
  int successors[kNumNodes];

  // Depth-First Search Spanning Tree.
  int dfn[kNumNodes];
  int dfsTreeParent[kNumNodes];
  int dfnToNode[kNumNodes];

  // Weighted union set to solve the semi-dominator of the graph.
  int ancestor[kNumNodes];
  int best[kNumNodes];
  int semi[kNumNodes];

  // Immetiate dominators.
  std::vector<int> bucket[kNumNodes];
  int rdom[kNumNodes];
  int idom[kNumNodes];
};

Graph graph;

int main() {
  int n, m;
  int from, to;

  scanf("%d %d", &n, &m);
  graph.numNode = n;
  graph.numEdge = m;

  for (int i = 0; i < m; i++) {
    scanf("%d %d", &from, &to);
    graph.addEdge(from, to);
  }

  graph.solveDFN(1);
  graph.solveSemiDominator(1);
  graph.solveIDominator();

  for (int i = 1; i <= n; i++) {
    std::cout << "Node " << i << " Semi-Dominator: " << graph.semi[i] << ", "
              << "Immediate-Dominator: " << graph.idom[i] << std::endl;
  }

  return 0;
}