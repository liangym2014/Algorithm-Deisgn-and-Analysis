// TSP.cpp : Defines the entry point for the console application.
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
//#include <algorithm>
#include <unordered_map>

using namespace std;

typedef unsigned long mask_type;
typedef double length;

typedef vector<pair<char, length>> vec_value;
typedef unordered_map<mask_type, vec_value> umapv;

typedef unordered_map<char,length> map_value;
typedef unordered_map<mask_type,map_value> umap;


class TSP {
private:
    vector<vector<length>> distance;  //city coordinates and distances
    int num_cities;  //number of cities

public:
    TSP(string &filename) {
        ifstream file(filename);
        vector<pair<length, length>> cities;

        if (file.is_open()) {
            length x, y;
            file >> num_cities;
          
            while (file >> x >> y)
                cities.push_back(make_pair(x, y));

            file.close();

            distance = vector<vector<length>>(num_cities, vector<length>(num_cities));
            for (int i = 0; i < cities.size(); ++ i) {
                for (int j = i + 1; j < cities.size(); ++ j) {
                    distance[i][j] = sqrt(pow(cities[i].first - cities[j].first, 2) + pow(cities[i].second - cities[j].second, 2));
                    distance[j][i] = distance[i][j];
                }
            }
        }
    }

    // returns the next larger number with the same number of set bits as x.
    mask_type generate_next_mask(mask_type x) {
        mask_type rightOne, nextHigherOneBit, rightOnesPattern, next = 0;

        if (x) {
            // right most set bit
            rightOne = x & -(signed)x;

            // reset the pattern and set next higher bit
            // left part of x will be here
            nextHigherOneBit = x + rightOne;

            // isolate the pattern
            rightOnesPattern = x ^ nextHigherOneBit;

            // right adjust pattern
            rightOnesPattern = (rightOnesPattern) / rightOne;

            // correction factor
            rightOnesPattern >>= 2;

            // integrate new pattern
            next = nextHigherOneBit | rightOnesPattern;
        }

        return next;
    }

    //compute the subset of the corresponding mask.
    //each element in subset is a city in the path.
    vector<int> mask_to_subset(mask_type &mask) {
        vector<int> subset;

        for (size_t pos = 0; pos + 1 < num_cities; pos ++) {
            if (mask & (1 << pos))
                subset.push_back(pos + 1);
        }
        return subset;
    }

    //return the path length when last city == node
    length find_length(vec_value& vec, int& node) {
        for (auto& e : vec) {
            if (e.first != node)
                continue;
            return e.second;
        }
    }


    long Minicost_Held_Karp_v3() {
        //subproblem: the number of set bits indicates the size of subproblem.
        //in the binary format, the least significant digit corresponds to city 1.
        //for each digit, "1" means the corresponding city is included in path and "0" means not included 
        int subproblem = 1;


        //key: mask(predecessors)
        //value: pair<char,length>,first: the last city i; second: the shortest path length from city 0 to city i
        umapv previous, current;
        previous.insert({ 0, vec_value() });

        //in the 1st traversal, compute the path length between city i and city 0.
        //val_ptr point to the value of umapv, which is a vector
        vec_value *val_ptr = &previous[0];
        for (int i = 1; i < num_cities; i++) 
            val_ptr->push_back({ i, distance[i][0] });

        int j = 0;  //iterate through the subproblems from small size to large size
        mask_type max = (1 << (num_cities - 1)) - 1, mask = subproblem;

        while (1) {
            if (mask > max) {
                //already go through all subproblems, exit
                if (j + 2 == num_cities)
                    break;
                //the size of subproblem increases by 1
                subproblem |= (1 << (++ j));
                mask = subproblem;

                //the elements in current are results of subproblem size j, assign them to previous
                previous = current;

                //release the memory assigned to current
                current.clear();
            }

            vector<int> subset = mask_to_subset(mask);
            //a new mask is generated and push into hashtable. its value is an empty vector pointed to by val_ptr
            current[mask] = vec_value();
            val_ptr = &current[mask];

            //if mask <= max, compute the shortest path of each city from city 0 through all the cities indicated by the mask
            for (int i = num_cities - 1; i >= 0; i --) {
                //path without all the cities in it will not be computed
                if (!i && mask!= max)
                    continue;

                if (i){
                    //path with predecessors including city i will not be computed 
                    if(mask != max && (mask & (1 << (i - 1))))
                        continue;
                    //path with all the cities as predecessor but not leading to city 0 will not be computed
                    if (mask == max)
                        continue;
                }

                length min_length = 0xffffffff;

                for (int t : subset) {
                    //subpath = predecessors with city t as the last node
                    mask_type subpath = mask & ~(1 << (t-1));

                    //len = distace between city t and city i + subpath with city t as last node
                    length len = distance[i][t] + find_length(previous[subpath], t);

                    //min set as the minimum of all the subpath leading to city i
                    min_length = min(min_length, len);
                }
                //min is the shortest path starting at city 0 and ending at city i
                val_ptr->push_back({ i, min_length });
            }

            //generate next mask
            mask = generate_next_mask(mask);
        }

        return long(current[max].begin()->second);
    }


