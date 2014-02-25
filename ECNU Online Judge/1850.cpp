//
//  main.cpp
//  eoj1850
//
//  Created by whyisyoung on 2/18/14.
//  Copyright (c) 2014 whyisyoung. All rights reserved.
//

#include <iostream>
#include <cstdio>
#include <cmath>
using namespace std;

int a[105][60]; // a[i][j] 代表的是第 i 个Catalan数的倒数第 j+1 位, i 从 1 算起
int b[60];      // b[i] 代表的是第 i 个Catalan数的位数

void catalan() //求卡特兰数
{
    int i, j, len, carry, temp;
    a[1][0] = b[1] = 1;
    len = 1;
    for(i = 2; i <= 100; i++) {
        for(j = 0; j < len; j++) //乘法
            a[i][j] = a[i-1][j] * (4*i - 2);
        carry = 0;
        for(j = 0; j < len; j++) { //处理相乘结果
            temp = a[i][j] + carry;
            a[i][j] = temp % 10;
            carry = temp / 10; // carry 为 a[i-1][j] * (4*i - 2) 的进位结果
        }
        while(carry) { //进位处理
            a[i][len++] = carry % 10;
            carry /= 10;
        }
        carry = 0;
        for(j = len-1; j >= 0; j--) { //除法
            temp = carry*10 + a[i][j]; // 还原 a[i-1][j] * (4*i - 2) 的结果
            a[i][j] = temp / (i+1);  // 递推式中要把相乘结果除以 i+1
            carry = temp % (i+1);
        }
        while(!a[i][len-1]) //高位零处理
            len--;
        b[i] = len;
    }
}



int main(int argc, char const *argv[])
{
    int n;

    catalan();

    while(cin >> n) {
        for(int i = b[n]-1; i >= 0; i--)
            cout << a[n][i];
        cout << endl;
    }
    return 0;
}
