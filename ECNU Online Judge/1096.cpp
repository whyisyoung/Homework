//
//  main.cpp
//  eoj1096
//
//  Created by whyisyoung on 3/16/14.
//  Copyright (c) 2014 whyisyoung. All rights reserved.
//

#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cmath>
using namespace std;


const int INF = 100000000;

int dp[15][10][10][10][10];
int sum[10][10]; // 从 （1，1）到 (i,j) 的分值和的平方

int score[10][10];

void init_sum()
{
	memset(sum, 0, sizeof(sum));

	for(int i = 1; i <= 8; ++i) {
		for(int j = 1; j <= 8; ++j) {
			for( int x = 1; x <= i; ++x) {
				for(int y = 1; y <= j; ++y) {
					sum[i][j] += score[x][y];
				}
			}
		}
	}
}

int partial_sum_square(int x1, int y1, int x2, int y2)
// 左上角为 (x1,y1), 右下角为 (x2,y2) 的矩形的分值和的平方
{
	int tmp = sum[x2][y2] - sum[x1-1][y2] - sum[x2][y1-1] + sum[x1-1][y1-1];
	return (tmp * tmp);
}

int solve(int k, int x1, int y1, int x2, int y2)
{
	if(dp[k][x1][y1][x2][y2] != -1)
		return dp[k][x1][y1][x2][y2];
	if(k == 0) {
		int tmp = partial_sum_square(x1,y1,x2,y2);
		return (dp[k][x1][y1][x2][y2] = tmp);
	}

	int ans = INF;

	for(int a = x1; a < x2; ++a) {   // 沿着横坐标切，即竖切
		int horizon_min = min(solve(k-1, x1, y1, a, y2) + partial_sum_square(a+1, y1, x2, y2), solve(k-1, a+1, y1, x2, y2) + partial_sum_square(x1, y1, a, y2));
		ans = min(ans, horizon_min);
	}

	for(int b = y1; b < y2; ++b) {   // 沿着纵坐标切，即横切
		int vertical_min = min(solve(k-1, x1, y1, x2, b) + partial_sum_square(x1, b+1, x2, y2), solve(k-1, x1, b+1, x2, y2) + partial_sum_square(x1, y1, x2, b));
		ans = min(ans, vertical_min);
	}
	return (dp[k][x1][y1][x2][y2] = ans);
}

int main()
{
	int n;
	while(~ scanf("%d", &n)) {
		int total_sum = 0;

		memset(dp, -1, sizeof(dp));

		for(int i = 1; i <= 8; ++i) {
			for(int j = 1; j <= 8; ++j) {
				scanf("%d", &score[i][j]);
				total_sum += score[i][j];
			}
		}

		init_sum();

		int x = solve(n-1, 1, 1, 8, 8);

		double ans = sqrt(1.0 * (x*n-total_sum * total_sum)/(n*n));
		printf("%.3lf\n", ans);

	}
	return 0;
}