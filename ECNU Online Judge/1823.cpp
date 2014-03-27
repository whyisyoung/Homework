//
//  main.cpp
//  eoj1823
//
//  Created by whyisyoung on 3/22/14.
//  Copyright (c) 2014 whyisyoung. All rights reserved.
//

#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;

const int maxn = 502;

int a[maxn][maxn]; // the original tower
int d[maxn][maxn]; // d[i][j] 表示不考虑其中一步可以随意走的情况下从第一行走到 a[i][j]的最大和
int f[maxn][maxn]; // f[i][j] 表示其中一步可以随意走的情况下从第一行走到 a[i][j]的最大和

int solve(int n)
{
    for(int i = 1; i <= n; ++i)
        f[n][i] = d[n][i] = a[n][i];

    for(int i = n - 1; i >= 1; --i) {
		int tmp = d[i+1][i+1];
		for(int j = 1; j <= i; ++j)
		    tmp = (d[i+1][j] > tmp ? d[i+1][j] : tmp);

		for(int j = 1; j <= i; ++j) {
		    d[i][j] = a[i][j] + max(d[i+1][j], d[i+1][j+1]);
		    f[i][j] = a[i][j] + max(f[i+1][j], max(f[i+1][j+1], tmp));
		}
    }

    return f[1][1];
}

int main()
{
    int C, N;
    scanf("%d", &C);
    while(C--) {
		scanf("%d", &N);
		for(int i = 1; i <= N; ++i) {
		    for(int j = 1; j <= i; ++j) {
                scanf("%d", &a[i][j]);
		    }
		}

		cout << solve(N) << endl;
    }
    return 0;
}
