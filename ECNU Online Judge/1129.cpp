#include <iostream>
#include <cstdio>
using namespace std;

long long f[21] = {0, 0, 1, 2};

long long factorial(int n)
{
    return (n == 0)?1:(factorial(n - 1) * n);
}

void get_first_twenty_derangement()
{
	for (int i = 4; i < 20; ++i)
		f[i] = (i - 1) * (f[i - 1] + f[i - 2]);
}

int main()
{
	int c;
	int m, n;
	long long ans;

	get_first_twenty_derangement();

	scanf("%d", &c);
	for (int i = 0; i < c; ++i) {
		scanf("%d%d", &n, &m);

		ans = factorial(n) / (factorial(m) * factorial(n-m)) * f[m];
		cout << ans << endl;
	}

	return 0;
}
