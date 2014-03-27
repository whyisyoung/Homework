//
//  main.cpp
//  eoj1824
//
//  Created by whyisyoung on 3/26/14.
//  Copyright (c) 2014 whyisyoung. All rights reserved.
//

#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;

const int maxn = 102;

int a[maxn][maxn]; // the original tower

// d[i][j][k] 表示以 (i, j) 为根的子三角形的所有数之和的个位数若有为 k 的路径则取值为1， 否则取值为0（其中 k 为 0-9 ）
int d[maxn][maxn][10];

int solve(int n)
{
    memset(d, 0, sizeof(d));

    for(int i = 1; i <= n; ++i) {
    	d[n][i][a[n][i] % 10] = 1;
    }

    for(int i = n; i >= 1; --i) {
    	for(int j = 1; j <= i; ++j) {
    		for( int k = 0; k <= 9; ++k) {
    			if(d[i][j][k]) {
    				d[i - 1][j - 1][(k + a[i - 1][j - 1] % 10)] = 1;
    				d[i - 1][j][(k + a[i - 1][j]) % 10] = 1;
    			}
    		}
    	}
    }

    int ans = 0;
    for(int k = 1; k <= 9; ++k) {
    	if(d[1][1][k]) {
    		if(k > ans)
    			ans = k;
    	}
    }

    return ans;
}

int main()
{
    int C, N;
    scanf("%d", &C);
    while(C--) {
		scanf("%d", &N);

		memset(a, 0, sizeof(a));

		for(int i = 1; i <= N; ++i) {
		    for(int j = 1; j <= i; ++j) {
                scanf("%d", &a[i][j]);
		    }
		}

		cout << solve(N) << endl;
    }
    return 0;
}
