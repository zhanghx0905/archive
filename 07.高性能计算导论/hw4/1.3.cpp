#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "pthreadVC2.lib")
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <pthread.h>
using namespace std;

pthread_mutex_t mu;
bool msg_available;
char message[100];
int recv_rank, thread_cnt;

void* fn(void* _tid) {
    int my_rank = (int)_tid;
    bool send = false, recv = false; //�ֲ�������¼ÿ���߳̽������
    while (!send | !recv) {
        pthread_mutex_lock(&mu);
        if (msg_available) { // ����Ƿ���Խ���
            if (!recv && recv_rank == my_rank) {
                printf("Tid %d get message: %s\n", my_rank, message);
                msg_available = false;
                recv = true;
            }
        }
        else if (!send) { // ����Ƿ���Է���
            sprintf(message, "message from tid %d", my_rank);
            msg_available = true;
            send = true;
            recv_rank = (my_rank + 1) % thread_cnt;
        }
        pthread_mutex_unlock(&mu);
    }
    return nullptr;
}
int main(int argc, char* argv[]) {
    thread_cnt = atoi(argv[1]);
    unique_ptr<pthread_t[]> threads(new pthread_t[thread_cnt]);
    pthread_mutex_init(&mu, nullptr);
    for (int i = 0; i < thread_cnt; i++) {
        pthread_create(&threads[i], nullptr, fn, (void*)i);
    }

    for (int i = 0; i < thread_cnt; i++) {
        pthread_join(threads[i], nullptr);
    }

    return 0;
}