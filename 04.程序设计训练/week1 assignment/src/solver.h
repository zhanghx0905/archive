#ifndef SOLVER_H
#define SOLVER_H

#include <QVector>
#include <QSet>

typedef QPair<int, int> Corrd;

struct Point {
    Corrd pos;
    int pre = -1;
};

template<typename T>
struct Queue { //简易队列
    int first, last;
    T* arr;
    Queue(int n) : first(0), last(0) {
        arr = new T[n];
    }
    ~Queue() { if (arr)delete[]arr; }
    void push(T n) {
        arr[last++] = n;
    }
    T& front() {
        return arr[first];
    }
    int pop() {
        first++;
        return first - 1;
    }
    bool empty() {
        return (first == last);
    }
};

class Solver
{
    QSet<Corrd> UnReach;
    Point pos1, pos2;//从(x1,y1)到(x2,y2)
    int x_max, y_max;//行列数;
public:
    QList<Corrd> route;
    Solver(Corrd _pos1, Corrd _pos2, const QSet<Corrd>& _u, int x_m, int y_m);
    bool BFS();
};


#endif // SOLVER_H
