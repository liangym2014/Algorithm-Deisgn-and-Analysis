// huffman code.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
/*
In this programming problem and the next you'll code up the greedy algorithm from the lectures on Huffman coding.

This file describes an instance of the problem. It has the following format:

[number_of_symbols]

[weight of symbol #1]

[weight of symbol #2]

...

For example, the third line of the file is "6852892," indicating that the weight of the second symbol of the alphabet is 6852892. 
 (We're using weights instead of frequencies, like in the "A More Complex Example" video.)

Question 1
Your task in this problem is to run the Huffman coding algorithm from lecture on this data set. What is the maximum length of a codeword in the resulting Huffman code?

ADVICE: If you're not getting the correct answer, try debugging your algorithm using some small test cases. And then post them to the discussion forum!

Question 2
Continuing the previous problem, what is the minimum length of a codeword in your Huffman code?
*/
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
using namespace std;

bool cmp(const tuple<int, int, int>& l, const tuple<int, int, int>& r) {
    return get<0>(l) > get<0>(r);
}

void data_input(vector<tuple<int,int, int>>& ws, string& filename) {
    ifstream f;
    f.open(filename);

    if (f.is_open()) {
        string element;
        f >> element;

        while (!f.eof()) {
            f >> element;
            ws.push_back(make_tuple(stoi(element), 0, 0));
        }
        f.close();
    }
}

tuple<int, int, int> merge(tuple<int, int, int>& l, tuple<int, int, int>& r) {
    return make_tuple(get<0>(l) + get<0>(r), max(get<1>(l), get<1>(r)) + 1, min(get<2>(l), get<2>(r)) + 1);
}

int main()
{
    vector<tuple<int, int, int>> weights;  //{weight, max path from current position to any leaf, min path from current position to any leaf}
    vector<string> files{ "huffman.txt","test1.txt","test2.txt" };
    data_input(weights, files[0]);

    make_heap(weights.begin(), weights.end(), cmp); //min-heap

    while (weights.size() > 1) {
        tuple<int, int, int> e1 = weights.front();
        pop_heap(weights.begin(), weights.end(), cmp);
        weights.pop_back();
        //cout << "e1: " << get<0>(e1);

        tuple<int, int, int> e2 = weights.front();
        pop_heap(weights.begin(), weights.end(), cmp);
        weights.pop_back();
        //cout << ", e2: " << get<0>(e2)<<endl;

        weights.push_back(merge(e1, e2));
        push_heap(weights.begin(), weights.end(), cmp);
    }
    cout << "max path:" << get<1>(weights[0]) << "\nmin path:" << get<2>(weights[0]) << endl;
}
