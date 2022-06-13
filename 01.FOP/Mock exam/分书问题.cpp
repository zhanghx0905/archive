/*
题目描述
现在有N本书，准备分给N个人。每个人的阅读兴趣，用一个二维数组like[i][j]加以描述。
若 like[i][j] = 1 ，则表示第i个人喜欢第j本书；反之，若 like[i][j] = 0 ，则表示第i个人不喜欢第j本书。

现在已知like数组，请写一个程序，输出所有的分书方案，让人人皆大欢喜。
输入描述
第一行有一个整数N。(N <= 10)
之后有N行，每i行有N个数a_ij，取值为0或1，表示like数组，即第i个人是否喜欢第j本书。0表示不喜欢，1表示喜欢。
输出描述
输出多行。
每行输出一种分书方案，按顺序输出1~N每个书分给对应的人的编号。
所有分书方案，按字典序从小到大的顺序输出。*/
#include <iostream>		// cout
using namespace std;

int n;            // 书和人的个数
int take[11];        // 记录书分别分给谁
bool assigned[11];   // 记录书是否已分配

int like[11][11];

void Try(int id) {
	// 递归中止条件：所有读者均已分配合适书籍
	if (id == n) {
		for (int i = 0; i < n; i++)
			cout << take[i] << ' ';
		cout << endl;
		return;
	}

	// 逐一为每本书找到合适的读者
	for (int book = 0; book < n; book++) {
		if ((like[id][book] == 1) && !assigned[book]) {
			take[id] = book+1;
			assigned[book] = true;

			Try(id + 1);

			//回溯，尝试另一种方案
			assigned[book] = false;
		}
	}
}

int  main() {
	cin >> n;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			cin >> like[j][i];
	Try(0);

	return 0;
}
