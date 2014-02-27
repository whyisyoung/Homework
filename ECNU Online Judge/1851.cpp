#include <iostream>
#include <cstdio>
#include <cmath>
using namespace std;

const int MOD = 98765431;
const int maxn = 50005;

int c[ maxn ];


/*
 * recursive version
 */
// long long single_mod(long long x, long long n) // x^n % MOD
// {
// 	long long ans;

// 	if(n == 0)
// 		return 1;
// 	if(n == 1)
// 		return x % MOD;
// 	ans = single_mod(x, n/2);
// 	ans = ans * ans % MOD;
// 	if(n & 1)
// 		ans = ans * x % MOD;
// 	return ans;
// }

/*
 * unrecursive version
 */
long long single_mod(long long x, long long n) // x^n % MOD
{
	long long ans = 1;
	while(n) {
		if(n % 2)
			ans = ans * x % MOD;
		n >>= 1;
		x = x * x % MOD;
	}
	return ans;
}

long long sum_mod(int n, int t) // ((n-1)^(t-1) - (n-1)^(t-2) + ... + (-1)^(t-1)*(n-1)^0) % MOD
{
	long long a = single_mod(1-n+MOD, t);
	long long b = single_mod(n, MOD-2);
	long ans = ((MOD + 1 - a) * b) % MOD;
	return ((t & 1) ? ans : (MOD-ans));

}

int main()
{
	int n, t;
	while(scanf("%d%d", &n, &t)!=EOF) {
		long long init_sum = 0;

		if(n == 1) {
			cout << 0 << endl;
			continue;
		}


		for(int i = 1; i <= n; i++) {
			scanf("%d", &c[i]);
			init_sum = (init_sum + c[i]) % MOD;
		}

		long long total_sum = sum_mod(n, t) * init_sum % MOD;

		if(t & 1)
        	for(int i = 1; i <= n; i++)
            	printf("%lld\n", (total_sum + MOD - c[i]) % MOD); // You have to add MOD or it fails :(
		else
		 	for(int i = 1; i <= n; i++)
		 		printf("%lld\n", (total_sum + c[i]) % MOD) ;
	}

	return 0;
}