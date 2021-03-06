#include<iostream>
#include<cstring>
using namespace std;

struct Student {
	int   id;           // 学号
	char  name[13];     // 姓名
	char  dept[32];     // 院系
	char  group[10];    // 班级
	char  type[10];     // 类别
	char  email[58];    // 邮箱
	long long cell;     // 手机
};

struct LinkNode {
	LinkNode * next;
	LinkNode * prev;
	Student  * stu;
};

LinkNode * initLinkedList() {
	LinkNode * phead = new LinkNode;
	LinkNode * ptail = new LinkNode;

	phead->next = ptail;
	phead->prev = NULL;
	phead->stu = NULL;

	ptail->next = NULL;
	ptail->prev = phead;
	ptail->stu = NULL;

	return phead;
}

void insertionSort(LinkNode * phead)  //插入排序 
{
	LinkNode * TravDown;
	LinkNode * TravUp;
	int j = 1;
	while (true) {
		j++;
		TravDown = phead;
		for (int k = 1; k <= j; k++) //TravDown指向第j号结点
			TravDown = TravDown->next;
		if (TravDown->next) {   //如果TravDown不是尾哨兵，执行排序
			TravUp = TravDown->prev;
			while (j > 1 && strcmp(TravUp->stu->group, TravDown->stu->group) > 0) //TravUp向上遍历已经排好的序列[1,j-1]，找到比TravDown小的最靠前结点
			{																	  //若这样的结点不存在，TravUp置为头哨兵
				TravUp = TravUp->prev;
				j--;
			}
			TravDown->prev->next = TravDown->next; //将TravDown插入到TravUp后面
			TravDown->next->prev = TravDown->prev;
			TravDown->next = TravUp->next;
			TravDown->prev = TravUp;
			TravUp->next->prev = TravDown;
			TravUp->next = TravDown;
		}
		else break; //如果TravDown是尾哨兵，排序结束
	}
}

int readStructsIntoList(LinkNode * phead) {
	int length = 0;
	char buffer[256];
	Student* ta = new Student;
	if (gets_s(buffer)) {
		length++;
		LinkNode * newnode = initLinkedList();
		sscanf(buffer, "%d%10s%31s%9s%9s%57s%I64d",
			&ta->id,
			ta->name,
			ta->dept,
			ta->group,
			ta->type,
			ta->email,
			&ta->cell);
		newnode->stu = ta;
		newnode->prev = phead;
		phead->next = newnode;
		readStructsIntoList(newnode);//递归
	}
	return length;
}

void writeListIntoStructs(LinkNode * phead)  //顺序输出结点信息 
{
	if (phead->next != NULL)
	{
		LinkNode * tmp = phead->next;
		Student* ta = tmp->stu;
		cout << ta->id
			<< "\t" << ta->name
			<< "\t" << ta->dept
			<< "\t" << ta->group
			<< "\t" << ta->type
			<< "\t" << ta->email
			<< "\t" << ta->cell << "\n";
		if (tmp->next->next != NULL)
			writeListIntoStructs(tmp);
	}
}

void releaseLinkList(LinkNode * phead) {
	while (phead->next) {
		LinkNode* pta = phead;
		Student* ta = phead->stu;
		phead = phead->next;
		delete pta; delete ta;
	}
	delete phead; //删除了尾指针
}

int main(int argc, char* argv[]) {
	LinkNode * phead = initLinkedList(); // 初始化链表
	readStructsIntoList(phead);          // 从文件roster.txt中读取数据
	insertionSort(phead);                // 插入排序
	writeListIntoStructs(phead);         // 将排序后的结果写到文件roster_sorted.txt中，格式同roster.txt
	releaseLinkList(phead);              // 释放链表
	return 0;
}