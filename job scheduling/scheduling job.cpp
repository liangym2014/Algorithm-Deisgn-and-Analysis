// scheduling job.cpp : Defines the entry point for the console application.
//
/*
In this programming problem and the next you'll code up the greedy algorithms
from lecture for minimizing the weighted sum of completion times.. Download the
text file here. This file describes a set of jobs with positive and integral
weights and lengths. It has the format
[number_of_jobs]
[job_1_weight] [job_1_length]
[job_2_weight] [job_2_length]
...
For example, the third line of the file is "74 59", indicating that the second
job has weight 74 and length 59. You should NOT assume that edge weights or 
lengths are distinct.

Your task in this problem is to run the greedy algorithm that schedules jobs
in decreasing order of the difference (weight - length). Recall from lecture
that this algorithm is not always optimal. IMPORTANT: if two jobs have equal
difference (weight - length), you should schedule the job with higher weight
first. Beware: if you break ties in a different way, you are likely to get
the wrong answer. You should report the sum of weighted completion times of the
resulting schedule --- a positive integer --- in the box below.

For this problem, use the same data set as in the previous problem. Your task 
now is to run the greedy algorithm that schedules jobs (optimally) in decreasing
order of the ratio (weight/length). In this algorithm, it does not matter how 
you break ties. You should report the sum of weighted completion times of the 
resulting schedule --- a positive integer --- in the box below.
*/
#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
using namespace std;

bool cmp_by_difference(const pair<int, int> &l, const pair<int, int> &r) {
    int difference_1 = l.first - l.second, difference_2 = r.first - r.second;
    return (difference_1 > difference_2) ? true : (difference_1 == difference_2 ? (l.first > r.first) : false);
}

bool cmp_by_ratio(const pair<int, int> &l, const pair<int, int> &r) {
    double ratio_1 = 1.0 * l.first/ l.second, ratio_2 = 1.0 * r.first/ r.second;
    return ratio_1 > ratio_2;
}

class Jobs{
private:
    vector<pair<int, int>> jobs;

public:
    Jobs(string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            int weight, length;
            file >> weight;
            while (file >> weight >> length)
                jobs.push_back({weight, length});

            file.close();
        }
    }

    long long difference_calculation() {
        sort(jobs.begin(), jobs.end(), cmp_by_difference);
        long long weighted_complete_time = 0, length = 0;

        for (int i = 0; i < jobs.size(); ++ i) {
            length += jobs[i].second;
            weighted_complete_time += jobs[i].first * length;
        }
        return weighted_complete_time;
    }

    long long ratio_calculation() {
        sort(jobs.begin(), jobs.end(), cmp_by_ratio);
        long long weighted_complete_time = 0, length = 0;

        for (int i = 0; i < jobs.size(); ++i) {
            length += jobs[i].second;
            weighted_complete_time += jobs[i].first * length;
        }
        return weighted_complete_time;
    }

};

int main()
{
    string filename[] = { "jobs.txt", "test.txt","test1.txt" };
    int i = 0;

    Jobs jobs(filename[i]);
    cout << "difference completion time:" << jobs.difference_calculation() << endl;
    cout << "ratio completion time:" << jobs.ratio_calculation() << endl;

    cin.get();
    return 0;
}

