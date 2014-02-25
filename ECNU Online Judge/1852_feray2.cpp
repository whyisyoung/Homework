#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
using namespace std;

void farey(int n)
{
	int pa=0, pb=1, ca=1, cb=n;
	int na, nb;

	printf("%d/%d\n", pa, pb);
	printf("%d/%d\n", ca, cb);
	while(ca!=cb){
		na = (pb+n) / cb * ca - pa;
		nb = (pb+n) / cb * cb - pb;
		printf("%d/%d\n", na, nb);
		pa = ca;
		pb = cb;
		ca = na;
		cb = nb;
	}
}
int main()
{
	int n;

	while(cin >> n)
		farey(n);
	return 0;
}