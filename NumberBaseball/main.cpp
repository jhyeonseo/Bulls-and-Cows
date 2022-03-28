#include"node.h"
#include"draw.h"
#include <iostream>
#include<ctime>
#include<cstdlib>
#include<crtdbg.h>

using namespace std;

int main()
{
	time_t start, end;
	//_CrtSetBreakAlloc(225);
	srand(time(NULL));
	int answer[length];

	for (int i = 0; i < length;)
	{
		bool test = true;
		int tmp = rand() % range + 1;
		for (int p = 0; p < length; p++)
		{
			if (answer[p] == tmp)
			{
				test = false;
				break;
			}
		}
		if (tmp == range)
			test = false;

		if (test)
		{
			answer[i] = tmp;
			i++;
		}

	}

	int outrange[range];
	bool outrange_bool[range];
	for (int i = 0; i < range; i++)
		outrange_bool[i] = true;
	int inrange_size;
	int strike; int ball; int out;
	int try_count = 0;
	node* storage[30];

	/*
	int answer2[length] = { 4,3,2,1 }; int answer3[length] = { 2,9,8,3 };
	int outrange2[range] = { 5,6,7,8,9,10,-1,-1,-1,-1 }; int outrange3[range] = { 5,6,7,10,-1,-1,-1,-1,-1,-1 };
	int inrange_size2 = 4; int inrange_size3 = 6;
	node a(answer2, outrange2, inrange_size2);
	a.arrmaker(0, 2, 2);
	node b(answer3, outrange3, inrange_size3);
	b.arrmaker(a, 1, 1, 2);
	*/

	do
	{
		if (try_count == 15)
		{
			cout << "정답 검출 실패!" << endl;
			break;
		}
		for (int i = 0; i < length; i++)
		{
			cout << answer[i] << " ";
			outrange_bool[answer[i] - 1] = false;
		}
		cout << endl;
		int push = 0;
		for (int i = 0; i < range; i++)
		{
			if (outrange_bool[i])
			{
				outrange[push] = i + 1;
				push++;
			}
		}

		inrange_size = range - push;

		cout << "strike = ";
		cin >> strike;
		if (strike == length)
		{
			cout << try_count + 1 << "번의 시도로 정답 검출" << endl;
			break;
		}
		cout << "ball = ";
		cin >> ball;
		out = length - strike - ball;
		cout << "out =" << out << endl;

		if (try_count == 0)
		{
			start = clock();
			node* parents = new node(answer, outrange, inrange_size);
			parents->arrmaker(strike, ball, out);
			//parents->printer();
			parents->treemaker();
			storage[try_count] = parents;
		}
		else
		{
			start = clock();
			node* parents = new node(answer, outrange, inrange_size);
			parents->arrmaker(*(storage[try_count - 1]), strike, ball, out);
			//parents->printer();
			parents->treemaker();
			storage[try_count] = parents;
			end = clock();
			cout << "실행시간 = " << (double)(end - start) / 1000 << endl;
		}

		try_count++;

	} while (true);

	for (int i = 0; i < try_count; i++)
	{
		delete storage[i];
	}

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
}