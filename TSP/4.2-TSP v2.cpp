// 4.2-TSP v2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
/*
In this assignment you will implement one or more algorithms for the traveling
salesman problem, such as the dynamic programming algorithm covered in the video
lectures. Here is a data file describing a TSP instance. The first line indicates
the number of cities. Each city is a point in the plane, and each subsequent
line indicates the x- and y-coordinates of a single city.
The distance between two cities is defined as the Euclidean distance --- that
is, two cities at locations (x,y) and (z,w) have distance sqrt((x−z)^2+(y−w)^2) between them.

In the box below, type in the minimum cost of a traveling salesman tour for this
instance, rounded down to the nearest integer.

OPTIONAL: If you want bigger data sets to play with, check out the TSP instances
from around the world here. The smallest data set (Western Sahara) has 29 cities
, and most of the data sets are much bigger than that. What's the largest of
these data sets that you're able to solve --- using dynamic programming or, if
you like, a completely different method?

HINT: You might experiment with ways to reduce the data set size. For example,
trying plotting the points. Can you infer any structure of the optimal solution?
Can you use that structure to speed up your algorithm?
*/

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <unordered_map>

using namespace std;
 
vector<vector<double>> compute_distance(string &filename) {
    ifstream file(filename);
    vector<vector<double>> distance;

    if (file.is_open()) {
        double x, y;
        int n;
        vector<pair<double, double>> cities;
        file >> n;
        while (file >> x >> y)
            cities.push_back({ x, y });

        file.close();

        distance = vector<vector<double>> (n, vector<double>(n));
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                distance[i][j] = sqrt(pow(cities[i].first - cities[j].first, 2) + pow(cities[i].second - cities[j].second, 2));
                distance[j][i] = distance[i][j];
            }
        }
    }
    return distance;
}


double compute_cost(vector<vector<double>> &distance, int num_cities) {
    //origin: city 0
    const int origin = 0;

    //key: 32-bit integer, bit 1 ~ 24 bits for visited cities, bit 26 ~ 30 for destination city
    //value: distance from city 0 to destination city, travelling through visited cities
    unordered_map<int, double> costmap, m;
    const int destination_bit = 26, path_operator = 0x1ffffff;


    for (int destination = 1; destination < num_cities; ++ destination) {//subproblem size = 2
        int key = 0;
        key |= (1 << destination);
        key |= (destination << destination_bit);
        costmap[key] = distance[origin][destination];
    }

    for (int i = 3; i <= num_cities; ++ i) { //subproblem size
        cout << "subproblem size: " << i;
         for (int destination = 1; destination < num_cities; ++ destination) {
            for (auto &cost: costmap) {
                if ((cost.first >> destination) & 1)
                    continue;

                int j = (cost.first >> destination_bit);  //the destination city of previous path
                int key = (cost.first & path_operator) | (1 << destination);  //add current city to the previous path
                key |= (destination << destination_bit);    //update destination city as current city

                //cost(origin, destination) = cost(origin, j) + distance(j, destination)
                double val = cost.second + distance[j][destination];

                m[key] = m.count(key)? min(m[key], val): val;
            }
        }

        costmap = m;
        m.clear();
        cout << ", map size: " << costmap.size() << endl;
    }

    double min_cost = DBL_MAX;

    for (auto &cost : costmap) {
        int j = (cost.first >> destination_bit);
        min_cost = min(min_cost, cost.second + distance[j][origin]);
    }

    return min_cost;
}


int main()
{
    string filename[] = { "tsp.txt", "test2.txt" };
    int idx = 0;

    vector<vector<double>> distance = compute_distance(filename[idx]);

    //vector<vector<double>> distance{{0,10,15,20},{10,0,35,25},{15,35,0,30},{20,25,30,0}};  //4 cities, 80
    //vector<vector<double>> distance{{0,2,9,10},{1,0,6,4},{15,7,0,8},{6,3,12,0}};    //4 cities, 21
    //vector<vector<double>> distance{{0,3,4,2,7},{3,0,4,6,3},{4,4,0,5,8},{2,6,5,0,6},{7,3,8,6,0}};   //5 cities, 19

    double cost = compute_cost(distance, distance.size());

    cout << int(cost) << endl;
}



