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
                        edges.push_back({node1, node2});    //node1 < node2, remove self-loop and duplicate edges
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

    pair<int, int> pick_random_edge(int &edge_num) {
        int rand_idx = rand() % (1 + edge_num);
        swap(edges[rand_idx], edges[edge_num]); //randomly pick an edge in selectable range [0 ~ edge_num], swap it and edges[edge_num]
        return edges[edge_num --];  //selectable range decrease by 1
    }


    int RandomizedContracted(int edge_num) {
        int remain = n;  //the number of remaining nodes
        unions = vector<int>(n + 1);

        while (remain > 2) {
            pair<int, int> rand_edge = pick_random_edge(edge_num);

            int node1 = rand_edge.first, node2 = rand_edge.second;
            int node1_union_idx = find_union(unions[node1]);
            int node2_union_idx = find_union(unions[node2]);
            if (!node1_union_idx || !node2_union_idx || node1_union_idx != node2_union_idx){
                remain --;
                merge(node1, node2);
            }
        }

        int count = 0; //the number of cross edges

        while (edge_num >= 0) {
            pair<int, int> rand_edge = pick_random_edge(edge_num);

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
        int last_edge_index = edges.size() - 1, i = n * n * log(n);
        int minCut = RandomizedContracted(last_edge_index);
        //cout << i << endl;
        while (-- i) {
            int cut = RandomizedContracted(last_edge_index);
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
    int i = 1;

    Graph graph(file[i], num_nodes[i]);
    cout << graph.RepeatRandomizedContracted() << endl;

}
