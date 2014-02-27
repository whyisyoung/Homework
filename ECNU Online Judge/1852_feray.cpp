//
//  main.cpp
//  eoj1852
//
//  Created by whyisyoung on 2/23/14.
//  Copyright (c) 2014 whyisyoung. All rights reserved.
//

#include <iostream>
#include <cstdio>
using namespace std;

int N;

void Feray(int a, int b, int c, int d)
{
	if(b + d > N)
		return ;
	Feray(a, b, a+c, b+d);
	cout << a+c << '/' << b+d << endl;
	Feray(a+c, b+d, c, d);
}

int main(int argc, char const *argv[])
{
	while(cin >> N) {
		cout << "0/1\n";
		Feray(0, 1, 1, 1); // The first two number is 0/1 and 1/1
        cout << "1/1\n";
	}
	return 0;
}