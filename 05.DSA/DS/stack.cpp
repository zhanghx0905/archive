
const int maxn = 1e5;
template<typename T>
struct Stack {
	int size;
	T data[maxn];
	Stack(int sentry = 0) : size(0) {
		data[0] = sentry;//设置哨兵
	}
	~Stack() {
		if (data) delete[] data;
	}
	T& top() {
		return data[size];
	}
	void push(T t) {
		data[++size] = t;
	}
	T pop() {
		return data[(empty()) ? (size) : (size--)];
	}
	bool empty() {
		return (!size);
	}
};

template<typename T>
class Staap {//单调堆
	Stack<T> data, maxs;
public:
	Staap(int size) :data(size), maxs(size, -1) {}
	void push(T x) {
		data.push(x);
		maxs.push((x > maxs.top()) ? x : maxs.top());
	}
	bool empty() { return data.empty(); }
	T pop() {
		maxs.pop();
		return data.pop();
	}
	T& max() { return maxs.top(); }
};