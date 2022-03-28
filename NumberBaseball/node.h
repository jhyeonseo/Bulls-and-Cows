#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#define length 4
#define range 10           //1~부터 n까지의 숫자의 갯수=range
using namespace std;
class node
{
private:
	vector<int*>possible; //가능집합
	vector<vector<int>>indicator;//아웃지표
	int *indicatorindex;     //아웃지표 인덱스
	int *answer;  //정답집합
	int *outrange; //비등장집합
	int inrange_size;  //등장원소개수
	int combination_count(int n, int r);   // 조합 개수 구하기
	int permutation_count(int n, int r);   // 순열 개수 구하기
	int floating_count(int n, int r);      // 특수순열
	int totalpossible();
	node(int* answer, int* outrange, int inrange_size, int stage);    //초기화 (뿌리노드)
public:
	void printer();
	void arrmaker(node &object, int strike, int ball, int out);      //가능집합생성함수  (탐색 완료후 다음노드)
	void arrmaker(int strike, int ball, int out);  //가능집합생성함수      (게임시작)
	void treemaker();                      // 트리 생성기
	node(int *answer, int *outrange, int inrange_size);    //초기화 
	~node();

};
