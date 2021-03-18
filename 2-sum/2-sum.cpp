// 2-sum.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
/*
The goal of this problem is to implement a variant of the 2-SUM algorithm
(covered in the Week 6 lecture on hash table applications).
The file contains 1 million integers, both positive and negative (there might be
some repetitions!).This is your array of integers, with the ith row of the file
specifying the ith entry of the array.

Your task is to compute the number of target values t in the interval
[-10000,10000] (inclusive) such that there are distinct numbers x,y in the input
file that satisfy x+y=t. (NOTE: ensuring distinctness requires a one-line
addition to the algorithm from lecture.)

Write your numeric answer (an integer between 0 and 20001) in the space provided.

*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_set>
using namespace std;

void data_input(string filename, unordered_set<long long> &unique) {
    fstream f(filename);

    if (f.is_open()) {
        string num;
        while (!f.eof()) {
            f >> num;
            unique.insert(stoll(num));
        }
        f.close();
    }
}

int main()
{
    string filename[] = {"algo1-programming_prob-2sum.txt", "test.txt"};
    pair<long, long> t[] = { {-10000, 10000},               {-5, 7} };
    int idx = 0;

    unordered_set<long long> unique;
    data_input(filename[idx], unique);

    vector<long long> nums(unique.begin(), unique.end());
    sort(nums.begin(), nums.end());

    long long t_min = t[idx].first, t_max = t[idx].second;
    unordered_set<long long> ans;

    for (int i = 0; i < nums.size(); ++ i) {
        if (nums[i] > t_max)
            break;

        long long p1 = t_min - nums[i];
        auto it1 = lower_bound(nums.begin() + i + 1, nums.end(), p1);
        if (it1 == nums.end() || *it1 + nums[i] > t_max)
            continue;

        int pos1 = it1 - nums.begin();

        long long p2 = t_max - nums[i];
        auto it2 = upper_bound(nums.begin() + i + 1, nums.end(), p2);
        int pos2 = it2 - nums.begin() - 1;

        for (int j = pos1; j <= pos2; ++ j) {
            ans.insert(nums[i] + nums[j]);
        }
    }

    cout<<ans.size();
}

