//
//  main.cpp
//  eoj 1488
//
//  Created by whyisyoung on 3/22/14.
//  Copyright (c) 2014 whyisyoung. All rights reserved.
//

#include <iostream>
#include <cstdio>
using namespace std;

// 四条规则：
// 1. 硬币应该按照从小往大拿
// 2. 已经拥有的直接忽略
// 3. 比如 1 + 2 >= 3, 所以 2 是肯定不要找的，再比如 1 + 5 + 10 > 13, 所以 10 也是不在所求结果中的
// 4. 每个硬币最多只要找回一次
// 比如数据：
// 2 28
// 1 0
// 13 0

// 如果找回两个13一个1的话，收集的硬币种类最多也还是只有两种，
// 倒不如找回一个13一个1，这样找回的零钱最少，买到的东西最贵

const int maxn = 500005;
int coin[maxn];
int possess[maxn];
int N, K;

int main()
{
    while(~ scanf("%d%d", &N, &K)) {
    	for(int i = 1; i <= N; ++i) {
    		scanf("%d%d", &coin[i], &possess[i]);
    		// TODO:
  			// getchar() 可以大幅提高速度....只是要加上一个处理函数
    	}

    	int ans = 0;
    	int tmp = 0;
    	coin[N+1] = K;

    	for(int i = 1; i <= N; ++i) {
    		if(possess[i] == 1)
    			continue;
    		else {
    			if(tmp + coin[i] >= coin[i+1])
    				continue;
    			else {
    				tmp += coin[i];
    				++ans;
    			}
    		}
    	}

    	if(ans == 0)
    		cout << 0 << endl << N - 1 << endl;
    	else
    		cout << ans << endl << K-tmp << endl;
    }
    return 0;
}
