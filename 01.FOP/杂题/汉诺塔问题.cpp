#include <iostream>
using namespace std;

int coun = 0;//不能命名count，与namespace冲突
void hanoi(unsigned n, char a, char b, char c) {
	if (n == 1) {
		cout << 1 << ':' << a << "->" << c << endl;
		coun++;
	}
	else {
		hanoi(n - 1, a, c, b);
		cout << n << ':' << a << "->" << c << endl;
		hanoi(n - 1, b, a, c);
		coun++;
	}
}

int main() {
	int n;
	cin >> n;
	hanoi(n, 'a', 'b', 'c');
	cout << "共运行了" << coun << "次";
	return 0;
}