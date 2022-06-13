/*
用1到9组成一个九位数，使得这个数的前n位组成的n位数能被n整除，1<=n<=9

answer:381654729
*/
#include<iostream>
using namespace std;

int nums[10];

int ToInt(int digit,int a[]) {
	int temp = 0;
	for (int i = 1; i <= digit; i++)
		temp += a[i] * pow(10, digit - i);
	return temp;
}

void dfs(int digit,int num) {
	if (num == 10) return;

	if (digit == 10) {
		for (int i = 1; i <= 9; i++) cout << nums[i];
		return;
	}


	nums[digit] = num;
	if (!(ToInt(digit, nums) % digit)) {
		int mark = 0;
		for (int i = 1; i < digit; i++)
			if (nums[digit] == nums[i]) {
				mark = 1;
				break;
			}

		if (!mark)  dfs(digit + 1, 1);
	}
	dfs(digit, num + 1);

}

int main(){
	dfs(1, 1);
	return 0;
}