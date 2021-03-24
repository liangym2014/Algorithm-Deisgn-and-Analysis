// Median Maintenance.cpp : Defines the entry point for the console application.
//
/*
The goal of this problem is to implement the "Median Maintenance" algorithm
(covered in the Week 5 lecture on heap applications). The text file contains
a list of the integers from 1 to 10000 in unsorted order; you should treat 
this as a stream of numbers, arriving one by one. Letting xi denote the ith
number of the file, the kth median mk is defined as the median of the numbers
x1,…,xk. (So, if k is odd, then mk is ((k+1)/2)th smallest number among x1,…,xk;
if k is even, then mk is the (k/2)th smallest number among x1,…,xk.)

In the box below you should type the sum of these 10000 medians, modulo 10000
(i.e., only the last 4 digits). That is, you should compute 
(m1+m2+m3+⋯+m10000) mod 10000.

OPTIONAL EXERCISE: Compare the performance achieved by heap-based and 
search-tree-based implementations of the algorithm.
*/
#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <functional>
using namespace std;

int main()
{
    vector<string> filename{ "Median.txt","test.txt" };
    vector<int> n{ 10000, 8 };  //number of numbers
    int i = 0;

    vector<int> source;
    ifstream file(filename[i]);

    if (file.is_open()) {
        int number;

        while (!file.eof()) {
            file >> number;
            source.push_back(number);
        }
        file.close();

        //for(int i=0;i<source.size();i++)
          //cout<<source[i]<<endl;

        priority_queue<int, vector<int>, greater<int>> highheap;   //min heap
        priority_queue<int> lowheap;  //max heap
        lowheap.push(source[0]);
        int sum = source[0];

        for (int i = 1; i < source.size(); ++ i) {
            int num = source[i];

            if (lowheap.size() == highheap.size()) {
                if (num > lowheap.top()) {
                    highheap.push(num);
                    sum += highheap.top();
                }
                else {
                    lowheap.push(num);
                    sum += lowheap.top();
                }
                continue;
            }

            if (lowheap.size() > highheap.size()) {
                if (num > lowheap.top())
                    highheap.push(num);
                else {
                    highheap.push(lowheap.top());
                    lowheap.pop();
                    lowheap.push(num);
                }
            }
            else {
                if (num < highheap.top())
                    lowheap.push(num);
                else {
                    lowheap.push(highheap.top());
                    highheap.pop();
                    highheap.push(num);
                }
            }
            sum += lowheap.top();
        }

        cout << "result : " << (sum % 10000) << endl;
    }
    cin.get();
    return 0;
}



