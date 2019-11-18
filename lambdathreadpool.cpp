//
// Created by root on 2019/11/17.
//

#include "lambdathreadpool.h"

#include <utility>

namespace bryanser {
    Task::Task(function<void(void)> func) : func(std::move(func)) {

    }

    bool ThreadPool::isEmpty() {
        return threads->empty();
    }

    ThreadPool::ThreadPool(int max) : maxThread(max) {
        pthread_mutex_init(&(this->lock), null);
        pthread_cond_init(&(this->ready), null);
        id = new pthread_t[max];

        for (int i = 0; i < max; i++) {
            pthread_create(&(this->id[i]), null, threadRun, this);// 创建线程并绑定函数threadRun
        }
    }

    Task *ThreadPool::waitingNext() {
        pthread_mutex_lock(&(this->lock));// 使调用本函数的线程获得锁
        while (this->isEmpty() && !this->shutdown) {// 如果没有要等待执行的任务则等待唤醒
            pthread_cond_wait(&(this->ready), &(this->lock));
        }
        if (this->shutdown) {// 若线程池已经被关闭则退出
            pthread_mutex_unlock(&(this->lock));
            pthread_exit(null);
        }
        Task *t = this->threads->front();// 从队列中读取要执行的任务
        this->threads->pop();// 弹出顶部任务
        pthread_mutex_unlock(&(this->lock));//解锁
        return t;
    }

    void ThreadPool::addTask(function<void(void)> &func) {
        pthread_mutex_lock(&(this->lock));// 获得线程锁
        Task *t = new Task(func);
        this->threads->push(t);
        pthread_mutex_unlock(&(this->lock));
        pthread_cond_signal(&(this->ready));
    }

    void ThreadPool::shutdownPool() {
        if (this->shutdown) {
            return;
        }
        this->shutdown = true;
        pthread_cond_broadcast(&(this->ready));// 广播信号 使得线程能够终止
        for (int i = 0; i < this->maxThread; i++) {
            pthread_join(this->id[i], null);// 等待未完成的线程完成
        }
        delete[] this->id;
        while (!this->isEmpty()) {// 清理队列
            Task *t = this->threads->front();
            this->threads->pop();
            delete t;
        }
        delete this->threads;
        pthread_mutex_destroy(&(this->lock));
        pthread_cond_destroy(&(this->ready));
    }


    template<class T>
    Channel<T>::Channel() {
        sem_init(&(this->sem), 0, 0);
        pthread_mutex_init(&(this->data_lock), null);
    }

    template<class T>
    void Channel<T>::send(T &t) {
        if(isClose){
            return;
        }
        pthread_mutex_lock(&(this->data_lock));
        this->data->push(t);
        pthread_mutex_unlock(&(this->data_lock));
        sem_post(&(this->sem));
    }

    template<class T>
    T *Channel<T>::receive() {
        if(isClose){
            return null;
        }
        sem_wait(&(this->sem));
        if(isClose){
            return null;
        }
        pthread_mutex_lock(&(this->data_lock));
        T &t = this->data->front();
        this->data->pop();
        pthread_mutex_unlock(&(this->data_lock));
        return &t;
    }

    template<class T>
    Channel<T>::~Channel() {
        delete data;
        sem_destroy(&(sem));
        pthread_mutex_destroy(&(this->data_lock));
    }

    template<class T>
    void Channel<T>::close() {
        if(isClose){
            return;
        }
        isClose = true;
        sem_post(&(this->sem));
    }

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

    // 负责线程池的线程运行
    // 参数为这个线程所属的线程池指针
    void *threadRun(void *args) {
        ThreadPool *pool = (ThreadPool *) args;
        while (true) {
            Task *t = pool->waitingNext();// 从线程池中读取下一个要执行的任务(阻塞方式)
            if (t != null) {
                t->func();// 执行lambda
            }
        }
        pthread_exit(null);
    }

#pragma clang diagnostic pop
}