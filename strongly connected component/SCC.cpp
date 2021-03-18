// SCC.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

/*
The file contains the edges of a directed graph. Vertices are labeled as
positive integers from 1 to 875714. Every row indicates an edge, the vertex
label in first column is the tail and the vertex label in second column is the
head (recall the graph is directed, and the edges are directed from the first
column vertex to the second column vertex). So for example, the 11th row looks
liks : "2 47646". This just means that the vertex with label 2 has an outgoing
edge to the vertex with label 47646

Your task is to code up the algorithm from the video lectures for computing
strongly connected components (SCCs), and to run this algorithm on the given
graph.

Output Format: You should output the sizes of the 5 largest SCCs in the given
graph, in decreasing order of sizes, separated by commas (avoid any spaces).
So if your algorithm computes the sizes of the five largest SCCs to be 500,
400, 300, 200 and 100, then your answer should be "500,400,300,200,100". If
your algorithm finds less than 5 SCCs, then write 0 for the remaining terms.
Thus, if your algorithm computes only 3 SCCs whose sizes are 400, 300, and
100, then your answer should be "400,300,100,0,0".

WARNING: This is the most challenging programming assignment of the course.
Because of the size of the graph you may have to manage memory carefully. The
best way to do this depends on your programming language and environment, and
we strongly suggest that you exchange tips for doing this on the discussion forums.
*/

#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <fstream>
#include <algorithm>
using namespace std;

struct Node {
    vector<int> edges;
    int index = 0; //index == 0: undefined
    int lowlink = 0; //nodes in the same SCC have the same lowlink, which is the lowest index in this SCC
    bool on_stack = false;
};


class Graph {
private:
    int desired_num;    // the desired number of largest SCCs
    vector<Node> nodes;
    vector<int> sccs;   //size of the desired SCCs
    stack<int> path;   //path of a unvisited node to all the nodes it can reach


public:
    Graph(string filename, int num_nodes): desired_num(5){
        nodes = vector<Node>(num_nodes + 1);

        fstream f(filename);
        if (f.is_open()) {
            string node1, node2;

            while (!f.eof()) {
                f >> node1 >> node2;
                nodes[stoi(node1)].edges.push_back(stoi(node2));
            }
            f.close();
        }
    }


    void compute_scc_with_tarjan() {
        for (int i = 1, idx = 1; i < nodes.size(); ++ i) {
            if (nodes[i].index)
                continue;

            stack<pair<int, int>> dfs; //{node, the index of next edge}
            dfs.push({i, 0});
            path.push(i);
            nodes[i].index = idx;
            nodes[i].lowlink = idx;
            nodes[i].on_stack = true;
            ++ idx;

            while (!dfs.empty()) {
                int node = dfs.top().first, edge_idx = dfs.top().second;

                if(edge_idx < nodes[node].edges.size()) {
                    int e = nodes[node].edges[edge_idx];
                    dfs.top().second ++;

                    if (!nodes[e].index) {
                        dfs.push({e, 0});
                        path.push(e);
                        nodes[e].index = idx;
                        nodes[e].lowlink = idx;
                        nodes[e].on_stack = true;
                        ++ idx;
                    }
                    else if (nodes[e].on_stack) {
                        nodes[node].lowlink = min(nodes[node].lowlink, nodes[e].lowlink);
                    }
                }
                else {
                    if (nodes[node].index == nodes[node].lowlink) {//this is the start point of the SCC
                        int count = 1; //the size of the SCC = the number of nodes on top of nodes[node] + nodes[node]

                        while (nodes[path.top()].index != nodes[node].index) {
                            ++ count;
                            nodes[path.top()].on_stack = false;
                            path.pop();
                        }

                        //update sccs
                        if (sccs.size() < desired_num) {
                            sccs.push_back(count);
                            push_heap(sccs.begin(), sccs.end(),greater<int>());
                        }
                        else if (sccs.front() < count) {
                            pop_heap(sccs.begin(), sccs.end(), greater<int>());
                            sccs.back() = count;
                            push_heap(sccs.begin(), sccs.end(), greater<int>());
                        }
                        
                        //pop out the start point
                        nodes[node].on_stack = false;
                        path.pop();
                    }

                    dfs.pop();

                    if (!dfs.empty())
                        nodes[dfs.top().first].lowlink = min(nodes[node].lowlink, nodes[dfs.top().first].lowlink);

                }
            }
        }
        
        if (sccs.size() < desired_num)
            cout << "only " << sccs.size() << " SCCS:\n";

        sort(sccs.begin(), sccs.end(), greater<int>());

        for(auto &scc: sccs)
            cout << scc << ", ";

        cout << endl;
    }
};

int main()
{
    string files[] = {"SCC.txt", "test.txt", "test0.txt", "test1.txt","test2.txt","test3.txt","test4.txt", "test5.txt" };
    int num_nodes[] = { 875714,    11,         11,           4,           9,          14,         7,            8};
    int i = 0;

    Graph g(files[i], num_nodes[i]);
    g.compute_scc_with_tarjan();
    return 0;
}

