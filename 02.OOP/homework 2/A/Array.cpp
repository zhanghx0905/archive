#include "Array.h"



Array::Array(int _len):len(_len)
{
	Nodes=new Node[_len];
}


Array::~Array()
{
	if (Nodes)
		delete[] Nodes;
}

void Array::update(int q, int r)
{
	Nodes[q] = Nodes[r];
}

void Array::insert(int q, int r)
{
	for (int i = len - 1; i > q; i--) {
		Nodes[i] = std::move(Nodes[i-1]);
	}
	Nodes[q] = std::move(Node(r));
}

void Array::swap(int q, int r)
{
	std::swap(Nodes[q], Nodes[r]);
}

Node& Array::operator[](int i)
{
	return Nodes[i];
}