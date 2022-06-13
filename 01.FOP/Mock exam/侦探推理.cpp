/*
同洛谷P1039 侦探推理
*/#include<iostream>
#include<cstring>
#include<string>
using namespace std;

int fake[21], whos[105], error, offend = -1;
string names[21], say[105];
string day[7] =
{ "Today is Sunday.", "Today is Monday.", "Today is Tuesday.",
"Today is Wednesday.", "Today is Thursday.", "Today is Friday.",
"Today is Saturday." };
//判断是否有矛盾发生
inline void judge(int who, int yx) {
	if (fake[who] && fake[who] != yx)
		error = 1;
	else
		fake[who] = yx;
}

int main() {
	int m, n, p;
	cin >> m >> n >> p;

	for (int i = 0; i < m; i++)
		cin >> names[i];

	for (int i = 0; i < p; i++) {
		//读入证人的名字,需要去掉":"
		string name;
		cin >> name;
		name.erase(name.end() - 1);
		//记录说话者
		for (int j = 0; j < m; j++)
			if (names[j] == name) {
				whos[i] = j;
				break;
			}
		getline(cin, say[i]);
		say[i].erase(say[i].begin());   //去掉空格 
		//此处特别注意，windows换行是\r\n，/LINUX换行是\n，有的oj读入时需要将换行符去掉
		//say[i].erase(say[i].end()-1);
	}

	for (int td = 0; td < 7; td++)      //枚举今天是星期几 
		for (int px = 0; px < m; px++) {        //枚举罪犯 
			error = 0;
			memset(fake, 0, sizeof(fake));
			//依次判断每句话
			for (int i = 0; i < p; i++) {
				int who = whos[i];

				if (say[i] == "I am guilty.")
					judge(who, px == who ? 1 : -1);
				if (say[i] == "I am not guilty.")
					judge(who, px != who ? 1 : -1);

				for (int j = 0; j < m; j++) {
					if (say[i] == names[j] + " is guilty.")
						judge(who, j == px ? 1 : -1);
					if (say[i] == names[j] + " is not guilty.")
						judge(who, j != px ? 1 : -1);
				}

				for (int j = 0; j < 7; j++)
					if (say[i] == day[j])
						judge(who, j == td ? 1 : -1);
			}
			//faker:说假话的人数，unkonwn:不确定的人数 
			int faker = 0, unkonwn = 0;
			for (int i = 0; i < m; i++) {
				if (fake[i] == -1)
					faker++;
				if (fake[i] == 0)
					unkonwn++;
			}
			if (!error && faker <= n && faker + unkonwn >= n) {
				if (offend != -1 && offend != px) {
					//如果出现了两个合理的罪犯，结束枚举
					cout << "Cannot Determine" << endl;
					return 0;
				}
				else
					offend = px;
			}
		}

	if (offend == -1)
		cout << "Impossible" << endl;
	else
		cout << names[offend] << endl;

	return 0;
}