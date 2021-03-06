#include<iostream>
#include<cstring>
using namespace std;
const int N = 500;
char input1[N + 1], input2[N + 1];
int input1_num[N + 1], input2_num[N + 1], out[N + 1];
int main() {
	cin >> input1 >> input2;
	int len1 = strlen(input1), len2 = strlen(input2);
	int lenmax = (len1 >= len2) ? len1 : len2;
	for (int i = len1 - 1, j = 0; i >= 0; i--, j++)
		input1_num[j] = input1[i] - '0';
	for (int i = len2 - 1, j = 0; i >= 0; i--, j++)
		input2_num[j] = input2[i] - '0';
	for (int k = 0; k < lenmax; k++) {
		out[k] += input1_num[k] + input2_num[k];
		if (out[k] >= 10) {
			out[k + 1] += 1;
			out[k] -= 10;
		}
	}
	if (out[lenmax])
		for (int j = lenmax; j >= 0; j--)
			cout << out[j];
	else
		for (int j = lenmax - 1; j >= 0; j--)
			cout << out[j];
	return 0;
}
