// InversionCount.cpp : Defines the entry point for the console application.
/*
This file contains all of the 100,000 integers between 1 and 100,000
(inclusive) in some order, with no integer repeated.

Your task is to compute the number of inversions in the file given, where the
ith row of the file indicates the ith entry of an array.
Because of the large size of this array, you should implement the fast divide
-and-conquer algorithm covered in the video lectures.
*/
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

long long SplitInversionCount(vector<int>& array, int l, int r, int mid) {
    int left = l, right = mid + 1;
    long long count = 0;
    vector<int> sortedArray;

    while (left <= mid && right <= r) {
        if (array[left] < array[right])
            sortedArray.push_back(array[left ++]);
        else {
            sortedArray.push_back(array[right ++]);
            count += mid - left + 1;
        }
    }

    if (right <= r)
        sortedArray.insert(sortedArray.end(), array.begin() + right, array.begin() + r + 1);
    else if (left <= mid)
        sortedArray.insert(sortedArray.end(), array.begin() + left, array.begin() + mid + 1);


    for (auto& num: sortedArray)
        array[l ++] = num;

    return count;
}

long long InversionCount(vector<int>& array, int l, int r) {
    if (l >= r)
        return 0;

    int mid = (l + r) / 2;
    long long leftcount = InversionCount(array, l, mid);
    long long rightcount = InversionCount(array, mid + 1, r);
    long long splitcount = SplitInversionCount(array, l, r, mid);
    return leftcount + rightcount + splitcount;

}

int main()
{
    ifstream file;
    string line;
    vector<int> arr;
    file.open("IntegerArray.txt");

    if (file.is_open()) {
        while (getline(file, line))
            arr.push_back(stoi(line));
        file.close();
    }

    long long count = InversionCount(arr, 0, arr.size() - 1);

    cout << count << endl;

}




