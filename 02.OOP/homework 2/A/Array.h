#pragma once
#include"Node.h"
#include<vector>
#include<list>
class Array
{
	int len;
	Node* Nodes;
public:
	Array(int _len);
	//Array(const Array&);
	//Array(Array&&);
	~Array();
	void update(int q, int r);
	void insert(int q, int r);
	void swap(int q, int r);
	Node& operator[](int i);
};
