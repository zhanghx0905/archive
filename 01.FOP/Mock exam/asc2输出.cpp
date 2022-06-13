/*
题目描述
给定一个文件，由大小写英文和标点符号组成。依照ASCII值的从小到大的顺序，统计输出各字符的数量。
输入描述
从input.txt读入。
文件中包含大小写英文、标点符号、空格等ASCII码可见字符，以及换行符、tab等。
输入文件小于100kb。
输出描述
输出到output.txt中。
依照ASCII值的大小，统计输出各字符的数量。
其中，没有出现的字符不用输出，不可见字符（换行符、tab等）不用统计，空格不用统计。
输出格式每行为 x:y，其中x为对应字符，y为数量。具体格式参考见样例输出。*/
#include <iostream>
#include <fstream>
#include<string>
using namespace std;
string input;
int num[127];
int  main() {
	ifstream fin("input.txt");
	while (getline(fin, input)) {
		for (int i = 0; input[i] != '\0'; i++) {
			if (input[i] != ' ')
				num[(int)input[i]]++;
		}
	}
	fin.close();
	ofstream fout("output.txt");
	for (int i = 33; i < 127; i++) {  //!在asc2码中排第33,前面的全是操作符
		if (num[i] != 0&&num[i]<=100000) {
			fout << (char)i << ':' << num[i] << endl;
		}
	}
	return 0;
}
