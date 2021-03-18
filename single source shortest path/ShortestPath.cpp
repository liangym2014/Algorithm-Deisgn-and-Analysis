// ShortestPath.cpp : Defines the entry point for the console application.
/*
The file contains an adjacency list representation of an undirected weighted 
graph with 200 vertices labeled 1 to 200. Each row consists of the node tuples
that are adjacent to that particular vertex along with the length of that edge.
For example, the 6th row has 6 as the first entry indicating that this row 
corresponds to the vertex labeled 6. The next entry of this row "141,8200" 
indicates that there is an edge between vertex 6 and vertex 141 that has length
8200. The rest of the pairs of this row indicate the other vertices adjacent to
vertex 6 and the lengths of the corresponding edges.

Your task is to run Dijkstra's shortest-path algorithm on this graph, using 1
(the first vertex) as the source vertex, and to compute the shortest-path 
distances between 1 and every other vertex of the graph. If there is no path
between a vertex v and vertex 1, we'll define the shortest-path distance 
between 1 and v to be 1000000. 

You should report the shortest-path distances to the following ten vertices, 
in order: 7,37,59,82,99,115,133,165,188,197. You should encode the distances 
as a comma-separated string of integers. So if you find that all ten of these
vertices except 115 are at distance 1000 away from vertex 1 and 115 is 2000 
distance away, then your answer should be 1000,1000,1000,1000,1000,2000,1000,
1000,1000,1000. Remember the order of reporting DOES MATTER, and the string 
should be in the same order in which the above ten vertices are given. Please
type your answer in the space provided.

IMPLEMENTATION NOTES: This graph is small enough that the straightforward O(mn)
time implementation of Dijkstra's algorithm should work fine. OPTIONAL: For 
those of you seeking an additional challenge, try implementing the heap-based
version. Note this requires a heap that supports deletions, and you'll probably
need to maintain some kind of mapping between vertices and their positions in 
the heap.
*/


#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

struct Node {
    vector<pair<int, int>> edges;
    bool determined = false;
    int distance = 1000000;
};

bool cmp(const pair<int, int>& l, const pair<int, int>& r) {
    return l.first > r.first;
}

class Graph {
private:
    vector<Node> nodes;

public:
    Graph(string filename, int num_nodes): nodes(num_nodes + 1) {
        ifstream file;
        file.open(filename);

        string line;
        int index = 0;
        char* c;
        pair<int, int> edge;
        while (getline(file, line)) {
            index = stoi(line);
            nodes[index].edges = vector<pair<int, int>>();
            size_t pos = line.find("\t");
            line = line.substr(pos + 1);
            c = &line[0];
            while (*c != '\0') {
                sscanf_s(c, "%d,%d\t", &edge.first, &edge.second);
                nodes[index].edges.push_back(edge);
                pos = line.find("\t");
                line = line.substr(pos + 1);
                c = &line[0];
            }
        }
    }

    void Dijkstra(int source) {
        nodes[source].distance = 0;
        vector<pair<int,int>> heap;  //{distance, node index}
        heap.push_back({ 0, source });

        while (!heap.empty()) {
            int d = heap.front().first, source = heap.front().second;
            pop_heap(heap.begin(), heap.end(), cmp);
            heap.pop_back();

            if (nodes[source].determined)
                continue;

            nodes[source].determined = true;

            for (auto &edge : nodes[source].edges) {
                int new_distance = nodes[source].distance + edge.second;

                if (nodes[edge.first].distance > new_distance) {
                    nodes[edge.first].distance = new_distance;
                    heap.push_back({ new_distance, edge.first });
                    push_heap(heap.begin(), heap.end(), cmp);
                }
            }

        }
    }

    void print_distance(vector<int> destinations) {
        for (auto &n: destinations) {
            cout << nodes[n].distance << ", ";
        }
        cout << endl;
    }

};

int main()
{
    string file[] = { "dijkstraData.txt" };
    //report the shortest paths between the start poit and each element in target array
    vector<vector<int>> destinations{{7,37,59,82,99,115,133,165,188,197}};
    int num_nodes[] = { 200 };


    int idx = 0;

    Graph graph(file[idx],num_nodes[idx]);

    int source = 1;
    graph.Dijkstra(source);
    graph.print_distance(destinations[idx]);

}