#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<iostream>
#include <string>
#include <cstring>
#include"temperature.h"
#define min(a,b) (a<b)?a:b
#define max(a,b) (a>b)?a:b

using namespace std;
int n, num;
const int maxn = 50001;
long long record;
struct D {//data
	int x, y, temp;
}Data[maxn];

struct Node {//kd树结点
	long long sum;//offspring_sum
	int num;//offspring_num
	int X[2], Y[2];//range of offspring
	Node* l, *r;
	D* data;//左右子节点, 数据节点
	Node(D* _d) :data(_d), l(nullptr), r(nullptr) {
		sum = _d->temp; num = 1;
		X[0] = X[1] = _d->x;
		Y[0] = Y[1] = _d->y;
	};
};
//flag=1 按照x选取中位数，否则选y
void SelectMedian(int low, int high, bool flag) {//修改后的快速选择
	int k = (low + high) >> 1;

	while (low < high) {
		int i = low, j = high;	
		D pivot = Data[i];
		if (flag) {
			while (i < j) {
				while (i < j && pivot.x <= Data[j].x) j--; Data[i] = Data[j];
				while (i < j && Data[i].x <= pivot.x)i++; Data[j] = Data[i];
			}

		}
		else {
			while (i < j) {
				while (i < j && pivot.y <= Data[j].y) j--; Data[i] = Data[j];
				while (i < j && Data[i].y <= pivot.y)i++; Data[j] = Data[i];
			}
		}
		Data[i] = pivot;
		if (k <= j) high = i - 1;
		if (i <= k) low = i + 1;
	}
}

Node* buildKdTree(int lo, int hi, int d) {
	if (lo > hi) return nullptr;
	//若深度为偶数则按照x选择中位数，否则按y
	bool flag = (!d) ? true : false;
	SelectMedian(lo, hi, flag);
	int mid = (lo + hi) >> 1;
	Node* root = new Node(&Data[mid]);

	root->l = buildKdTree(lo, mid - 1, d ^ 1);
	root->r = buildKdTree(mid + 1, hi, d ^ 1);
	return root;
}

void Query(Node*root, int x1, int y1, int x2, int y2, int d) {
	int x = root->data->x, y = root->data->y;

	if (x1 <= root->X[0] && root->X[1] <= x2 && y1 <= root->Y[0] && root->Y[1] <= y2) {
		record += root->sum;
		num += root->num;
		return;
	}
	if (x1 <= x && x <= x2 && y1 <= y && y <= y2) {
		record += root->data->temp;
		num++;
	}
	if (d) {//如果深度为奇数，比较y
		if (root->l&&y1 <= y)
			Query(root->l, x1, y1, x2, y2, d ^ 1);
		if (root->r&&y <= y2)
			Query(root->r, x1, y1, x2, y2, d ^ 1);
	}
	else {//否则比较x
		if (root->l&&x1 <= x)
			Query(root->l, x1, y1, x2, y2, d ^ 1);
		if (root->r&&x <= x2)
			Query(root->r, x1, y1, x2, y2, d ^ 1);
	}
}
void Maintain(Node* root) {
	if (root->l) {
		Maintain(root->l);
		root->sum += root->l->sum;
		root->num += root->l->num;
		root->X[0] = min(root->l->X[0], root->X[0]); root->X[1] = max(root->l->X[1], root->X[1]);
		root->Y[0] = min(root->l->Y[0], root->Y[0]); root->Y[1] = max(root->l->Y[1], root->Y[1]);
	}
	if (root->r) {
		Maintain(root->r);
		root->sum += root->r->sum;
		root->num += root->r->num;
		root->X[0] = min(root->r->X[0], root->X[0]); root->X[1] = max(root->r->X[1], root->X[1]);
		root->Y[0] = min(root->r->Y[0], root->Y[0]); root->Y[1] = max(root->r->Y[1], root->Y[1]);
	}
}
int main() {
	n = GetNumOfStation();
	for (int i = 0; i < n; i++) {
		GetStationInfo(i, &Data[i].x, &Data[i].y, &Data[i].temp);
	}
	Node*root = buildKdTree(0, n - 1, 0);
	Maintain(root);
	int x1, x2, y1, y2;
	while (GetQuery(&x1, &y1, &x2, &y2)) {
		record = num = 0;
		Query(root, x1, y1, x2, y2, 0);
		if (!num) Response(0);
		else {
			Response(record / num);
		}
	}
	return 0;
}
