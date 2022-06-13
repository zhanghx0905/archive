#include "BasicContainer.h"
#include <map>
#include<algorithm>
#include<vector>
using namespace std;

template<class A>
struct Point {
	Pos pos;
	A value;
	Point(Pos p, A v) : pos(p), value(v) {}
	const bool operator < (const Point<A> &b) const { return this->pos < b.pos; }
	const bool operator < (const Pos& b) const { return this->pos < b; }

};

template<class A, class C>
class Container : public BasicContainer<A> {
public:
	void insert(const Pos &p, const A &v) {};
	A* find(Pos p) { return NULL; };
};


template <class A>
class Container<A, vector<A> > : public BasicContainer<A> {
	std::vector< Point<A> > base;
	bool isSorted = 0;
public:
	void insert(const Pos &p, const A &v) {
		base.push_back(Point<A>(p, v));
		isSorted = 0;
	}

	A* find(Pos p) {
		if (!isSorted) {
			isSorted = 0;
			std::sort(base.begin(), base.end());
		}
		auto it = std::lower_bound(base.begin(), base.end(), p);
		if (it == base.end() || it->pos != p)
			return NULL;
		else
			return &(it->value);
	}
};


template <class A>
class Container<A, map<Pos, A> > : public BasicContainer<A> {
	std::map<Pos, A> base;
public:
	void insert(const Pos &p, const A &v) {
		base[p] = v;
	};

	A* find(Pos p) {
		auto it = base.find(p);
		if (it != base.end()) {
			return &(it->second);
		}
		else
			return NULL;
	}

};