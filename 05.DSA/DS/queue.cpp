const int maxn = 1e5;

template<typename T>
class Queue { //队列
	int first, last;
	T arr[maxn];
public:
	Queue(int n) : first(0), last(0) {}
	void push(T n) {
		arr[last++] = n;
	}
	T& front() {
		return arr[first];
	}
	void pop() {
		first++;
	}
	bool empty() {
		return (first == last);
	}
};

//单链表
struct queueNode {
	queueNode* next;
	int id;
	queueNode(int _id) :
		id(_id), next(nullptr) {}
	queueNode() :
		queueNode(-1) {}
};
struct queue {
	queueNode* first;//头哨兵
	queue() {
		first = new queueNode;
	}
	int frontId() {
		return first->next->id;
	}
	void push(int id) {
		queueNode* tmp = first;
		while (tmp->next) tmp = tmp->next;
		tmp->next = new queueNode(id);
	}
	void pop() {
		if (first->next)
			first->next = first->next->next;
	}
	bool empty() {
		if (first->next) return true;
		return false;
	}
};