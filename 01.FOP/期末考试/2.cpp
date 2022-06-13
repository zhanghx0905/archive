/*
题目描述
小明参加了系里的学术新星计划，最近他有一个不成熟的idea，并打算找他的导师进行讨论。
现在是2019.1.1的0:00，在Google Calendar上小明看到他的导师有N个时间段已被占用（被占用的时间段之间互不重叠），以及导师将会在Y年M月D日H时m分出差。小明想知道，在导师出差前，他能预约到的最长讨论时间段为多少分钟？
注意：需要考虑跨日、跨月的情况。为了简便，所有的测试用例的时间均在2019年内。
输入描述
第一行六个整数，依次为N, Y, M, D, H, m，分别表示被占用时间段数目N和导师出差的时间（Y年M月D日H时m分）。其中H是24小时制，m是60分钟制。
接下来N行，每行十个整数, s1~s5，t1~t5，表示s1年s2月s3日s4时s5分到t1年t2月t3日t4时t5分导师在忙。（所有被占用的时间段按时间的先后有序输入，不考虑无序的情况）
输出描述
小明能预约到的最长讨论时间段的分钟数。
样例输入
1 2019 1 2 0 0
2019 1 1 3 0 2019 1 1 17 0
样例输出
420
*/
#include<iostream>
using namespace std;
const int N = 10000;
int booked[N + 1][2];
//计算当前时间到开始时间一共多少分钟
int calmin(int s2,int s3,int s4,int s5) {
	int ans = s5;
	switch (s2){
	case 1:  ans += 0; break;
	case 2:  ans += 31 * 24 * 60; break;
	case 3: ans += (28 + 31) * 24 * 60; break;
	case 4: ans+=(28+62) * 24 * 60; break;
	case 5: ans+=(28+62+30) * 24 * 60; break;
	case 6: ans += (151) * 24 * 60; break;
	case 7:ans+= (151+30) * 24 * 60; break;
	case 8:ans+=(181+31) * 24 * 60; break;
	case 9:ans+= (212+31) * 24 * 60; break;
	case 10:ans += (243+30) * 24 * 60; break;
	case 11:ans += (31+273) * 24 * 60; break;
	case 12:ans += (304+30) * 24 * 60; break;
	}
	ans += (s3-1) * 24 * 60 + s4 * 60;
	return ans;
}
int main() {
	int N, Y, M, D, H, m;
	cin >> N >> Y >> M >> D >> H >> m;
	int limit = calmin(M, D, H, m);
	int s1, s2, s3, s4, s5;
	int t1, t2, t3, t4, t5;
	
	for (int i = 0; i < N; i++) {
		cin >> s1 >> s2 >> s3 >> s4 >> s5;
		cin >> t1 >> t2 >> t3 >> t4 >> t5;
		booked[i][1] = calmin(t2, t3, t4, t5);
		booked[i][0] = calmin(s2, s3, s4, s5);
	}
	int j = 0;
	int Max = booked[0][0];
	while (booked[j][1] <= limit&&j+1<N) {
		j++;
		Max = max(Max, booked[j][0]-booked[j-1][1]);
	}
	Max = max(Max,limit- booked[j][1]);
	cout << Max;

	return 0;
}