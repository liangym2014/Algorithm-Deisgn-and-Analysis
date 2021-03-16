// knapsack.cpp : Defines the entry point for the console application.
/*
Q1:
This file describes a knapsack instance, and it has the following format:
[knapsack_size][number_of_items]
[value_1] [weight_1]
[value_2] [weight_2]
...
For example, the third line of the file is "50074 659", indicating that the
second item has value 50074 and size 659, respectively.
You can assume that all numbers are positive. You should assume that item
weights and the knapsack capacity are integers.

Q2:
This file describes a knapsack instance, and it has the following format:
[knapsack_size][number_of_items]
[value_1] [weight_1]
[value_2] [weight_2]
...
For example, the third line of the file is "50074 834558", indicating that the
second item has value 50074 and size 834558, respectively. As before, you should
assume that item weights and the knapsack capacity are integers.

This instance is so big that the straightforward iterative implemetation uses an
infeasible amount of time and space. So you will have to be creative to compute
an optimal solution. One idea is to go back to a recursive implementation, 
solving subproblems --- and, of course, caching the results to avoid redundant
work --- only on an "as needed" basis. Also, be sure to think about appropriate
data structures for storing and looking up solutions to subproblems.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;


class Ksack{
private:
    vector<int> dp;
    vector<pair<int, int>> items; //{value, weight}

public:
    Ksack(string filename) {
        ifstream file(filename);
        int capacity = 0, num_items = 0, value, weight;

        if (file.is_open()) {
            file >> capacity >> num_items;
            while (!file.eof()) {
                file >> value >> weight;
                items.push_back({ value, weight });
            }
            file.close();
        }
        dp = vector<int>(capacity + 1, -1);
        dp[0] = 0;
    }

    int max_Value() {
        int mx = 0;

        for (auto &item:items) {
            for (int i = dp.size() - 1; i >= item.second; --i) {
                if (dp[i - item.second] < 0)  continue;
                dp[i] = max(dp[i], dp[i - item.second] + item.first);
                mx = max(mx, dp[i]);
            }
        }
        return mx;
    }
};

int main()
{
  string filename[] = {"knapsack1.txt",
                       "knapsack_big.txt",
                       "test1.txt",
                       "test2.txt",
                       "test3.txt",
                       "test4.txt",
                       "test5.txt",
                       "test6.txt",
                       "test7.txt" };

  Ksack k(filename[0]);
  cout<<k.max_Value()<<endl;
  return 0;
}

