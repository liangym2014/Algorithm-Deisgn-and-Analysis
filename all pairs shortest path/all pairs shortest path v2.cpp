// all-pairs shortest-path.cpp : Defines the entry point for the console application.
/*
In this assignment you will implement one or more algorithms for the all-pairs
shortest-path problem. Here are data files describing three graphs: graph #1;
graph #2; graph #3.
The first line indicates the number of vertices and edges, respectively. Each
subsequent line describes an edge (the first two numbers are its tail and head,
respectively) and its length (the third number). NOTE: some of the edge lengths
are negative. NOTE: These graphs may or may not have negative-cost cycles.

Your task is to compute the "shortest shortest path". Precisely, you must first
identify which, if any, of the three graphs have no negative cycles. For each such
graph, you should compute all-pairs shortest paths and remember the smallest one
(i.e., compute minu,v∈Vd(u,v), where d(u,v) denotes the shortest-path distance
from u to v).

If each of the three graphs has a negative-cost cycle, then enter "NULL" in the
box below. If exactly one graph has no negative-cost cycles, then enter the
length of its shortest shortest path in the box below. If two or more of the
graphs have no negative-cost cycles, then enter the smallest of the lengths
of their shortest shortest paths in the box below.

OPTIONAL: You can use whatever algorithm you like to solve this question. If you
have extra time, try comparing the performance of different all-pairs
shortest-path algorithms!

OPTIONAL: If you want a bigger data set to play with, try computing the shortest
shortest path for this graph.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <utility>
#include <algorithm>
using namespace std;


struct vertex {
  bool visited = false;
  long p = 0, distance = INT_MAX;
  vector<pair<int,int>> edges;
};


bool cmp(const pair<long, int>& l, const pair<long, int>& r) {
  return l.first > r.first;
}



class Graph {
private:
  vector<vertex> vertices;
  long shortest_shortest_path = INT_MAX;


public:
  Graph(string& filename) {
    ifstream file(filename);
    int num_vertices, num_edges, head, tail, edge;

    //read data from source file
    if (file.is_open()) {
      file >> num_vertices >> num_edges;
      vertices = vector<vertex>(num_vertices + 1);

      while (file >> tail >> head >> edge) {
        vertices[tail].edges.push_back({head,edge}); //{the other end, edge}
      }
      file.close();
    }

    /*
    for (int i = 1; i < vertices.size(); ++i) {
      cout << "vertex " << i << ": ";
      for (auto& edge : vertices[i].edges) {
        cout << "[" << edge.first << "," << edge.second << "],";
      }
      cout << endl;
    }
    */
  }


  bool Bellman_ford() {
    int num_iteration = vertices.size();
    bool change = false;

    for (int i = 0; i <= num_iteration; ++i) {
      for (int j = 0; j < vertices.size(); ++j) {
        for (auto& e : vertices[j].edges) {
          if (vertices[e.first].p > (e.second + vertices[j].p)) {
            change = true;
            vertices[e.first].p = e.second + vertices[j].p;
          }
        }
      }

      if (change)
        change = (i == num_iteration);
      else
        break;
    }

    return change; //true for negative cycle exists
  }

  long Dijkstra_Algorithm(int source) {
    //set a big initial value to all the distances
    for (auto& v: vertices) {
      v.distance = INT_MAX;
      v.visited = false;
    }

    //set source distance as 0
    vertices[source].distance = 0;
    vector<pair<long, int>> heap = { {0, source} }; //{distance, index}

    while (!heap.empty()) {
      pair<long, int> f = heap.front();
      pop_heap(heap.begin(), heap.end(), cmp);
      heap.pop_back();

      int index = f.second;
      if (vertices[index].visited)
        continue;

      vertices[index].visited = true;

      for (auto &edge: vertices[index].edges) {
        long d = f.first + edge.second;

        if (d < vertices[edge.first].distance) {
          vertices[edge.first].distance = d;
          heap.push_back({ d, edge.first });
          push_heap(heap.begin(), heap.end(), cmp);
        }
      }
    }

    //transform the distance in G' to the one in G. pick the minimum as the shortest distance
    long shortest_distance = INT_MAX;
    for (int i = 1; i < vertices.size(); i++) {
      if (i == source)
        continue;
      vertices[i].distance += vertices[i].p;
      shortest_distance = min(vertices[i].distance, shortest_distance);
    }

    return shortest_distance - vertices[source].p;
  }

  void Johnsons_Algorithm() {
    if (Bellman_ford()) {
      cout << "negative cycle\n";
      return;
    }

    cout << "\n-------shortest path:--------\n";
    //reweighting edges, make all edges >= 0
    for (auto& v : vertices) {
      for (auto& edge : v.edges) {
        edge.second += v.p - vertices[edge.first].p;
      }
    }

    //run Dijkstra's algorithm per vertex
    for (int source = 1; source < vertices.size(); source++) {
      long d = Dijkstra_Algorithm(source);
      //cout << source<<':'<<d << '\n';

      shortest_shortest_path = min(shortest_shortest_path, d);
    }
    cout << shortest_shortest_path;

  }

};

int main()
{
  vector<string> filename = { "g1.txt", "g2.txt", "g3.txt", "test4.txt","large.txt"};
  Graph t(filename[4]);

  t.Johnsons_Algorithm();

  //cin.get();
  return 0;
}

