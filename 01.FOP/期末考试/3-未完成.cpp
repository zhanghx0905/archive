/*
题目描述
本题首先需要输入一批操作命令来构建一个环形网络，例如图1展示了执行样例输入各操作命令后环形网络的构建情况，然后输入一个指定网络节点的标识，请问从网络接入点到指定的网络节点中间所经过的网络节点列表，以及总的网络传输延迟？
一个环形网络包含多个网络节点，相邻的网络节点之间只能按顺时针方向进行单向的信号传输。每个网络节点具有两个属性，一是每个节点具有唯一的“网络标识”，由6个小写字母组成；二是信号经过每个节点时，会有一定的“传输延迟”，由一个正整数表示。初始时环形网络为空。
对于一个环形网络，我们定义了如下5种操作命令：
1)      添加（Append）：如果网络为空，添加一个节点并将其作为接入点；否则在当前接入点所在的网络节点“逆时针”方向添加一个相邻的节点，并保持接入点位置不变。如命令“Append 1 nameaa”表示添加一个传输延迟为1，网络标识为“nameaa”的节点。
2)      剔除（Eliminate）：以接入点所在的网络节点为1开始朝顺时针方向计数，每计到第m个节点就将其从当前网络中剔除，然后由下一个节点重新计数，直至剔除出n个网络节点为止（n小于网络中节点的数量）。若剔除的网络节点为接入点，则将顺时针方向的下一个网络节点设为接入点。如“Eliminate m n”表示剔除计数为m的n个网络节点。
3)      移动（Move）：将接入点朝顺时针或者逆时针方向移动r个节点，r是一个正整数。如“Move 0 3”表示将接入点按顺时针方向移动3个节点。其中第一个数字“0”表示移动方向，0表示顺时针方向（1表示逆时针方向）；第二个数字“3”表示移动的节点数目。
4)      排序（Sort）：将环形网络中网络节点按传输延迟由小到大排序，若传输延迟相同，则按节点网络标识的字典序排列。新的接入点仍指向排序前的网络节点。
5)      删除（Delete）：从环形网络中删除一个指定网络标识的节点。当指定网络标识的节点不存在时，则不执行任何操作；当删除的节点为接入点时，则将接入点顺时针方向第一个节点作为新的接入点；当删除网络中剩余的最后一个节点时，则把接入点置空。如“Delete namebb”表示删除环形网络中网络标识为“namebb”的网络节点。
注意：所有网络节点的网络标识都不相同，但网络传输延迟可以相同。
输入描述
第一行一个整数T，表示命令数量。( 1 <= T <= 1000)
接下来的T行，每行输入一个命令。
最后一行输入一个网络标识id，表示需要查询的网络节点。

输出描述
输出一行或者多行。如果最终环形网络为空或指定查询网络标识对应的节点不在环形网络中，则输出“-1”；如果查询的节点就是接入点，则输出“0”；否则按序输出接入点到指定网络标识节点之间经过网络节点的网络标识列表（每行输出一条），以及总的传输延迟。

样例输入
8
Append 1 nameaa
Append 3 bbbbbb
Append 5 aabbcc
Append 4 abcdef
Sort
Eliminate 2 1
Move 0 1
Delete nameaa
aabbcc
样例输出
abcdef
4
*/
#include<algorithm>
#include<cmath>
#include<cstdlib>
#include<cstdio>
#include<cstring>
#include<ctime>
#include<ctype.h>
#include<iomanip>
#include<iostream>
#include<string>
using namespace std;
const int N = 10000;

struct map {
	char name[100];
	int t;  //延迟
	int pre, next;//后继和前驱
}a[N + 1];
int mark = -1;
char input[100];

int main() {
	int t, ord = 0;
	cin >> t;
	for (int i = 0; i < t; i++) {
		cin >> input;
		if (!strcmp(input, "Append")) {
			char name[100];
			int tar = 0;
			cin >> tar >> name;
			if (mark == -1) {
				mark = ord;
				strcpy(a[ord].name, name);
				a[ord].t = tar;
				a[ord].pre = mark;
				a[ord].next = mark;
				a[ord].index = ord;
				ord++;
			}
			else {
				int b = mark;
				while (a[b].next != mark) b = a[b].next;
				strcpy(a[ord].name, name);
				a[ord].t = tar;
				a[ord].next = mark;
				a[mark].pre = ord;
				a[ord].pre = b;
				a[b].next = ord;
				a[ord].index = ord;
				ord++;
			}
		}
		if (!strcmp(input, "Eliminate")) {
			int m, n;
			cin >> m >> n;
			int count_m = 1, count_n = 0;
			int b = mark;
			while (true) {
				b = a[b].next;
				count_m++;
				//执行删除操作
				if (count_m == m) {
					count_n++;
					count_m = 0;
					int pre = a[b].pre;
					int next = a[b].next;
					a[pre].next = next;
					a[next].pre = pre;
					a[b].Not = true;
					if (b == mark)
						mark = a[b].next;
				}
				//如果已经删够了,就退出
				if (count_n == n)
					break;
			}
		}
		if (!strcmp(input, "Move")) {
			int p, r;
			cin >> p >> r;
			if (p == 0) {
				for (int i = 0; i < r; i++)
					mark = a[mark].next;
			}
			else if (p == 1) {
				for (int i = 0; i < r; i++)
					mark = a[mark].pre;
			}
		}
		if (!strcmp(input, "Delete")) {
			char del[100];
			cin >> del;
			int b = mark;
			while (strcmp(a[b].name, del) != 0 && a[b].next != mark) {
				b = a[b].next;
			}
			if (b == mark) {
				if (!strcmp(a[b].name, del)) {
					int pre = a[b].pre;
					int next = a[b].next;
					if (pre == next)
						mark = -1;
					else {
						a[pre].next = next;
						a[next].pre = pre;
						a[b].Not = true;
						mark = a[b].next;
					}
				}
			}
			else {
				int pre = a[b].pre;
				int next = a[b].next;
				a[pre].next = next;
				a[next].pre = pre;
				a[b].Not = true;
			}

		}
		if (!strcmp(input, "Sort")) {

		}
	}
	cin >> input;
	if (mark == -1) {
		cout << "-1";
		return 0;
	}
	if (!strcmp(a[mark].name, input))
		cout << '0';
	else {
		int b = mark, count = a[mark].t, j = 0;
		char jinguo[100][100];
		strcpy(jinguo[j++], a[mark].name);
		while (strcmp(a[b].name, input) != 0 && a[b].next != mark) {
			b = a[b].next;
			count += a[b].t;
			strcpy(jinguo[j++], a[mark].name);
		}
		if (b == mark) cout << "-1";
		else {
			for (int i = 0; i < j; i++)
				cout << jinguo[i] << endl;
			cout << count;
		}
	}

	return 0;
}