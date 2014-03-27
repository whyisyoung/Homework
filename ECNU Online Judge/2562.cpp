//
//  main.cpp
//  eoj2562
//
//  Created by whyisyoung on 3/27/14.
//  Copyright (c) 2014 whyisyoung. All rights reserved.
//

#include <iostream>
#include <cstdio>
#include <map>
using namespace std;

const int maxn = 100002;
const int len = 21;

int disset[maxn];
int cnt[maxn];
map<string, int> name;

void init()
{
	name.clear();

	for(int i = 1; i < maxn; ++i) {
		disset[i] = i; // 所有的结点初始均以自己为根节点
		cnt[i] = 1;    // 所有结点的初始子节点只有自己一个，所以均为 1， 合并后会增加
	}
}

int find(int x)
{
	int r = x;
	while(disset[r] != r)
		r = disset[r];
	int i = x;
	while(i != r) {
		int j = disset[i];
		disset[i] = r;
		i = j;
	}

	return r;
}

void merge(int x, int y)
{
	int root_x = find(x);
	int root_y = find(y);

	if(root_x != root_y) {          // 两个结点的根节点不同时才合并，否则无需合并
		disset[root_x] = root_y;    // 此处无需考虑结点大小问题，只要合并成一棵新的有向树即可
		cnt[root_y] += cnt[root_x]; // 合并之后该根节点的子节点个数增多
	}

	printf("%d\n", cnt[root_y]);
}

int main()
{
	int c;
	int f;
	char na[len], nb[len];
	while(~ scanf("%d", &c)) {
		while(c--) {
			scanf("%d", &f);

			init();

			int tmp = 1;

			for(int i = 1; i <= f; ++i) {
				scanf("%s%s", na, nb);

				if(!name[na]) // na 是以前未出现过的名字
					name[na] = tmp++;
				if(!name[nb]) // nb 是以前未出现过的名字
					name[nb] = tmp++; // 比如名字的顺序是A,B; B,C; 那么 A,B,C 对应的int值则分别为1，2，3
				merge(name[na], name[nb]);
			}
		}
	}
	return 0;
}