    long Minicost_Held_Karp_v4() {
        //subproblem: the number of set bits indicates the size of subproblem.
        //in the binary format, the least significant digit corresponds to city 1.
        //for each digit, "1" means the corresponding city is included in path and "0" means not included 
        int subproblem = 1;


        //key: mask(predecessors)
        //value: unordered_map<char,length>, key: the last city i; value: the shortest path length from city 0 to city i
        umap previous, current;
        previous.insert({ 0, map_value() });

        //in the 1st traversal, compute the path length between city i and city 0.
        //val_ptr point to the value of umap, which is an unordered_map
        map_value* val_ptr = &previous[0];
        for (int i = 1; i < num_cities; i++)
            val_ptr->insert({ i, distance[i][0] });

        int j = 0;  //iterate through the subproblems from small size to large size
        mask_type max = (1 << (num_cities - 1)) - 1, mask = subproblem;

        while (1) {
            if (mask > max) {
                //already go through all subproblems, exit
                if (j + 2 == num_cities)
                    break;
                //the size of subproblem increases by 1
                subproblem |= (1 << (++ j));
                mask = subproblem;

                //the elements in current are results of subproblem size j, assign them to previous
                previous = current;

                //release the memory assigned to current
                current.clear();
            }

            vector<int> subset = mask_to_subset(mask);
            //a new mask is generated and push into hashtable. its value is an empty unordered_map pointed to by val_ptr
            current[mask] = map_value();
            val_ptr = &current[mask];

            //if mask <= max, compute the shortest path of each city from city 0 through all the cities indicated by the mask
            for (int i = num_cities - 1; i >= 0; i--) {
                //path without all the cities in it will not be computed
                if (!i && mask != max)
                    continue;

                if (i) {
                    //path with predecessors including city i will not be computed 
                    if (mask != max && (mask & (1 << (i - 1))))
                        continue;
                    //path with all the cities as predecessor but not leading to city 0 will not be computed
                    if (mask == max)
                        continue;
                }

                length min_length = 0xffffffff;

                for (int t : subset) {
                    //subpath = predecessors with city t as the last node
                    mask_type subpath = mask & ~(1 << (t - 1));

                    //len = distace between city t and city i + subpath with city t as last node
                    length len = distance[i][t] + previous[subpath][t];

                    //min set as the minimum of all the subpath leading to city i
                    min_length = min(min_length, len);
                }
                //min is the shortest path starting at city 0 and ending at city i
                val_ptr->insert({ i, min_length });
            }

            //generate next mask
            mask = generate_next_mask(mask);
        }

        return long(current[max].begin()->second);
    }
};


int main()
{
    string filename[] = { "tsp.txt", "test2.txt" };
    int idx = 0;

    TSP tsp(filename[idx]);

    //memory: 2 GB, time: 1 min 30 sec
    cout << "minimum cost: " << tsp.Minicost_Held_Karp_v3()<< '\n';

    //memory: 11 GB, time: 3 min 08 sec
    //cout << "minimum cost: " << tsp.Minicost_Held_Karp_v4() << '\n';

    return 0;
}


