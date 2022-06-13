#include <stdio.h>
#include <iostream>
#include <time.h>
#include <math.h>
using namespace std;

struct student {
	int   id;
	char  name[13]; //padding = 16 - 13 = 3 if exchanged with id
	char  dept[32];
	char  group[10];
	char  type[10];
	char  email[58]; //padding = 6 = 64 - 58
	long long cell; //2^32 < 10^10 < 2^64
	unsigned int  pts; //grade >= 0
	student* next; //ignored unless used as a list node
};

void print(student & s) { //record called by REFERENCE
	cout << s.id << "\n"
		<< s.name << "\n"
		<< s.dept << "\n"
		<< s.group << "\n"
		<< s.type << "\n"
		<< s.email << "\n"
		<< s.cell << "\n"
		<< s.pts << "\n\n";
}

int readStructsIntoList(student* & stu) {
	char buffer[256];
	int k = 0;
	for (; gets(buffer); k++) {
		student* ta = new student;
		sscanf(buffer, "%d%12s%31s%9s%9s%57s%I64d",
			&ta->id,
			ta->name,
			ta->dept,
			ta->group,
			ta->type,
			ta->email,
			&ta->cell);
		ta->pts = (int)(sqrt((25 + rand() % 75) * 100.0) + 0.5); //a random grade within (50, 100]
		ta->next = stu; stu = ta;
	}
	return k;
}

void selectionsort(student* s) { //sort by grade
	for (student* ta = s; ta; ta = ta->next) {
		student* tm = ta;
		for (student* tb = ta; tb; tb = tb->next)
			if (tm->pts > tb->pts) tm = tb;
		swap(*ta, *tm); swap(ta->next, tm->next);
	}
}

void bubblesort(student* s) { //sort by group: STABLE!
	for (bool sorted = false; sorted = !sorted; )
		for (student* ta = s; student* tb = ta->next; ta = ta->next)
			if (strcmp(ta->group, tb->group) > 0) { //what if ">= 0" used instead?
				sorted = false;
				swap(*ta, *tb); swap(ta->next, tb->next);
			}
}

void insertionsort(student* & s) { //sort by group: REVERSED stable
	for (student* t = s; student* x = t->next; ) { //repeat inserting [x] into the sorted prefix = [s, t]
		if (strcmp(x->group, s->group) <= 0) { //[x] <= [s]
			t->next = x->next; x->next = s; s = x; //insert [x] as the predecessor of [s]
		}
		else {
			student* p = s; //determine the predecessor of x
			while (strcmp(x->group, p->next->group) > 0) p = p->next; //by a scan
			if (p->next == x) //[s, t] < [x]
				t = x;
			else {
				t->next = x->next; x->next = p->next; p->next = x; //insert [x] as the successor of [p]
			}
		}
	}
}

int main(int argc, char* argv[]) {

	srand((unsigned int)time(NULL));

	student* fcpStu = NULL;
	int n = readStructsIntoList(fcpStu); cout << n << " record(s) read\n";

	bubblesort(fcpStu); cout << "\nBubblesorted by group\n";
	for (student* ta = fcpStu; ta; ta = ta->next) {
		cout << "[" << --n << "]\t" << ta->name << "\t" << ta->group << "\t" << ta->pts << "\n";
	}
	selectionsort(fcpStu); cout << "\nSelectionsorted by grade\n";
	for (student* ta = fcpStu; ta; ta = ta->next) {
		cout << "[" << n++ << "]\t" << ta->name << "\t" << ta->group << "\t" << ta->pts << "\n";
	}
	insertionsort(fcpStu); cout << "\nInsertionsorted by group\n";
	for (student* ta = fcpStu; ta; ta = ta->next) {
		cout << "[" << --n << "]\t" << ta->name << "\t" << ta->group << "\t" << ta->pts << "\n";
	}

	while (fcpStu) {
		student* ta = fcpStu; fcpStu = fcpStu->next; delete ta;
	}

	return 0;
}