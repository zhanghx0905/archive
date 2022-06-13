#include "Node.h"
#include <vector>
using std::vector;
class Array {
	vector<Node> num;
public:
	Array(int len) {
		num.resize(len);
	}
	Node& operator[](int n) {
		return num[n];
	}
	void update(int q, int r) {
		num[q] = num[r];
	}
	void swap(int q, int r) {
		std::swap(num[q], num[r]);
	}
	void insert(int q, int r) {
		for (int i = num.size()-1; i >q;--i) {
			num[i] = std::move(num[i - 1]);
		}
		num[q] = r;
	}
};