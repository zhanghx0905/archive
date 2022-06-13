#include "compute.h"

map<string, int> compute::precedence = {
	map<string, int>::value_type("<=",4),
	map<string, int>::value_type("<",4),
	map<string, int>::value_type(">=",4),
	map<string, int>::value_type(">",4),
	map<string, int>::value_type("==",4),
	map<string, int>::value_type("!=",4),
	map<string, int>::value_type("not",3),
	map<string, int>::value_type("and",2),
	map<string, int>::value_type("or",1),
};

//将中缀表达式转换为逆波兰表达式
vector<string> compute::Transfer(const vector<string> &str)const{
	vector<string> result;
	stack<string> operators;
	if (str.empty()) return result;

	for (int i = 0; i < str.size(); ++i)
	{
		auto pos = precedence.find(str[i]);	
		if (pos == precedence.end()) {
			result.push_back(str[i]);
		}

		else{
			int weight = precedence[str[i]];
			if (operators.empty() || weight > precedence[operators.top()])
				operators.push(str[i]);
			else {
				while (!operators.empty())
				{
					string op = operators.top();
					if (precedence[op] >= weight)
					{
						result.push_back(op);
						operators.pop();
					}
					else break;
				}
				operators.push(str[i]); 
			}
		}
	}
	while (!operators.empty())
	{
		string tmp = operators.top();
		result.push_back(tmp);
		operators.pop();
	}
	return result;
}

int compute::Calculate(const vector<string> &operations)const
{
	vector<string> result = Transfer(operations);
	stack<int> nums;

	for (int i = 0; i < result.size(); ++i)
	{
		//检查是否是变量名
		variable tmp(result[i]);
		auto pos = graph.find(tmp);
		if (pos != graph.end()) {
			nums.push((*pos).getValue());
			continue;
		}
		//检查是否是int变量
		if (IsDigit(result[i])) {
			nums.push(stoi(result[i]));
			continue;
		}
		//检查是否是bool变量
		if (result[i] == "True") {
			nums.push(1);
			continue;
		}
		else if (result[i] == "False") {
			nums.push(0);
			continue;
		}

		int num2 = nums.top();
		nums.pop();

		if (result[i] == "not") {
			nums.push((!num2));
			continue;
		}

		int num1 = nums.top();
		nums.pop();
		if (result[i] == ">")
			nums.push((num1 > num2));
		else if (result[i] == ">=")
			nums.push((num1 >= num2));
		else if (result[i] == "<")
			nums.push((num1 < num2));
		else if (result[i] == "<=")
			nums.push((num1 <= num2));
		else if (result[i] == "==")
			nums.push((num1 == num2));
		else if (result[i] == "!=")
			nums.push((num1 != num2));
		else if (result[i] == "and")
			nums.push((num1 && num2));
		else if (result[i] == "or")
			nums.push((num1 || num2));

	}
	return nums.top();
}

void compute::insert(const string& name, vector<string>& value)
{
	if (value.size() == 1) {
		if (value[0] == "True") {
			variable tmp(name, Bool, 1);
			auto pos = graph.find(tmp);
			if (pos != graph.end())
				graph.erase(*pos);
			graph.insert(tmp);
		}
		else if (value[0] == "False") {
			variable tmp(name, Bool, 0);
			auto pos = graph.find(tmp);
			if (pos != graph.end())
				graph.erase(*pos);
			graph.insert(tmp);
		}
		else if (IsDigit(value[0])) {
			variable tmp(name, Int, stoi(value[0]));
			auto pos = graph.find(tmp);
			if (pos != graph.end())
				graph.erase(*pos);
			graph.insert(tmp);
		}
		else {
			variable tmp1(value[0]), tmp2(name);
			auto pos1 = graph.find(tmp1), pos2 = graph.find(tmp2);
			if (pos1 != graph.end() && pos1 != pos2) {
				if (pos2 != graph.end()) {
					graph.erase(*pos2);
				}
				variable tmp(name, pos1->getType(), pos1->getValue());
				graph.insert(tmp);
			}
		}
	}
	else {
		int result = Calculate(value);
		variable tmp(name, Bool, result);
		auto pos = graph.find(tmp);
		if (pos != graph.end())
			graph.erase(*pos);
		graph.insert(tmp);
	}
}

void compute::print(const vector<string>& value)const
{
	if (value.size() == 1) {
		if (value[0] =="True" || value[0] == "False" || IsDigit(value[0]))
			std::cout << value[0] << std::endl;
		else {
			variable tmp(value[0]);
			auto pos = graph.find(tmp);
			if (pos != graph.end())
				(*pos).print();
		}
	}
	else {
		int result = Calculate(value);
		if (result == 1)
			std::cout << "True" << std::endl;
		else std::cout << "False" << std::endl;
	}
}

bool IsDigit(const string& result) {
	const char* tmp_char = result.c_str();
	for (int j = 0; j < result.size(); ++j)
		if (!isdigit(tmp_char[j])) {
			return false;
		}
	return true;
}

vector<string> split(const string &str, const string &sep)
{
	vector<string> tmp;
	string::size_type pos[2] = { 0, str.find(sep) };
	while (string::npos != pos[1])
	{
		tmp.push_back(str.substr(pos[0], pos[1] - pos[0]));
		pos[0] = pos[1] + sep.size();
		pos[1] = str.find(sep, pos[0]);
	}
	if (pos[0] != str.length())
		tmp.push_back(str.substr(pos[0]));

	return tmp;
}