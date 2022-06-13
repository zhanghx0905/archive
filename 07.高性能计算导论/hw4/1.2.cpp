#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "pthreadVC2.lib")
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <pthread.h>
using namespace std;

pthread_mutex_t mu;
bool message_available;
char message[100];

void* fn(void* _tid) {
    int my_rank = (int)_tid;
    while (true) {
        pthread_mutex_lock(&mu);
        if (my_rank % 2) { // consumer
            if (message_available) {
                printf("Tid %d get message: %s\n", my_rank, message);
                message_available = false;
                pthread_mutex_unlock(&mu);
                break;
            }
        }
        else if (!message_available) { //producer
            sprintf(message, "message from tid %d", my_rank);
            message_available = true;
            pthread_mutex_unlock(&mu);
            break;
        }
        pthread_mutex_unlock(&mu);
    }
    return nullptr;
}

int main(int argc, char* argv[]) {
    int thread_cnt = atoi(argv[1]);
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