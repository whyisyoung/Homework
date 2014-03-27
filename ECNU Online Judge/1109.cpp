#include <iostream>
#include <cstdio>
using namespace std;

const int maxn = 100005;
int a[maxn];

 // 若一个子序列的 和 是 负 的，则它不可能是最大连续子串的一部分。

int get_max_subsequence_sum(int a[], int size)
{
	int current_sum = 0;
	int start = 0;
	int max_sum = 0;

	for(int i = 0; i < size; ++i) {
		current_sum += a[i];

		if(current_sum < 0) {
			start = i + 1;
			current_sum = 0;
		}

		if(current_sum > max_sum)
			max_sum = current_sum;
	}

	return max_sum;
}

int main()
{
	int t, n;
	scanf("%d", &t);
	while(t--) {
		bool all_negative = true;
		int ans = 0;

		scanf("%d", &n);
		for(int i = 0; i < n; ++i) {
			scanf("%d", &a[i]);
			if(a[i] > 0)
				all_negative = false;
		}

		if(all_negative) {
			ans = a[0];
			for(int i = 1; i < n; ++i) {
				if(a[i] > ans)
					ans = a[i];
			}
		}

		else
			ans = get_max_subsequence_sum(a, n);

		cout << ans << endl;

	}
	return 0;
}