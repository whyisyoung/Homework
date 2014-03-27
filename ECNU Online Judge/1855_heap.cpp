//
//  main.cpp
//  eoj1855_heap
//
//  Created by whyisyoung on 3/17/14.
//  Copyright (c) 2014 whyisyoung. All rights reserved.
//

#include <iostream>
#include <cstdio>
#include <algorithm>
using namespace std;

const int maxn = 10005;

int N, L, P;

struct stop {
	int dis;
	int fuel;
}Stop[maxn];

int heap[maxn];

int compare(const stop &a, const stop &b)
{
	return a.dis > b.dis;
}

int main()
{
	while(~ scanf("%d", &N)) {
		for(int i = 0; i < N; ++i) {
			scanf("%d%d", &Stop[i].dis, &Stop[i].fuel);
		}
		scanf("%d%d", &L, &P);

        sort(Stop, Stop + N, compare);

		int remain = L - P;
		int ans = 0;
		int size = 0;
		int i = 0;

		while(remain > 0) {
			for( ; i < N; ++i) {
				// 当前油用完前能经过的加油站入堆
				if(Stop[i].dis >= remain) {
					heap[size++] = Stop[i].fuel;
					push_heap(heap, heap + size);
				}
				// 若当前加油站不能入堆，则下一个也无法入堆
				else
					break;
			}

			if(size == 0) // 未经过任何一个加油站
				break;

			remain -= heap[0];
			ans++;

			// 堆顶出栈
			pop_heap(heap, heap + size);
			size--;
		}

		if(remain > 0)
			cout << -1 << endl;
		else
			cout << ans << endl;

	}

	return 0;
}