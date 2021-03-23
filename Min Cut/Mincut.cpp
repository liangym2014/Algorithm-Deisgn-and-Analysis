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
#include <vector>
#include <random>

using namespace std;


class Graph {
private:
    vector<pair<int,int>> edges;
    vector<int> unions;  //0: undefined, >= 0: defined
    vector<int> random_edge_order;
    int n;  //number of nodes
    

public:
    Graph(string filename, int num_nodes): n(num_nodes){
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                //the first one is vertex
                int start = 0;
                size_t pos = line.find('\t', start);
                if (pos == string::npos)
                    continue;

                int node1 = stoi(line.substr(start, pos - start));
                start = pos + 1;

                pos = line.find('\t', start);
                while (pos != string::npos) {
                    int node2 = stoi(line.substr(start, pos - start));
                    if(node1 < node2)
                        edges.push_back({node1, node2}); //node1 < node2, remove self-loop and duplicate edges
                    start = pos + 1;
                    pos = line.find('\t', start);
                }

            }
            file.close();
        }
        else
            cout << "can't open file.\n";
    }

    int find_union(int idx) {
        if (!idx)
            return idx;

        if (unions[idx] != idx)
            unions[idx] = find_union(unions[idx]);

        return unions[idx];
    }

    void merge(int &node1, int &node2) {
        if (!unions[node1]) {
            if (!unions[node2]) {
                unions[node1] = node1;
                unions[node2] = node1;
            }
            else {
                int node2_union_idx = find_union(unions[node2]);
                unions[node1] = node2_union_idx;
            }
        }
        else {
            if (!unions[node2]) {
                int node1_union_idx = find_union(unions[node1]);
                unions[node2] = node1_union_idx;
            }
            else {
                int node1_union_idx = find_union(unions[node1]);
                int node2_union_idx = find_union(unions[node2]);

                if (node1_union_idx != node2_union_idx)
                    unions[node2_union_idx] = node1_union_idx;
            }
        }
    }

    int RandomizedContracted() {
        int remain = n;  //the number of remaining nodes
        unions = vector<int>(n + 1);
        
        //rearrange random_edge_order
        random_shuffle(random_edge_order.begin(), random_edge_order.end());

        int edge = 0;

        while (remain > 2) {
            pair<int, int> rand_edge = edges[random_edge_order[edge ++]];

            int node1 = rand_edge.first, node2 = rand_edge.second;
            int node1_union_idx = find_union(unions[node1]);
            int node2_union_idx = find_union(unions[node2]);
            if (!node1_union_idx || !node2_union_idx || node1_union_idx != node2_union_idx){
                remain --;
                merge(node1, node2);
            }
        }

        int count = 0; //the number of cross edges

        while (edge < random_edge_order.size()) {
            pair<int, int> rand_edge = edges[random_edge_order[edge ++]];

            int node1 = rand_edge.first, node2 = rand_edge.second;
            int node1_union_idx = find_union(unions[node1]);
            int node2_union_idx = find_union(unions[node2]);

            if (!node1_union_idx)
                unions[node1] = node1;
            else if(!node2_union_idx)
                unions[node2] = node2;
            else if (node1_union_idx != node2_union_idx)
                count ++;

        }

        return count;
    }

    int RepeatRandomizedContracted() {
        int i = n * n * log(n);

        //indices from 0 to edges.size() - 1
        for (int j = 0; j < edges.size(); ++j)
            random_edge_order.push_back(j);

        int minCut = RandomizedContracted();
        //cout << i << endl;
        while (-- i) {
            int cut = RandomizedContracted();
            //cout<<cut<<endl;
            if (cut < minCut) 
                minCut = cut;
        }
        return minCut;
    }

};


int main()
{
    vector<string> file{ "kargerMinCut.txt", "test.txt"};
    vector<int> num_nodes = { 200, 8 };
    int i = 0;

    Graph graph(file[i], num_nodes[i]);
    cout << graph.RepeatRandomizedContracted() << endl;

}
