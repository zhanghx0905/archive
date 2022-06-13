#pragma once
#include <list>
#include <memory>
#include<iostream>
using namespace std;

class MyList
{
private:
	std::shared_ptr< std::list<int> > pt; // a pointer to the real container
	std::list<int>::iterator left, right; // the position of slice is [left, right). 'left' is included, 'right' is excluded.
	bool mark = false;
	std::list<int>::iterator forward(int pos) const{
		// count from 'left', find the element at position 'pos'.
		auto now = left;
		while(pos--) now++;
		return now;
	}

public:
	MyList(): pt(new std::list<int>()){
		left = pt->begin();
		right = pt->end();
		// Actually, left = right = pt->end(), because there is no element in the list.
	}

	void append(int i) {
		if (!mark) {
			right = pt->insert(left, i);
		}
		else { right = pt->insert(right, i); }
		right++;
	}

	int& operator[](int pos){
		if (!mark) {
			left = pt->begin();
		}
		return *forward(pos); // access the element at the 'pos'
	}

	friend ostream& operator<< (ostream& out, const MyList& src) {
		out << "[";
		if (src.left != src.right) {
			typename std::list<int>::iterator now = src.left;
			out << *now;
			now++;
			for (; now != src.right; now++) {
				out << "," << *now;
			}
		}
		out << "]";
		return out;
	}
	MyList operator() (int a, int b){
		MyList* tmp = new MyList;
		tmp->pt = pt;
		tmp->left = forward(a);
		tmp->right = forward(b);
		tmp->mark = true;
		return *tmp;
	}
};