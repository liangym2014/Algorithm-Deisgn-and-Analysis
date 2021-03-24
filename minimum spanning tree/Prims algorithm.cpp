// Prims algorithm.cpp : Defines the entry point for the console application.
//
/*
In this programming problem you'll code up Prim's minimum spanning tree
algorithm. Download the text file here. This file describes an undirected graph
with integer edge costs. It has the format
[number_of_nodes] [number_of_edges]
[one_node_of_edge_1] [other_node_of_edge_1] [edge_1_cost]
[one_node_of_edge_2] [other_node_of_edge_2] [edge_2_cost]
...
For example, the third line of the file is "2 3 -8874", indicating that there
is an edge connecting vertex #2 and vertex #3 that has cost -8874. You should
NOT assume that edge costs are positive, nor should you assume that they are 
distinct.

Your task is to run Prim's minimum spanning tree algorithm on this graph. You
should report the overall cost of a minimum spanning tree --- an integer, which
may or may not be negative --- in the box below.

IMPLEMENTATION NOTES: This graph is small enough that the straightforward O(mn)
time implementation of Prim's algorithm should work fine. OPTIONAL: For those of
you seeking an additional challenge, try implementing a heap-based version. The
simpler approach, which should already give you a healthy speed-up, is to maintain
relevant edges in a heap (with keys = edge costs). The superior approach stores
the unprocessed vertices in the heap, as described in lecture. Note this requires
a heap that supports deletions, and you'll probably need to maintain some kind of
mapping between vertices and their positions in the heap.
*/
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
using namespace std;

struct vertex {
    bool visited = false;
    vector<pair<int, int>> edges;  //{edge, the other end point}
};

struct cmp {
    bool operator()(const pair<int, int> &p1, const pair<int, int> &p2) const {
        return p1.first > p2.first;
    }
};

class SpanTree {
private:
    vector<vertex> vertices;
    int n; //number of nodes
    priority_queue<pair<int, int>, vector<pair<int, int>>, cmp> pqueue;

public:
    SpanTree(string &filename) {
        ifstream file(filename);

        if (file.is_open()) {
            int head, tail, edgecost;
            file >> n >> edgecost;

            vertices = vector<vertex>(n + 1);
            while (file >> head >> tail >> edgecost) {
                vertices[head].edges.push_back({edgecost, tail});
                vertices[tail].edges.push_back({edgecost, head});
            }

            file.close();
        }
    }

    int Prims() {
        int edgesum = 0;
        vertices[1].visited = true; //start from vertex 1

        for(auto &edge: vertices[1].edges)
            pqueue.push(edge);

        int count = 1;//the number of vertices visited

        while (count != n) {
            pair<int, int> edge = pqueue.top();
            pqueue.pop();
            int node = edge.second;

            if (vertices[node].visited)
                continue;

            vertices[node].visited = true;
            count ++;

            edgesum += edge.first;

            for (auto &e: vertices[node].edges) {
                if(!vertices[e.second].visited)
                    pqueue.push(e);
            }
        }
        return edgesum;
    }

};

int main()
{
    
    string filename[] = { "edges.txt", "test1.txt","test2.txt" };
    int i = 0;

    SpanTree sptree(filename[i]);
    cout <<  "edgesum:" << sptree.Prims() <<endl;
}