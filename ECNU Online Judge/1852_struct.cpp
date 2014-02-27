//
//  main.cpp
//  eoj1852_struct
//
//  Created by whyisyoung on 2/27/14.
//  Copyright (c) 2014 whyisyoung. All rights reserved.
//

#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstdlib>
using namespace std;

struct fraction {
	int numerator;
	int denominator;
}frac[7820];

int gcd(int a, int b)
{
	return (b == 0) ? a : (gcd(b, a % b));
}

int generate_fractions(int n)
{
	int count = 0;

	for(int i = 1; i <= n-1; i++) {
		for(int j = i+1; j <= n; j++) {
			if(gcd(i, j) == 1) {
				frac[count].numerator 	 = i;
				frac[count++].denominator = j;
			}
		}
	}
	return count;
}

int compare(const void* a, const void* b)
{
	fraction *x = (fraction *)a;
	fraction *y = (fraction *)b;
	double frac_a = x->numerator * 1.0 / x->denominator;
	double frac_b = y->numerator * 1.0 / y->denominator;
	if(frac_a < frac_b)
		return -1;
	if(frac_a > frac_b)
		return 1;
	return 0;
}

int main(int argc, char const *argv[])
{
	int n, count;

	while(scanf("%d", &n) != EOF) {
		count = generate_fractions(n);

		cout << "0/1\n";

		qsort(frac, count, sizeof(frac[0]), compare);
		for(int i = 0; i < count; i++)
			cout << frac[i].numerator << '/' << frac[i].denominator << endl;

		cout << "1/1\n";
	}
	return 0;
}