#include <list>
#include <queue>
#include <memory>
#include <thread>
#include <cstring>
#include <algorithm>
#include "util.h"
using namespace std;

chrono::milliseconds dura(500);

enum TaskType {
    NONE = 0, INSERT, REMOVE, FIND, OUTPUT
};

struct Task {
    TaskType type;
    int val;
};

Task genTask() {
    int samp = rand() % 15, val = rand() % 10;
    TaskType type;
    if (0 <= samp && samp <= 3)
        type = INSERT;
    else if (4 <= samp && samp <= 5)
        type = REMOVE;
    else if (6 <= samp && samp <= 13)
        type = FIND;
    else type = OUTPUT; // samp == 14
    return { type,val };
}

void runTask(list<int>& lis, const Task& task, bool par = true) {
    // control output
    char hint[10];
    if (par) 
        strcpy(hint, "par:");
    else
        strcpy(hint, "ser:"); 

    auto iter = find(lis.begin(), lis.end(), task.val);
    switch (task.type) {
    case INSERT:
        lis.push_back(task.val);
        printf("%s insert %d\n", hint, task.val);
        break;
    case OUTPUT:
        printf("%s output list: ", hint);
        for (int v : lis) {
            printf("%d ", v);
        }
        printf("\n");
        break;
    case FIND:
        if (iter == lis.end()) {
            printf("%s find %d fail\n",hint, task.val);
        }
        else {
            printf("%s find %d succeed\n",hint, task.val);
        }
        break;
    case REMOVE:
        if (iter == lis.end()) {
            printf("%s remove %d fail\n",hint, task.val);
        }
        else {
            lis.erase(iter);
            printf("%s remove %d succeed\n",hint, task.val);
        }
        break;
    default:
        break;
    }
}

struct TaskQueue {
    unique_ptr<pthread_t[]> threads;
    uint thread_cnt;
    queue<Task> q;
    list<int> lis;

    bool finished;
    Mutex mu;
    CondVar cond;
    TaskQueue(uint n) : threads(new pthread_t[n]), thread_cnt(n), finished(false) {
        for (uint i = 0; i < n; i++) {
            pthread_create(&threads[i], nullptr, run, this);
        }
    }
    ~TaskQueue() {
        finished = true;
        cond.wake_all();
        for (uint i = 0; i < thread_cnt; i++) {
            pthread_join(threads[i], nullptr);
        }
    }

    static void* run(void* _this) {
        TaskQueue* p = (TaskQueue*)_this;
        while (true) {
            {
                Locker<Mutex> lock(&p->mu);
                while (p->q.empty() && !p->finished) {
                    p->cond.wait(&p->mu);
                }
                if (p->q.empty()) {
                    break;
                }
                Task task = p->q.front();
                p->q.pop();
                runTask(p->lis, task);
            }
            this_thread::sleep_for(dura);
        }
        return nullptr;
    }

    void pushTask(Task obj) {
        Locker<Mutex> lock(&mu);
        q.push(obj);
        cond.wake_one();
    }

};

int main(int argc, char* argv[]) {
    setbuf(stdout,NULL);
    if (argc != 3) {
        printf("usage: %s [thread num] [task num]\n", argv[0]);
        exit(1);
    }
    srand((uint)time(nullptr));
    uint thread_cnt = atoi(argv[1]), task_cnt = atoi(argv[2]);
    unique_ptr<Task[]> tasks(new Task[task_cnt]);
    for (uint i = 0; i < task_cnt; i++) {
        tasks[i] = genTask();
    }

    list<int> ser_lis;
    TaskQueue task_queue(thread_cnt);

    for (uint i = 0; i < task_cnt; i++) {
        runTask(ser_lis, tasks[i], false);
        task_queue.pushTask(tasks[i]);
    }
    return 0;
}