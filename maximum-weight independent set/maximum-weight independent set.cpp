// maximum-weight independent set.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
/*
In this programming problem you'll code up the dynamic programming algorithm for computing a maximum-weight independent set of a path graph.

This file describes the weights of the vertices in a path graph (with the weights listed in the order in which vertices appear in the path). It has the following format:

[number_of_vertices]

[weight of first vertex]

[weight of second vertex]

...

For example, the third line of the file is "6395702," indicating that the weight of the second vertex of the graph is 6395702.

Your task in this problem is to run the dynamic programming algorithm (and the reconstruction procedure) from lecture on this data set.
The question is: of the vertices 1, 2, 3, 4, 17, 117, 517, and 997, which ones belong to the maximum-weight independent set?
(By "vertex 1" we mean the first vertex of the graph---there is no vertex 0.)
In the box below, enter a 8-bit string, where the ith bit should be 1 if the ith of these 8 vertices is in the maximum-weight independent set, and 0 otherwise.
For example, if you think that the vertices 1, 4, 17, and 517 are in the maximum-weight independent set and the other four vertices are not, then you should enter the string 10011010 in the box below.
*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

void data_input(vector<int>& vs, string& filename) {
    ifstream f;
    f.open(filename);

    if (f.is_open()) {
        string element;
        f >> element;
        vs = vector<int>(stoi(element) + 1);

        int i = 1;
        while (!f.eof()) {
            f >> element;
            vs[i ++] = stoi(element);
        }
        f.close();
    }
}

int main()
{
    vector<int> vertices;
    vector<string> files{ "mwis.txt", "test1.txt","test2.txt" };
    vector<vector<int>> indices{{ 1, 2, 3, 4, 17, 117, 517, 997 },{ 1,2,3,4 }, { 1,2,3,4,5,6,7,8,9,10 }};

    int j = 0; //jth file as input data

    data_input(vertices, files[j]);
    
    vector<int> dp(vertices.size());
    dp[0] = 0;
    dp[1] = vertices[1];

    for (int i = 2; i < dp.size(); ++i) {
        dp[i] = max(dp[i - 1], dp[i - 2] + vertices[i]);
    }

    //reconstruct
    for (int i = dp.size() - 1; i > 0;) {
        if (dp[i] > dp[i - 1]) {
            vertices[i] = -1;
            i -= 2;
        }
        else {
            -- i;
        }
    }

    for (auto idx : indices[j])
        cout << idx << ": " << vertices[idx]<< endl; //vertices == -1: involved, otherwise not involved
}
