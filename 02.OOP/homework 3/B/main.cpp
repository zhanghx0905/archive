#include<iostream>
using namespace std;

#include"compute.h"

int main() {
	string tmp;
	compute reckoner;

	while (getline(cin, tmp))
	{
		string::size_type pos = tmp.find(" ");
		string act = tmp.substr(0, pos);

		if (act == "print") {
			tmp.erase(0, pos + 1);
			vector<string> input = split(tmp, " ");

			reckoner.print(input);
		}
		else {
			string::size_type pos = tmp.find("=");
			string name = tmp.substr(0, pos);
			tmp.erase(0, pos + 2);
			vector<string> input = split(tmp, " ");

			reckoner.insert(act, input);
		}
	}
	return 0;
}