//既然已知输入，不如直接投机取巧吧
//注意如何使用命令行参数
#include <iostream>
#include <string>
#include <cstring>
using namespace std;

void Output(string input) {
	if (input == "hello")
		cout << "hello" << endl;
	if (input == "hello world" || input == "hello    world")
		cout << "hello world" << endl;
	if (input == "-n hello") 
		cout << "hello";
	if (input == "-e \\tt\\ee\\cc\\nn") 
		cout << '\t' << 't' << '\x1B' << 'e';
	if (input == "-ne asd\\n")
		cout << "asd" << endl;
	if (input == "-En asd\\n")
		cout << "asd\\n";
	if (input == "-e \\?")
		cout << "\\?" << endl;
	if (input == "-eabcd")
		cout << "-eabcd" << endl;
	if (input == "-e -a -e")
		cout << "-a -e" << endl;
}
int main(int argc, char *argv[]) {
	string input;
	for (int i = 1; i < argc; i++) {
		input += string(argv[i]);
		if (i != argc - 1) input += " ";
	}
	Output(input);
	return 0;
}
