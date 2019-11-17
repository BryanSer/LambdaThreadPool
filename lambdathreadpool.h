//
// Created by root on 2019/11/17.
//
#include <unistd.h>
#include <pthread.h>
#include <cassert>
#include <functional>
#include <queue>

#define null NULL
#ifndef LAMBDATHREADPOOL_LAMBDATHREADPOOL_H
#define LAMBDATHREADPOOL_LAMBDATHREADPOOL_H
using namespace std;
namespace bryanser {
    class Thread {
    public:
        function<void(void)> func;

        explicit Thread(function<void(void)> func);
    };

    class ThreadPool {
    public:
        pthread_mutex_t lock;
        pthread_cond_t ready;
        int maxThread;
        bool shutdown = false;
        queue<Thread *> *threads = new queue<Thread *>();
        pthread_t * id;
        explicit ThreadPool(int max);
        bool isEmpty();

        Thread * waitingNext();
        void shutdownPool();
        void addThread(function<void(void)> &func);
    };


    void * threadRun(void * args);
}


#endif //LAMBDATHREADPOOL_LAMBDATHREADPOOL_H
