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
#include <cstring>
#include <cmath>
using namespace std;

const int maxn = 1002;
const double MAXDIS = 10000000;

double X[maxn], Y[maxn];
bool visit[maxn]; //记录某点是否访问过，即是否已加入到最小生成树中

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
	visit[1] = 1;

	int k = 0;

	for(int i = 1; i < N; ++i) {
		double min = MAXDIS;
		for(int j = 1; j <= N; ++j) {
			if(!visit[j] && lowcost[j] < min) {
				min = lowcost[j];
				k = j;
			}
		}
		if(min == MAXDIS) // 找不到新的点可以加入到树中
			break;

		visit[k] = 1; // k 已被访问过
		ans += min;

		for(int j = 1; j <= N; ++j) {
			if(!visit[j] && cost[k][j] < lowcost[j]) {
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

		memset(visit, 0, sizeof(visit));

		for(int i = 1; i <= N; ++i) {
			scanf("%lf%lf", &X[i], &Y[i]);
		}

		get_all_cost(X, Y, N);

		for(int i = 0; i < M; ++i) {
			scanf("%d%d", &x, &y);
			cost[x][y] = 0;
			cost[y][x] = 0;
		}

		ans = prim(N);

		printf("%.2lf\n", ans);

	}

	return 0;
}