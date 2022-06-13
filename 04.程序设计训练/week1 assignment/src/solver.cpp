#include "solver.h"

Solver::Solver(Corrd _pos1, Corrd _pos2, const QSet<Corrd> &_u, int x_m, int y_m) :
    UnReach(_u), x_max(x_m), y_max(y_m) {
    pos1.pos = _pos1; pos2.pos = _pos2;
}

bool Solver::BFS()
{
    Queue<Point> q(x_max*y_max);
    bool mark = false;

    q.push(pos1);
    UnReach.insert(pos1.pos);
    while (!q.empty()) {
        Point tmp = q.front(), next[4];
        int p=q.pop();
        if (tmp.pos == pos2.pos) {
            pos2.pre = p;
            mark = true;
            break;
        }

        next[0].pos = QPair<int, int>(tmp.pos.first - 1, tmp.pos.second);
        next[1].pos = QPair<int, int>(tmp.pos.first + 1, tmp.pos.second);
        next[2].pos = QPair<int, int>(tmp.pos.first, tmp.pos.second - 1);
        next[3].pos = QPair<int, int>(tmp.pos.first, tmp.pos.second + 1);
        for (int i = 0; i < 4; i++)
            next[i].pre = p;
        if (next[0].pos.first >= 1 && !UnReach.contains(next[0].pos)) {
            q.push(next[0]);
            UnReach.insert(next[0].pos);
        }
        if (next[1].pos.first <= x_max && !UnReach.contains(next[1].pos)) {
            q.push(next[1]);
            UnReach.insert(next[1].pos);
        }
        if (next[2].pos.second >= 1 && !UnReach.contains(next[2].pos)) {
            q.push(next[2]);
            UnReach.insert(next[2].pos);
        }
        if (next[3].pos.second <= y_max && !UnReach.contains(next[3].pos)) {
            q.push(next[3]);
            UnReach.insert(next[3].pos);
        }
    }
    if (!mark) return false;//empty

    int iter = pos2.pre;
    while (iter!=-1) {
        route.push_front(q.arr[iter].pos);
        iter = q.arr[iter].pre;
    }

    return true;
}
