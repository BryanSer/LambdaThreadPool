#include <iostream>
#include <string>
#include "lambdathreadpool.cpp"
#include <functional>
using namespace std;
using namespace bryanser;

int main() {
    ThreadPool *tp = new ThreadPool(5);
    for (int i = 0; i < 100; i++) {
        function<void()> f = [=]() {
            cout << "test"  << i << endl;
        };
        tp->addThread(f);
    }
    sleep(1);
    tp->shutdownPool();
    delete tp;
    return 0;
}