const int maxn = 1e5;
template<typename T>
class Deque { //双端队列
	int first, last;
	T arr[maxn];
public:
	Queue() : first(0), last(0) {}
	void push(T n) {
		arr[last++] = n;
	}
	T& front() {
		return arr[first];
	}
	T& back() {
		return arr[last - 1];
	}
	T pop_front() {
		return arr[first++];
	}
	T pop_back() {
		return arr[--last];
	}
	bool empty() {
		return (first == last);
	}
};

template<typename T>
class Queap {//单调队列
	Queue<T> q, mins;
public:
	Queap() : q(), mins() {}
	void push(T n) {
		q.push(n);
		while (!mins.empty() && n < mins.back())
			mins.pop_back();
		mins.push(n);
	}
	T pop() {
		T value = q.pop_front();
		if (value == mins.front())
			mins.pop_front();
		return value;
	}
	T getMin() {
		return mins.front();
	}
	T& front() {
		return q.front();
	}
	bool empty() { return q.empty(); }

};