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
    class Task {
    public:
        function<void(void)> func;

        explicit Task(function<void(void)> func);
    };

    class ThreadPool {
    public:
        pthread_mutex_t lock;
        pthread_cond_t ready;
        int maxThread;
        bool shutdown = false;
        queue<Task *> *threads = new queue<Task *>();
        pthread_t * id;
        explicit ThreadPool(int max);
        bool isEmpty();

        Task * waitingNext();
        void shutdownPool();
        void addTask(function<void(void)> &func);
    };


    void * threadRun(void * args);
}


#endif //LAMBDATHREADPOOL_LAMBDATHREADPOOL_H
