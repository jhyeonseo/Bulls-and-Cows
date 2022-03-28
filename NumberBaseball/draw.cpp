#include "draw.h"

draw::draw(int arr[], int size) {
	this->size = size;
	this->select = new bool[size];
	this->arr = new int[size];
	copy(arr, arr + size, this->arr);
	for (int i = 0; i < size; i++)
		select[i] = false;
	/*
	for (int i = 0; i < size; i++)
	   printf("[%d] ",this->arr[i]);
	cout << endl;
	*/
}
draw::~draw() {
	delete[]arr;
	delete[]select;
}
void draw::permutation(int n, int r, int Cnt)
{
	if (Cnt == r)
	{
		vector<int> mini;
		for (int i = 0; i < V.size(); i++)
		{
			mini.push_back(V[i]);
		}
		storage.push_back(mini);
		//mini.clear();
		//cout << "[" << mini.capacity() << "]" << endl;
		//mini.shrink_to_fit();
		return;
	}

	for (int i = 0; i < n; i++)
	{
		if (select[i] == true) continue;
		select[i] = true;
		V.push_back(arr[i]);
		permutation(n, r, Cnt + 1);
		V.pop_back();
		select[i] = false;
	}
}
void draw::combination(int n, int r, int Idx, int Cnt)
{
	if (Cnt == r)
	{
		vector<int> mini;
		for (int i = 0; i < n; i++)
		{
			if (select[i] == true)
			{
				mini.push_back(arr[i]);
			}
		}
		storage.push_back(mini);
		//mini.clear();
		//cout <<"["<<mini.capacity() << "]" << endl;
		//mini.shrink_to_fit();
		return;
	}
	for (int i = Idx; i < n; i++)
	{
		if (select[i] == true) continue;
		select[i] = true;
		combination(n, r, i, Cnt + 1);
		select[i] = false;
	}

}
void draw::print()
{
	cout << "벡터 전체의 크기=" << storage.size() << endl;
	for (int i = 0; i < storage.size(); i++)
	{
		for (int ii = 0; ii < storage[i].size(); ii++)
		{
			cout << storage[i][ii] << " ";
		}
		cout << endl;
	}
}
void draw::outlet(int a[], int n, int r) //a집합에 storage의 r번째 집합을 복사
{
	if (r >= storage.size())
	{
		cout << "[" << r << "] 인덱스에는 집합이 저장되어 있지 않습니다" << endl;
		return;
	}

	for (int i = 0; i < n; i++)
		a[i] = storage[r][i];
}
void draw::reset()
{
	storage.clear();
	storage.shrink_to_fit();
}
int draw::countsize()
{
	return storage.size();
}
