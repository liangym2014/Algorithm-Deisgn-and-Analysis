// heuristic for TSP.cpp : This file contains the 'main' function. Program execution begins and ends there.
/*
Question 1
In this assignment we will revisit an old friend, the traveling salesman problem (TSP).  This week you will implement a heuristic for the TSP, 
rather than an exact algorithm, and as a result will be able to handle much larger problem sizes.  Here is a data file describing a TSP instance
(original source: http://www.math.uwaterloo.ca/tsp/world/bm33708.tsp).

nn.txt
The first line indicates the number of cities. Each city is a point in the plane, and each subsequent line indicates the x- and y-coordinates of a single city.

The distance between two cities is defined as the Euclidean distance --- that is, two cities at locations (x,y)(x,y) and (z,w)(z,w) have distance \sqrt{(x-z)^2 + (y-w)^2}
(x−z)2 +(y−w)2 between them.

You should implement the nearest neighbor heuristic:

Start the tour at the first city.
Repeatedly visit the closest city that the tour hasn't visited yet.  In case of a tie, go to the closest city with the lowest index.  For example, 
if both the third and fifth cities have the same distance from the first city (and are closer than any other city), then the tour should begin by 
going from the first city to the third city.
Once every city has been visited exactly once, return to the first city to complete the tour.
In the box below, enter the cost of the traveling salesman tour computed by the nearest neighbor heuristic for this instance, rounded down to the nearest integer.

[Hint: when constructing the tour, you might find it simpler to work with squared Euclidean distances (i.e., the formula above but without the square root)
than Euclidean distances.  But don't forget to report the length of the tour in terms of standard Euclidean distance.]
*/

#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>
#include <cmath>
#include <limits>
using namespace std;


class Graph {
private:
    unordered_map<int, pair<double, double>> nodes;
    int num_nodes;
    pair<double, double> start; //node 1

public:
    Graph(string& filename) {
        ifstream f;
        f.open(filename);

        if (f.is_open()) {
            int idx;
            double xs, ys;

            f >> num_nodes;

            while (!f.eof()) {
                f >> idx >> xs >> ys;
                nodes[idx] = {xs, ys};
            }
            f.close();
        }

        start = nodes[1];
        //printf("%f %f %f\n", nodes[4978].first, nodes[num_nodes].first, nodes[num_nodes].second);
    }

    inline double compute_distance(pair<double, double>& node1, pair<double, double>& node2) {
        return pow(node1.first - node2.first, 2) + pow(node1.second - node2.second, 2);
    }

    double nn_cost() {
        double cost = 0;
        int current = 1;

        while (num_nodes) {
            double min_distance = DBL_MAX;
            int next = -1;
            pair<double, double> cur = nodes[current];
            nodes.erase(current);

            for (auto& node : nodes) {
                double distance = compute_distance(cur, node.second);
                if (distance > min_distance)
                    continue;

                if (distance < min_distance) {
                    min_distance = distance;
                    next = node.first;
                }
                else 
                    next = min(next, node.first);
                
            }
            cost += (-- num_nodes)? sqrt(min_distance): sqrt(compute_distance(start, cur));
            current = next;
            //printf("%i %f\n", next, cost);
        }

        return cost;
    }
};



int main()
{
    vector<string> files = { "nn.txt", "test1.txt", "test2.txt","test3.txt","test 50.txt","test 51.txt", "test 1000.txt" };
    Graph g(files[0]);
    printf("%f\n", g.nn_cost());

    cin.get();
    return 0;
}

