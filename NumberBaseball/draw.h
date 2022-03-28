#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class draw
{
private:
	int size;
	bool *select;
	int *arr;
	vector<int> V;
	vector<vector<int>> storage;
public:
	~draw();
	draw(int arr[], int size);
	void combination(int n, int r, int Idx = 0, int Cnt = 0);
	void permutation(int n, int r, int Cnt = 0);
	int countsize();
	void reset();
	void print();
	void outlet(int a[], int n, int r);
};