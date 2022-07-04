// Mincut.cpp : Defines the entry point for the console application.
/*
The file contains the adjacency list representation of a simple undirected
graph. There are 200 vertices labeled 1 to 200. The first column in the file
represents the vertex label, and the particular row (other entries except the
first column) tells all the vertices that the vertex is adjacent to. So for 
example, the 6th row looks like : "6	155	56	52	120	......". This just means
that the vertex with label 6 is adjacent to (i.e., shares an edge with) the 
vertices with labels 155,56,52,120,......,etc

Your task is to code up and run the randomized contraction algorithm for the 
min cut problem and use it on the above graph to compute the min cut (i.e., 
the minimum-possible number of crossing edges). (HINT: Note that you'll have 
to figure out an implementation of edge contractions. Initially, you might want
to do this naively, creating a new graph from the old every time there's an 
edge contraction. But you should also think about more efficient 
implementations.) (WARNING: As per the video lectures, please make sure to run
the algorithm many times with different random seeds, and remember the smallest
cut that you ever find.) 
*/

#include "stdafx.h"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <random>
#include <numeric>

using namespace std;

class Graph {
private:
    vector<pair<int,int>> edges;
    vector<int> unions;
    vector<int> random_edge_order;
    int n;  //number of nodes
    

public:
    Graph(string filename, int num_nodes): n(num_nodes), unions(num_nodes + 1) {
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                //the first one is vertex
                int i = 0, start;
                string end;
                istringstream _line = istringstream(line);
                while (getline(_line, end, '\t')) {
                    if ((i++) == 0) {
                        start = stoi(end);
                        continue;
                    }
                    int node = stoi(end);
                    if (start < node)
                        edges.emplace_back(start, node);

                }
            }
            file.close();
            return;
        }

        cout << "can't open file.\n";
    }

    int find(vector<int>& unions, int idx) {
        if (unions[idx] != idx)
            unions[idx] = find(unions, unions[idx]);

        return unions[idx];
    }

    int RandomizedContracted(vector<int> unions) {
        int remain = n;  //the number of remaining nodes
        
        //shuffle edges
        random_shuffle(random_edge_order.begin(), random_edge_order.end());

        int edge = 0;

        while (remain > 2) {
            pair<int, int> rand_edge = edges[random_edge_order[edge ++]];

            int node1 = rand_edge.first, node2 = rand_edge.second;
            int node1_union_idx = find(unions,node1), node2_union_idx = find(unions,node2);
            if (node1_union_idx != node2_union_idx){
                remain --;
                unions[node2_union_idx] = node1_union_idx;
            }
        }

        int count = 0; //the number of cross edges

        while (edge < random_edge_order.size()) {
            pair<int, int> rand_edge = edges[random_edge_order[edge ++]];

            int node1 = rand_edge.first, node2 = rand_edge.second;
            int node1_union_idx = find(unions,node1), node2_union_idx = find(unions,node2);

            if (node1_union_idx != node2_union_idx)
                count ++;

        }

        return count;
    }

    int RepeatRandomizedContracted() {
        int i = n * n * log(n);
        random_edge_order.resize(edges.size());
        iota(random_edge_order.begin(), random_edge_order.end(), 0);  // indices for edges
        iota(unions.begin(), unions.end(), 0);   // initial unions each vertex belongs to

        cout << "start computing:\n";
        int minCut = RandomizedContracted(unions);

        while (-- i) {
            int cut = RandomizedContracted(unions);
            if (cut < minCut) 
                minCut = cut;
        }
        return minCut;
    }

};


int main()
{
    string file[] = {"kargerMinCut.txt", "test.txt"};
    int num_nodes[] = {200, 8};
    int i = 0;

    Graph graph(file[i], num_nodes[i]);
    cout << graph.RepeatRandomizedContracted() << endl;

}
