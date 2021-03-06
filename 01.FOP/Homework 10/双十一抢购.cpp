/*
问题描述
　　一年一度的双十一又来了，某网购网站又开始了半价销售的活动。
　　小G打算在今年的双十一里尽情地购物，以享受购买的极度快感，她已经列好了她想买的物品的列表。
　　当然小G并不是出身富贵家庭，所以她网银里的钱只是一个有限的整数S（单位：元）。
　　这次抢购她打算遵循这三个原则选择每一个物品：
　　1.先买能“赚”最多的；
　　2.在“赚”一样多的情况下，先买最便宜的（这样买的东西就可能更多了）；
　　3.在前两条里都判断不了购买顺序的话，先购买在列表里靠前的。
　　（由于网站里还是有一部分商品并没有打五折，所以2的情况（“赚”的钱数为0）是完全可能发生的）
　　现在，在双十一的这一天，你要帮小G编写一个程序，来看看她应该去买她列表里的哪些物品。（总价格不要超过S哦）
输入格式
　　输入共N+1行。
　　第一行包含两个整数S和N，S表示小G的可用金额，N表示她看上的物品个数。
　　接下来N行，对应每一个物品，每行有两个整数a和b，a是物品的原价（单位：元），b为0或1，若b为0，则此物品不半价，若b为1，则此物品半价销售。
输出格式
　　输出共一行，为小G要买的物品序号（从1开始），用空格隔开，注意按序号从小到大输出。
　　若小G一件都买不了，则输出0.
样例输入
10 3
5 0
4 0
10 1
样例输出
2 3
样例输入
10 3
11 0
21 1
100 1
样例输出
0
数据规模和约定
0<S<=10000,0<N<=1000,每一个a和b满足0<a<=1000且b=0或1。
*/
#include<stdlib.h> 
#include<iostream>
using namespace std;

struct Nodis {
	double prize;
	int index;
}goods[1005];

struct dis {
	double prize;
	int index;
}goods_dis[1005];

int buy[1005];

int comp1(const void *a, const void *b) {
	//对于非半价处理的物品，赚的钱相等，越便宜优先度越高
	if ((*(Nodis*)a).prize != (*(Nodis *)b).prize)
		//一个大坑，因为qsort规定了排序函数的返回值只能是int，对double数组的排序必须类似三目运算符的方法判断！
		return (*(Nodis *)a).prize > (*(Nodis *)b).prize ? 1 : -1; 
	//价格相同，列表里靠前的优先度高
	else return (*(Nodis *)a).index - (*(Nodis *)b).index;
}

int comp2(const void *a, const void *b) {
	//对于半价处理的物品，价格越贵，赚得越多，优先度越高
	if ((*(dis*)a).prize != (*(dis *)b).prize)
		return (*(dis *)b).prize > (*(dis *)a).prize ? 1 : -1;
	//价格相同，赚的也相同，列表里靠前的优先度高
	else return (*(dis *)a).index - (*(dis *)b).index;
}
int comp(const void *a, const void *b) {   
	return *(int *)a - *(int *)b;
}

int main() {
	int N, IsHalf, j = 0, k = 0;   //j是打折的商品的个数，k是不打折的商品个数
	double prize, money;
	cin >> money >> N;
	for (int i = 1; i <= N; i++) {
		cin >> prize >> IsHalf;
		if (IsHalf == 1) {
			goods_dis[j].prize = prize / 2.0;
			goods_dis[j].index = i;
			j++;
		}
		else {
			goods[k].prize = prize;
			goods[k].index = i;
			k++;
		}
	}

	qsort(goods, k, sizeof(goods[0]), comp1);
	qsort(goods_dis, j, sizeof(goods_dis[0]), comp2);

	int count = 0;
	//先检查打折的商品能否购买
	for (int i = 0; i < j; i++) {
		if (money - goods_dis[i].prize >= 0) {
			buy[count++] = goods_dis[i].index;
			money -= goods_dis[i].prize;
		}
	}

	//再检查不打折的商品能否购买
	for (int i = 0; i < k; i++) {
		if (money - goods[i].prize >= 0) {
			buy[count++] = goods[i].index;
			money -= goods[i].prize;
		}
		else break;
	}

	if (count == 0) cout << '0';
	else {
		qsort(buy, count, sizeof(buy[0]), comp);
		for (int i = 0; i < count; i++) {
			if (i == 0) cout << buy[i];
			else cout << ' ' << buy[i];
		}
	}

	return 0;
}