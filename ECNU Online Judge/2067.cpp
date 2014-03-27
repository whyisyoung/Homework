//
//  main.cpp
//  eoj2067
//
//  Created by whyisyoung on 3/15/14.
//  Copyright (c) 2014 whyisyoung. All rights reserved.
//

#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cmath>
using namespace std;

const int maxn = 1002;
const double MAXDIS = 10000000; // 这里写成 int 真是 WTF!!!

double X[maxn], Y[maxn];

double cost[maxn][maxn];
double lowcost[maxn];
double ans = 0;

void get_all_cost(double X[], double Y[], int N)
{
	for(int i = 1; i <= N; ++i) {
		for(int j = 1; j <= N; ++j) {
			if(i == j)
				cost[i][j] = 0;
			else
				cost[i][j] = sqrt((double)(X[i]-X[j])*(X[i]-X[j])+(double)(Y[i]-Y[j])*(Y[i]-Y[j]));
		}
	}
}

double prim(int N)
{
	ans = 0;

    for(int i = 1; i <= N; ++i) {
		lowcost[i] = cost[1][i];
	}

	int k = 0;

	for(int i = 1; i < N; ++i) {
		double min = MAXDIS; // 这里写成 int 真是 	WTF!!!
		for(int j = 1; j <= N; ++j) {
			if(lowcost[j] != 0 && lowcost[j] < min) {
				min = lowcost[j];
				k = j;
			}
		}
		if(min == MAXDIS) // 找不到新的点可以加入到树中
			break;

		if(min != -1)
			ans += min;

        lowcost[k] = 0; // 将顶点 k 加入到最小生成树中

		for(int j = 1; j <= N; ++j) {
			if(lowcost[j] != 0 && cost[k][j] < lowcost[j]) {
				lowcost[j] = cost[k][j];
			}
		}
	}

	return ans;
}


int main()
{
	int N, M;
	int x, y;
	while(scanf("%d%d", &N, &M) != EOF) {
		for(int i = 1; i <= N; ++i) {
			scanf("%lf%lf", &X[i], &Y[i]);
		}

		get_all_cost(X, Y, N);

		for(int i = 0; i < M; ++i) {
			scanf("%d%d", &x, &y);
			cost[x][y] = -1;
			cost[y][x] = -1;
		}

		ans = prim(N);

		printf("%.2lf\n", ans);

	}

	return 0;
}