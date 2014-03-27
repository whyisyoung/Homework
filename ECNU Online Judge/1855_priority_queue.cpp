#include<iostream>
#include<cstdio>
#include<string>
#include<vector>
#include<map>
#include<cmath>
#include<algorithm>
#include <cstring>
#include <queue>
using namespace std;
typedef pair<int,int> Stop;
#define d first
#define p second
const int L=10010;
int n;
Stop stop[L],town;
int main()
{
	int i,ans=0;
	priority_queue<int>heap;
	cin>>n;
	for(i=0;i<n;i++)
		cin>>stop[i].d>>stop[i].p;
	cin>>town.d>>town.p;
	sort(stop,stop+n);
	i=n-1;
	while ((i>=0) && (town.d-stop[i].d<=town.p)){
		heap.push(stop[i].p);
        --i;
    }
    while ((town.d>town.p) && (!heap.empty())){
		++ans;
        town.p+=heap.top();
        heap.pop();
        while ((i >= 0) && (town.d-stop[i].d<=town.p)){
			heap.push( stop[i].p);
            --i;
        }
    }
    printf("%d\n",((town.d<=town.p)?ans :(-1)));
    return 0;
}
