//
//  main.cpp
//  eoj1854
//
//  Created by whyisyoung on 3/8/14.
//  Copyright (c) 2014 whyisyoung. All rights reserved.
//

#include <iostream>
#include <string>
#include <cstring>
using namespace std;
const int maxn = 352;

bool check_all_same(string str)
{
	for(int i = 1; i < str.length(); ++i) {
		if(str[i] != str[i-1])
			return false;
	}
	return true;
}


int main()
{
	int n;
	int ans;
	string necklace;
	int temp_r[2*maxn], temp_b[2*maxn];
	int r[maxn], b[maxn];

	while(cin >> n) {
		cin >> necklace;
		string circle = necklace + necklace;
		int kr = 0, kb = 0;
		int cntr = 0, cntb = 0;

		memset(r, 0, sizeof(r));
		memset(b, 0, sizeof(b));

		string tr = necklace, tb = necklace;
		for(int i = 0; i < n; ++i) {
			if(necklace[i] == 'w') {
				tr[i] = 'r';
				tb[i] = 'b';
			}
		}


		if(check_all_same(tr) || check_all_same(tb)) {
			cout << n << endl;
			continue;
		}

		for(int i = 0; i < 2*n; ++i) {
			if(circle[i] == 'r' && circle[i+1] == 'b')
				temp_r[kr++] = i;
			else if(circle[i] == 'b' && circle[i+1] == 'r')
				temp_b[kb++] = i;
			else if(circle[i] == 'r' && circle[i+1] == 'w' && circle[i+2] == 'b') {
				temp_r[kr++] = i;
				temp_r[kr++] = i + 1;
			}
			else if(circle[i] == 'b' && circle[i+1] == 'w' && circle[i+2] == 'r') {
				temp_b[kb++] = i;
				temp_b[kb++] = i + 1;
			}
		}

		for(int i = 0; i < kr; ++i) {
			for(int j = temp_r[i]; j >= 0; --j) {
				if(circle[j] == 'r' || circle[j] == 'w')
					r[cntr]++;
				else
					break;
			}
			cntr++;
			for(int j = temp_r[i]+1; j < 2*n; ++j) {
				if(circle[j] == 'b' || circle[j] == 'w')
					b[cntb]++;
				else
					break;
			}
			cntb++;
		}



		for(int i = 0; i < kb; ++i) {
			for(int j = temp_b[i]; j >= 0; --j) {
				if(circle[j] == 'b' || circle[j] == 'w')
					b[cntb]++;
				else
					break;
			}
			cntb++;
			for(int j = temp_b[i]+1; j < 2*n; ++j) {
				if(circle[j] == 'r' || circle[j] == 'w')
					r[cntr]++;
				else
					break;
			}
			cntr++;
		}

		ans = r[0] + b[0];

		for(int i = 1; i < cntr; ++i) {
			if(ans < r[i] + b[i])
				ans = r[i] + b[i];
		}

		if(ans <= n)
            cout << ans << endl;
        else
            cout << n << endl;
	}

	return 0;
}
