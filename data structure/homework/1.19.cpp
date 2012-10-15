#include<iostream>
#include<cstdio>
#include<vector>
#include<string>
#include<map>
#include<cmath>
#include<algorithm>
#include<queue>
#include<cstring>
using namespace std;
const int maxn=65;
struct node{
	int n;
	node *link;
};
node *head;
node *create(int n)
{
	node *p,*q;
	if(n==0)
		return NULL;
	head=new node;
	p=head;
	for(int i=1;i<n;i++){
		cin>>p->n;
		q=new node;
		p->link=q;
		p=q;
	}
	cin>>p->n;
	p->link=NULL;
	return head;
}

node *newcreate(node *head)
{
	node *p,*q;
	int x=0;
	if(head==NULL){
		head=p;
		p->n=x;
		p->link=head;
		return head;
	}
	q=head;
	while(head->link){
		head=head->link;
		x++;
	}
	p->n=x;
	head->link=p;
	p->link=q;
	return p;
}