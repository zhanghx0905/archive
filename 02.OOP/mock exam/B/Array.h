#pragma once
#include "Node.h"
#include<vector>
#include<algorithm>
using namespace std;

class Array {
	vector<Node> nodes;

public:
	Array(int _len) {
		nodes.resize(_len);
	}
	Array(Array&& obj) {
		nodes = std::move(obj.nodes);
	}
	Array(const Array& obj) :nodes(obj.nodes){
	
	}
	Node& operator[](int n) {
		if (n < nodes.size())
			return nodes[n];
		return nodes[0];
	}
	void operator=(Array&& obj) {
		nodes = std::move(obj.nodes);

	}
	void operator=(const Array& obj) {
		nodes = obj.nodes;
	}
};