//
//  main.cpp
//  eoj1852
//
//  Created by whyisyoung on 2/23/14.
//  Copyright (c) 2014 whyisyoung. All rights reserved.
//

//
// What if you are not allowed to use map, how to sort??
// Note: eliminate duplication can be done with gcd() constraint
//

#include <iostream>
#include <cstdio>
#include <algorithm>
#include <map>
#include <cstdlib>
#include <string>
#include <cstring>
using namespace std;

map<double, string> frac;

int gcd(int a, int b)
{
	return (b == 0)?a:(gcd(b, a % b));
}

int generate_fractions(int n)
{
    int size = 0;

    for (int i = 1; i <= n-1; ++i) {
		for(int j = i+1; j <= n; j++) {
			if(gcd(i, j) > 1)
				continue;
			else {
				char cstr[15], b[4];
				itoa(i, cstr, 10);
				itoa(j, b, 10);
				strcat(cstr, "/");
				strcat(cstr, b);
				string str(cstr);
				frac[i*1.0 / j] = str;
				size++;
            }
		}
	}
	return size;
}

int main(int argc, char const *argv[])
{
	int n, size;
	while(cin >> n) {
		size = generate_fractions(n);

		cout << "0/1\n";

		map<double, string>::iterator it = frac.begin();

		for( ; it != frac.end(); it++) {
			cout << it->second << endl;
		}

		cout << "1/1\n";
	}
	return 0;
}