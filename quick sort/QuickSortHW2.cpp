﻿// QuickSortHW2.cpp : Defines the entry point for the console application.
/*The file contains all of the integers between 1 and 10,000 (inclusive, with
no repeats) in unsorted order. The integer in the ith row of the file gives
you the ith entry of an input array.

Your task is to compute the total number of comparisons used to sort the given
input file by QuickSort. As you know, the number of comparisons depends on
which elements are chosen as pivots, so we'll ask you to explore three 
different pivoting rules.
You should not count comparisons one-by-one. Rather, when there is a recursive
call on a subarray of length m, you should simply add m−1 to your running 
total of comparisons. (This is because the pivot element is compared to each 
of the other m−1 elements in the subarray in this recursive call.)

WARNING: The Partition subroutine can be implemented in several different 
ways, and different implementations can give you differing numbers of 
comparisons. For this problem, you should implement the Partition 
subroutine exactly as it is described in the video lectures (otherwise you
might get the wrong answer).

DIRECTIONS FOR THIS PROBLEM:

(1)For the first part of the programming assignment, you should always use the
first element of the array as the pivot element.

(2)Compute the number of comparisons (as in Problem 1), always using the final
element of the given array as the pivot element. Again, be sure to implement
the Partition subroutine exactly as it is described in the video lectures.
Recall from the lectures that, just before the main Partition subroutine, you
should exchange the pivot element (i.e., the last element) with the first 
element.

(3)Compute the number of comparisons (as in Problem 1), using the 
"median-of-three" pivot rule. [The primary motivation behind this rule is to do
a little bit of extra work to get much better performance on input arrays that
are nearly sorted or reverse sorted.] In more detail, you should choose the
pivot as follows. Consider the first, middle, and final elements of the given
array. (If the array has odd length it should be clear what the "middle" 
element is; for an array with even length 2k, use the kth element as the
"middle" element. So for the array 4 5 6 7, the "middle" element is the second
one ---- 5 and not 6!) Identify which of these three elements is the median
(i.e., the one whose value is in between the other two), and use this as your
pivot. As discussed in the first and second parts of this programming 
assignment, be sure to implement Partition exactly as described in the video
lectures (including exchanging the pivot element with the first element just
before the main Partition subroutine).

EXAMPLE: For the input array 8 2 4 5 7 1 you would consider the first (8), 
middle (4), and last (1) elements; since 4 is the median of the set {1,4,8}, 
you would use 4 as your pivot element.

SUBTLE POINT: A careful analysis would keep track of the comparisons made in
identifying the median of the three candidate elements. You should NOT do this.
That is, as in the previous two problems, you should simply add m−1 to your 
running total of comparisons every time you recurse on a subarray with length m.

*/

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

void FirstPivot(vector<int>& array, int &left, int &right){}

void FinalPivot(vector<int>& array, int &left, int &right){
    swap(array[left], array[right]);
}

void MedianPivot(vector<int>& array, int &left, int &right){
    int middle = (left + right) >> 1;

    if ((array[middle] - array[right]) * (array[left]-array[right]) < 0)
        swap(array[left], array[right]);
    else if((array[left] - array[middle]) * (array[right] - array[middle]) < 0)
        swap(array[left], array[middle]);
}

int Partition(vector<int>& array, void (*f)(vector<int>&, int&, int&), int &left, int &right) {
    f(array, left, right);

    int pivot = array[left];

    //i indicates the end of the smaller part, j indicates the end of the larger part;
    int i = left + 1;
    for (int j = left + 1; j <= right; j ++) {
        if (array[j] < pivot)
            swap(array[i ++], array[j]);
    }
    swap(array[left], array[i - 1]);
  
    return i - 1;
}

int QuickSort(vector<int> array, void (*f)(vector<int>&, int&, int&), int left, int right) {
    if (left >= right)
        return 0;

    int PivotIndex = Partition(array, f, left,right);
    int leftComp = QuickSort(array, f, left, PivotIndex - 1);
    int rightComp = QuickSort(array, f, PivotIndex + 1, right);
    return leftComp + rightComp + (right - left);
}

int main()
{
    ifstream file;
    string line;
    vector<int> arr;

    file.open("QuickSort.txt");
    if (file.is_open()) {
    	while (getline(file,line))
            arr.push_back(stoi(line));

        file.close();

        void (*f)(vector<int>&, int&, int&) = FirstPivot;
        cout<<"first element as pivot: "<< QuickSort(arr, f, 0, arr.size() - 1) <<endl;

        f = FinalPivot;
        cout << "final element as pivot: " << QuickSort(arr, f, 0, arr.size() - 1) << endl;

        f = MedianPivot;
        cout << "median element as pivot: " << QuickSort(arr, f, 0, arr.size() - 1) << endl;

    }
}