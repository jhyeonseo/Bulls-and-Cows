#include "node.h"
#include"draw.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>

node::node(int* answer, int* outrange, int inrange_size)
{
	this->answer = answer;
	this->outrange = outrange;
	this->inrange_size = inrange_size;
}
void node::arrmaker(node& object, int strike, int ball, int out)
{
	int index[length];
	for (int i = 0; i < length; i++)
		index[i] = i;

	int* sindex = new int[strike];
	int* bindex = new int[ball];
	int* oindex = new int[out];

	int* except_oindex = new int[length - out];
	int* except_sindex = new int[length - strike];
	int* blocation = new int[ball];


	int new_possible_countsize = combination_count(length, out) * combination_count(length - out, strike) * floating_count(length - strike, ball);
	int** new_possible = new int* [new_possible_countsize];
	for (int i = 0; i < new_possible_countsize; i++)
		new_possible[i] = new int[length];       //신규 가능집합 공간 할당

	draw ostorage(index, length);
	ostorage.combination(length, out);
	int ostorage_countsize = ostorage.countsize();
	int* new_indicatorindex = new int[ostorage_countsize + 1];    //신규 out지표 인덱스 공간 할당
	int** new_indicator = new int* [ostorage_countsize];
	for (int i = 0; i < ostorage_countsize; i++)
		new_indicator[i] = new int[out];             //신규 out지표 공간 할당

	int count = 0;
	for (int i = 0; i < ostorage_countsize; i++) {
		ostorage.outlet(oindex, out, i);

		for (int k = 0; k < out; k++)
			new_indicator[i][k] = answer[oindex[k]];    //신규 out지표에 뽑은 out 저장
		new_indicatorindex[i] = count;    //신규 out지표 인덱스에 현재 count 저장 (현재 count = 현재 시작지점)

		int help_except_oindex = 0;
		int help_except_oindex2 = 0;

		for (int k = 0; k < length; k++)
		{
			if (oindex[help_except_oindex] == k)
				help_except_oindex++;

			else
			{
				except_oindex[help_except_oindex2] = k;
				help_except_oindex2++;
			}
		}                    //out 인덱스를 제외한 나머지 인덱스로 except_oindex 배열 생성


		draw sstorage(except_oindex, length - out);
		sstorage.combination(length - out, strike);



		int sstorage_countsize = sstorage.countsize();
		for (int ii = 0; ii < sstorage_countsize; ii++) {
			sstorage.outlet(sindex, strike, ii);

			int help_except_oindex3 = 0;
			int help_except_sindex = 0;
			int help_bindex = 0;
			int help_sindex = 0;

			for (int k = 0; k < length; k++)
			{
				if (except_oindex[help_except_oindex3] == k)
				{
					if (sindex[help_sindex] != k)
					{
						bindex[help_bindex] = k;
						help_bindex++;
						except_sindex[help_except_sindex] = k;
						help_except_sindex++;
					}
					else
						help_sindex++;
					help_except_oindex3++;
				}
				else
				{
					except_sindex[help_except_sindex] = k;
					help_except_sindex++;
				}
			}

			draw bstorage(except_sindex, ball + out);
			bstorage.permutation(ball + out, ball);
			int bstorage_countsize = bstorage.countsize();

			for (int iii = 0; iii < bstorage_countsize; iii++) {
				bstorage.outlet(blocation, ball, iii);

				bool check_overlap = false;
				for (int k = 0; k < ball; k++)
					if (blocation[k] == bindex[k])
					{
						check_overlap = true;
						break;
					}
				if (check_overlap)
					continue;                                   //bindex가 들어갈 자리 검사

				for (int k = 0; k < length; k++)
				{
					new_possible[count][k] = -1;  //out 저장
				}

				for (int k = 0; k < strike; k++)
				{
					new_possible[count][sindex[k]] = answer[sindex[k]];  //strike 저장		
				}

				for (int k = 0; k < ball; k++)
				{
					new_possible[count][blocation[k]] = answer[bindex[k]];  //ball 저장
				}

				count++;
			}
		}
	}
	new_indicatorindex[ostorage_countsize] = count;  //마지막 위치 저장     
	//cout << "count=" << count << endl;
	delete[]sindex;
	delete[]bindex;
	delete[]oindex;
	delete[]except_sindex;
	delete[]except_oindex;
	delete[]blocation;

	//////////////////////////////////
	int indicator_countsize = 0;
	int possible_countsize = 0;
	indicatorindex = new int[object.possible.size() * new_possible_countsize + 1];  //현재 노드에서의 indicatorindex 공간할당
	for (int i = 0; i < object.indicator.size(); i++)
	{
		for (int ii = 0; ii < ostorage_countsize; ii++)
		{
			vector<int>indicator_helper;  //indicator 순서대로 정렬
			sort(new_indicator[ii], new_indicator[ii] + out);
			sort(object.indicator[i].begin(), object.indicator[i].end());
			set_union(new_indicator[ii], new_indicator[ii] + out, object.indicator[i].begin(), object.indicator[i].end(), back_inserter(indicator_helper));
			
			if (indicator_helper.size() > range - length)  // length-(inrangesize-indicatorsize) should be <= outrangesize
				continue;

			bool indicator_overlap = false;
			for (int k = 0; k < indicator_helper.size(); k++)
			{
				for (int kk = 0; kk < length; kk++)
				{
					if (indicator_helper[k] == new_possible[new_indicatorindex[ii]][kk])
					{
						indicator_overlap = true;
						break;
					}
					else if (indicator_helper[k] == object.possible[object.indicatorindex[i]][kk])
					{
						indicator_overlap = true;
						break;
					}
				}
				if (indicator_overlap)
					break;
			}
			if (indicator_overlap)
			{
				continue;
			}

			indicatorindex[indicator_countsize] = possible_countsize;
			//아래:벡터 집합 합성하기, 위:아웃 지표 합성하기 

			bool test_noempty = false;

			for (int k = object.indicatorindex[i]; k < object.indicatorindex[i + 1]; k++)
			{
				for (int kk = new_indicatorindex[ii]; kk < new_indicatorindex[ii + 1]; kk++)
				{
					bool test_possible = true;
					int* possible_helparray = new int[length];
					for (int p = 0; p < length; p++)
					{
						if (((object.possible[k])[p] == -1))
						{
							possible_helparray[p] = new_possible[kk][p];
							continue;
						}
						else if (new_possible[kk][p] == -1)
						{
							possible_helparray[p] = object.possible[k][p];
							continue;
						}
						else if (object.possible[k][p] == new_possible[kk][p])
						{
							possible_helparray[p] = new_possible[kk][p];
							continue;
						}
						else
						{
							test_possible = false;
							break;
						}
					}
					for (int a = 0; a < length - 1; a++)
						if (possible_helparray[a] != -1)
							for (int aa = a + 1; aa < length; aa++)
								if (possible_helparray[aa] == possible_helparray[a])
								{
									test_possible = false;
								} //중복확인
					if (test_possible == true)
					{					
						possible.push_back(possible_helparray);
						possible_countsize++;
						test_noempty = true;
					}
					else
						delete[]possible_helparray;
				}
			}
			if (test_noempty)
			{
				indicator.push_back(indicator_helper);
				indicator_countsize++;
			}
		}
	}

	indicatorindex[indicator_countsize] = possible_countsize;
	//////////////////////////////////////////

	for (int i = 0; i < new_possible_countsize; i++)
		delete[]new_possible[i];
	delete[]new_possible;

	for (int i = 0; i < ostorage_countsize; i++)
		delete[]new_indicator[i];
	delete[]new_indicator;

	delete[]new_indicatorindex;

}
void node::arrmaker(int strike, int ball, int out)
{
	int index[length];

	for (int i = 0; i < length; i++)
		index[i] = i;

	int* sindex = new int[strike];
	int* bindex = new int[ball];
	int* oindex = new int[out];

	int* except_oindex = new int[length - out];
	int* except_sindex = new int[length - strike];
	int* blocation = new int[ball];



	int new_possible_countsize = combination_count(length, out) * combination_count(length - out, strike) * floating_count(length - strike, ball);
	int** new_possible = new int* [new_possible_countsize];
	for (int i = 0; i < new_possible_countsize; i++)
		new_possible[i] = new int[length];       //신규 가능집합 공간 할당

	draw ostorage(index, length);
	ostorage.combination(length, out);
	int ostorage_countsize = ostorage.countsize();
	indicatorindex = new int[ostorage_countsize + 1];    //신규 out지표 인덱스 공간 할당
	int** new_indicator = new int* [ostorage_countsize];
	for (int i = 0; i < ostorage_countsize; i++)
		new_indicator[i] = new int[out];             //신규 out지표 공간 할당

	int count = 0;
	for (int i = 0; i < ostorage_countsize; i++) {
		ostorage.outlet(oindex, out, i);

		for (int k = 0; k < out; k++)
			new_indicator[i][k] = answer[oindex[k]];    //신규 out지표에 뽑은 out 저장

		indicatorindex[i] = count;    //신규 out지표 인덱스에 현재 count 저장

		int help_except_oindex = 0;
		int help_except_oindex2 = 0;
		for (int k = 0; k < length; k++)
		{
			if (oindex[help_except_oindex] == k)
				help_except_oindex++;

			else
			{
				except_oindex[help_except_oindex2] = k;
				help_except_oindex2++;
			}
		}                    //out 인덱스를 제외한 나머지 인덱스로 except_oindex 배열 생성

		draw sstorage(except_oindex, length - out);
		sstorage.combination(length - out, strike);

		int sstorage_countsize = sstorage.countsize();
		for (int ii = 0; ii < sstorage_countsize; ii++) {
			sstorage.outlet(sindex, strike, ii);

			int help_except_oindex3 = 0;
			int help_except_sindex = 0;
			int help_bindex = 0;
			int help_sindex = 0;

			for (int k = 0; k < length; k++)

			{

				if (except_oindex[help_except_oindex3] == k)

				{

					if (sindex[help_sindex] != k)
					{
						bindex[help_bindex] = k;
						help_bindex++;
						except_sindex[help_except_sindex] = k;
						help_except_sindex++;
					}
					else
						help_sindex++;

					help_except_oindex3++;
				}
				else
				{
					except_sindex[help_except_sindex] = k;
					help_except_sindex++;
				}
			}

			draw bstorage(except_sindex, ball + out);
			bstorage.permutation(ball + out, ball);
			int bstorage_countsize = bstorage.countsize();

			for (int iii = 0; iii < bstorage_countsize; iii++) {
				bstorage.outlet(blocation, ball, iii);

				int check_overlap = 0;
				for (int k = 0; k < ball; k++)
					if (blocation[k] == bindex[k])
					{
						check_overlap = 1;
						break;
					}
				if (check_overlap == 1)
					continue;                                   //bindex가 들어갈 자리 검사

				for (int k = 0; k < length; k++)
					new_possible[count][k] = -1;  //out 저장

				for (int k = 0; k < strike; k++)
					new_possible[count][sindex[k]] = answer[sindex[k]];  //strike 저장		

				for (int k = 0; k < ball; k++)
					new_possible[count][blocation[k]] = answer[bindex[k]];  //ball 저장

				count++;
			}
		}
	}

	indicatorindex[ostorage_countsize] = count;  //마지막 위치 저장 


	for (int i = 0; i < count; i++)
	{
		int* possible_helparray = new int[length];
		for (int p = 0; p < length; p++)
			possible_helparray[p] = new_possible[i][p];

		possible.push_back(possible_helparray);
	}


	for (int i = 0; i < ostorage_countsize; i++)
	{
		vector<int>indicator_helper;
		for (int p = 0; p < out; p++)
		{
			indicator_helper.push_back(new_indicator[i][p]);
		}
		indicator.push_back(indicator_helper);
	}



	delete[]sindex;
	delete[]bindex;
	delete[]oindex;
	delete[]except_sindex;
	delete[]except_oindex;
	delete[]blocation;

	for (int i = 0; i < ostorage_countsize; i++)
		delete[]new_indicator[i];
	delete[]new_indicator;

	for (int i = 0; i < count; i++)
		delete[]new_possible[i];
	delete[]new_possible;

}
int node::combination_count(int n, int r)
{
	if (n == 0)
		return 1;   //4out일때 벡터공간 한개 할당을 위해서
	if (n == r)
		return 1;  //계산의 시간단축을 위해서
	if (n < r)
		return 0;  //불가능한 경우, 새로추가됨


	int x = 1;
	int y = 1;
	for (int i = 0; i < r; i++)
	{
		x = x * (n - i);
	}
	for (int i = 1; i <= r; i++)
	{
		y = y * i;
	}

	return x / y;
}
int node::permutation_count(int n, int r)
{	
	if (n < r)
		return 0;  //불가능한 경우, 새로추가됨

	int x = 1;
	for (int i = 0; i < r; i++)
		x = x * (n - i);

	return x;
}
int node::floating_count(int n, int r)
{
	switch (r)
	{
	case 0:
		return 1;
	case 1:
		return n - 1;
	case 2:
		return n * n - 3 * n + 3;
	case 3:
		return n * n * n - 6 * n * n + 14 * n - 13;
	case 4:
		return n * n * n * n - 10 * n * n * n + 41 * n * n - 84 * n + 73;
	}

	cout << endl << "nFr 함수의 미완성 구간입니다!" << endl;
	cout << "n=" << n << endl;
	cout << "r=" << r << endl;
	return 0;
}
void node::treemaker()
{
	int outrange_size = range - inrange_size;
	int* inrange = new int[inrange_size];
	int now_totalpossible = this->totalpossible();

	int help_put_inrange = 0;
	for (int i = 0; i < outrange_size; i++)
	{
		int before;
		if (i == 0)
			before = 1;
		else
			before = outrange[i-1]+1;

		int after = outrange[i];

			for (int k = before; k < after; k++)
			{
				inrange[help_put_inrange] = k; // 수리요망
				help_put_inrange++;
			}
	}


	if (help_put_inrange != inrange_size)
	{
		if (inrange_size == range)
			for (int i = 0; i < range; i++)
				inrange[i] = i + 1;
		else
		{
			for (int i = 0; i < inrange_size - help_put_inrange; i++)
			{
				inrange[inrange_size - 1 - i] = range-i;
			}
			for (int i = 0; i < inrange_size; i++)
				cout << inrange[i] << " ";
			cout << endl;
			//return;
		}
	}

	int* index_make_helper = new int[length];
	for (int i = 0; i < length; i++)
		index_make_helper[i] = i;

	draw index(index_make_helper, length);
	draw pick(inrange, inrange_size);

	int start, end;
	if (outrange_size < length)
		start = length - outrange_size;
	else
		start = 0;
	if (inrange_size < length)
		end = inrange_size;
	else
		end = length;
	/////////////// 사전 설정 ////////////////

	int pick_countsize=0;
	int index_countsize=0;
	for (int i = start; i <= end; i++)
	{
		pick_countsize += combination_count(inrange_size, i);
		index_countsize += permutation_count(length, i);
	}


	int max_point2 = 0;
	int temp_point;
	//cout << "start=" << start << endl;
	//cout << "end=" << end << endl;
	for (int i = start; i <= end; i++)
	{
		index.permutation(length, i);
		pick.combination(inrange_size, i);
		int lmi = length - i;      //length - i
		int temp_inrange_size = inrange_size + lmi;
		
		int* answer_index = new int[i];  
		int* answer_pick = new int[i];	 

		for (int ii = 0; ii < pick.countsize(); ii++)
		{
			pick.outlet(answer_pick, i, ii);
			

			for (int iii = 0; iii < index.countsize(); iii++)
			{
				index.outlet(answer_index, i, iii);
				int semipoint[15][3];   // s o p , 15 = range=4 에서의 최대경우
				semipoint[0][0] = -1;
				temp_point = 0;
				int total_max_point = 0;

				int temp_answer[length] = { 0, };
				int insert_helper = 0;
		
				for (int k = 0; k < i; k++)
					temp_answer[answer_index[k]] = answer_pick[k];
				for (int k = 0; k < length; k++)
				{
					if (temp_answer[k] == 0)
					{
						temp_answer[k] = outrange[insert_helper];
						insert_helper++;
					}
				}

				for (int a = 0; a < indicator.size(); a++)
				{
					int blank = length - inrange_size + indicator[a].size();
					int outrange_pick = length - i;
					int k = 0;
					int hit = 0;                         // possible과 answer에 동시에 등장하는 등장집합의 숫자
					int helper[length];
					int temp_out = 0;
					int temp_strike = 0;
					int fullpick_point;
					if (i == length)
						fullpick_point = permutation_count(outrange_size, blank);
					
					for (int t = 0; t < i; t++)
					{
						bool check = true;
						for(int tt=0;tt<indicator[a].size();tt++)
							if (answer_pick[t] == indicator[a][tt])
							{
								temp_out++;
								check = false;
								break;
							}
						if (check)
						{
							helper[hit] = t;
							hit++;
						}
					}

					for (int b = indicatorindex[a]; b < indicatorindex[a + 1]; b++)
					{
						temp_strike = 0;
						int overlap=blank;

						for (int c = 0; c < hit; c++)
							if (possible[b][answer_index[helper[c]]] == answer_pick[helper[c]])
								temp_strike++;
					
						///////////////////////
						///////////////////////
						for (int c = 0; c < i; c++)	
							if (possible[b][answer_index[c]] == -1)
								overlap--;

						int max_point = 0;
						int plus_temp_strike = 0;
						int plus_temp_out = 0;
						
						for (int n = 0; n <= overlap; n++)
						{
							int d = combination_count(overlap, n) * permutation_count(outrange_size - n, blank - n);
							if (max_point < d)
							{
								max_point = d;
								plus_temp_strike = n;
							}
						}
						
						temp_strike = temp_strike + plus_temp_strike;
						max_point = 0;

						///////////////////////////////////////////////////////// 10 - 13 수리완료
						int min;
						if (blank < lmi)
							min = lmi-blank;
						else
							min = 0;

						int remainder = blank - plus_temp_strike;
						for (int n = min; n <= lmi - plus_temp_strike; n++)
						{
							int d=0;
							int e = lmi - plus_temp_strike - n;  // plus_temp_ball

							if(lmi-n<overlap)
								for (int nn = 0; nn <= e; nn++)
								{
									d+=combination_count(overlap - plus_temp_strike, nn)* floating_count(remainder, nn)* combination_count(lmi - overlap, e - nn)* permutation_count(remainder - nn, e - nn)* permutation_count(outrange_size - lmi, remainder - e);
								}
							else
								for (int nn = 0; nn <= overlap - plus_temp_strike; nn++)
								{
									d+=combination_count(overlap - plus_temp_strike, nn)* floating_count(remainder, nn)* combination_count(lmi - overlap, e - nn)* permutation_count(remainder - nn, e - nn)* permutation_count(outrange_size - lmi, remainder - e);
								}
						

							if (max_point < d)
							{
								max_point = d;
								plus_temp_out = n;
							}
						}
						/////////////////////////////////////////////////////////
						/*
						if (0==0)
						{
							cout << "overlap=" << overlap << endl;
							cout << "pto=" << plus_temp_out << endl;
							cout << "pts=" << plus_temp_strike << endl;
							cout << "Strike=" << temp_strike << endl;
							cout << "Out=" << temp_out + plus_temp_out << endl;
						}
						
				        
						if (floating_count(length - temp_strike, length - temp_strike - (temp_out + plus_temp_out)) == 0)
						{
							cout << "**************************************************************" << endl;
							cout << "i=" << i << endl;
							cout << "hit=" << hit << endl;
							cout << "overlap=" << overlap << endl;
							cout << "pto=" << plus_temp_out << endl;
							cout << "pts=" << plus_temp_strike << endl;
							cout << "Strike=" << temp_strike << endl;
							cout << "Out=" << temp_out + plus_temp_out << endl;
							cout << "indicatorsize=" << indicator[a].size() << endl;
							cout << "k=" << k << endl;
							cout << "maxpoint=" << max_point << endl;
						}
                        */
						

						if (temp_out + plus_temp_out == 4 || floating_count(length - temp_strike, length - temp_strike - (temp_out + plus_temp_out)) == 0)
							continue;  ////////////////////////// 개선 여지 있음

						if (i == length)
						{
							max_point = fullpick_point;
							total_max_point += max_point;
						}

						for (int n = 0; n < 15; n++)
						{
							if (semipoint[n][0] == -1)
							{
								semipoint[n][0] = temp_strike;
								semipoint[n][1] = temp_out + plus_temp_out;
								semipoint[n][2] = max_point;
								total_max_point += max_point;
								if (n != 14)
									semipoint[n + 1][0] = -1;
							} 
							else if (semipoint[n][0] == temp_strike)
								if (semipoint[n][1] == temp_out + plus_temp_out)
								{
									semipoint[n][2] += max_point;
									total_max_point += max_point;
								}

						}
					}
				}		

				if (total_max_point == 0)
					continue;

				for (int n = 0; n < 15; n++)
				{
					if (semipoint[n][0] == -1)
						break;

					node child(temp_answer, outrange, temp_inrange_size);
					child.arrmaker(*this, semipoint[n][0], length - semipoint[n][0] - semipoint[n][1], semipoint[n][1]);

					if (semipoint[n][0] == length)
					{
						temp_point = 1000000 * semipoint[n][2];
						break;
					}
					else
					temp_point += (now_totalpossible-child.totalpossible()) * semipoint[n][2];
				}

				temp_point = temp_point / total_max_point;
					//for (int nn = 0; nn < length; nn++)
						//cout << temp_answer[nn] << " ";
					//cout << "temp_point=" << temp_point << endl;
				
				if (max_point2 < temp_point)
				{
					max_point2 = temp_point;
					for (int nn = 0; nn < length; nn++)
					{
						answer[nn] = temp_answer[nn];
						cout << answer[nn] << " ";
					}
					cout << endl;
					cout <<"max_point = " << max_point2 << endl;
				}
							
			}
		}
		pick.reset();
		index.reset();
		delete[] answer_index;
		delete[] answer_pick;
	}



	delete[] inrange;
	delete[] index_make_helper;
}
int node::totalpossible()
{
	int total = 0;
	int outrange_size = range - inrange_size;

	for (int i = 0; i < indicator.size(); i++)
	{
		int blank = length - inrange_size + indicator[i].size();
		total += permutation_count(outrange_size, blank) * (indicatorindex[i + 1] - indicatorindex[i]);
	}

	return total;
}
node::~node()
{
	//cout << stage << "번째의 노드 삭제" << endl;
	delete[]indicatorindex;

	for (int i = 0; i < possible.size(); i++)
		delete possible[i];

}
void node::printer()

{

	cout << "possible 출력" << endl;

	for (int i = 0; i < possible.size(); i++)

	{

		for (int k = 0; k < length; k++)

			cout << "[" << possible[i][k] << "]";



		cout << endl;

	}

	cout << endl;



	cout << "indicator/indicatorindex 출력" << endl;

	for (int i = 0; i < indicator.size(); i++)

	{

		for (int k = 0; k < indicator[i].size(); k++)

		{

			cout << "[" << indicator[i][k] << "]";

		}

		cout << "  (" << indicatorindex[i] << ")";

		cout << endl;

	}

	cout << "Last = " << indicatorindex[indicator.size()] << endl << endl;

}


