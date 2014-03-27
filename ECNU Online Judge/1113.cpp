#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;

const int maxn = 20002;
int f[maxn];
int V;

int main()
{
	int n;
	int vol[32];

	while(scanf("%d", &V) != EOF) {
		scanf("%d", &n);
		for(int i = 1; i <= n; ++i)
			scanf("%d", &vol[i]);

		memset(f, 0, sizeof(f));

		f[0] = 1;

		for(int i = 1; i <= n; ++i) {
			for(int j = V; j >= vol[i]; --j) {
				f[j] = f[j] || f[j-vol[i]]; // 可形成的且小于V的和都会被赋予 1，如 f[8], f[21], f[20],f[24]等等
			}
		}

		int tmp = V;
		while(tmp > 0 && f[tmp--] == 0); // 从 V 开始递减找到对应 f 值为 0 的

		cout << V - tmp - 1 << endl;
	}
	return 0;
}