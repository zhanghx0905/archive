#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<cstring>
using namespace std;

template<typename T>
class Stack {
	int capaticy, size;
	T* data;
public:
	Stack(int n) :capaticy(n), size(0) {
		data = new T[n];
	}
	Stack(const Stack& obj) :capaticy(obj.capaticy), size(obj.size) {
		data = new T[capaticy];
		for (int i = 0; i < size; i++)
			data[i] = obj.data[i];
	}
	Stack operator=(const Stack& obj) {
		return Stack(obj);
	}
	~Stack() {
		if (data) delete[] data;
	}
	T top() {
		return data[size - 1];
	}
	void push(T t) {
		data[size++] = t;
	}
	void pop() {
		if (!empty())
			size--;
	}
	bool empty() {
		return (!size);
	}
	int getSize() {
		return size;
	}
};
const int maxn = 1600001;

Stack<int>Q(maxn), P(maxn);

int way_count = 0;//全局计数器
bool way[2 * maxn]; //0表示push，1表示pop
int main()
{
	int n, m;
	scanf("%d %d", &n, &m);
	for (int i = n; i >= 1; i--)
		Q.push(i);

	bool flag = true;
	for (int i = 0; i < n; i++)
	{
		int tmp; scanf("%d", &tmp);
		if (!flag)
			continue;

		if (P.empty())
		{
			if (Q.empty())
			{
				flag = false;
				continue;
			}
			P.push(Q.top());
			Q.pop();
			way[way_count++] = 0;
			if (P.getSize() > m) {
				flag = false;
				continue;
			}
		}
		while (!P.empty())
		{
			if (P.top() == tmp)
			{
				P.pop();
				way[way_count++] = 1;
				break;
			}
			else
			{
				if (Q.empty())
				{
					flag = false;
					break;
				}
				P.push(Q.top());
				Q.pop();
				way[way_count++] = 0;
			}
			if (P.getSize() > m) {
				flag = false;
				break;
			}
		}

	}
	if (!flag)
		printf("No\n");
	else {
		for (int i = 0; i < way_count; i++) {
			const char* tmp = (way[i]) ? "pop" : "push";
			printf("%s\n", tmp);
		}
	}
}