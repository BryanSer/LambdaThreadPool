//
// Created by root on 2019/11/17.
//

#include "lambdathreadpool.h"

#include <utility>

namespace bryanser {
    Thread::Thread(function<void(void)> func) : func(std::move(func)) {

    }

    bool ThreadPool::isEmpty() {
        return threads->empty();
    }

    ThreadPool::ThreadPool(int max) : maxThread(max) {
        pthread_mutex_init(&(this->lock), null);
        pthread_cond_init(&(this->ready), null);
        id = new pthread_t[max];

        for (int i = 0; i < max; i++) {
            pthread_create(&(this->id[i]), null, threadRun, this);
        }
    }

    Thread *ThreadPool::waitingNext() {
        pthread_mutex_lock(&(this->lock));
        while (this->isEmpty() && !this->shutdown) {
            pthread_cond_wait(&(this->ready), &(this->lock));
        }
        if (this->shutdown) {
            pthread_mutex_unlock(&(this->lock));
            pthread_exit(null);
        }
        Thread *t = this->threads->front();
        this->threads->pop();
        pthread_mutex_unlock(&(this->lock));
        return t;
    }

    void ThreadPool::addThread(function<void(void)> &func) {
        pthread_mutex_lock(&(this->lock));
        Thread *t = new Thread(func);
        this->threads->push(t);
        pthread_mutex_unlock(&(this->lock));
        pthread_cond_signal(&(this->ready));
    }

    void ThreadPool::stop() {
        if (this->shutdown) {
            return;
        }
        this->shutdown = true;
        pthread_cond_broadcast(&(this->ready));
        for (int i = 0; i < this->maxThread; i++) {
            pthread_join(this->id[i], null);
        }
        delete [] this->id;
        while(!this->isEmpty()){
            Thread * t = this->threads->front();
            this->threads->pop();
            delete t;
        }
        delete this->threads;
    }


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

    void *threadRun(void *args) {
        ThreadPool *pool = (ThreadPool *) args;
        while (true) {
            Thread *t = pool->waitingNext();
            if (t != null) {
                t->func();
            }
        }
        pthread_exit(null);
    }

#pragma clang diagnostic pop
}