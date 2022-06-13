#pragma once
#include<set>
#include<map>
#include<stack>
#include<vector>

using std::vector;
using std::stack;
using std::map;
using std::set;
using std::string;

#include"variable.h"

class compute
{
 public:
	compute() = default;

	vector<string> Transfer(const vector<string>& str)const;
	int Calculate(const vector<string>& operations)const;
	void insert(const string & _name, vector<string>& _value);
	void print(const vector<string>& value)const;

 private:
	static map<string, int> precedence;
	set<variable> graph;
};

bool IsDigit(const string & result);
vector<string> split(const string & str, const string & sep);
