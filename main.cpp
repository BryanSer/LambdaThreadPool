#include <iostream>
#include <string>
#include "lambdathreadpool.cpp"
#include <functional>

using namespace std;
using namespace bryanser;
Channel<int *> intchannel;

int main() {
    ThreadPool *tp = new ThreadPool(2);
    function<void()> t1 = []() {
        while (true) {
            int **i = intchannel.receive();
            if(i == NULL){
                break;
            }
            cout << "t1 receive: " << **i << endl;
            delete *i;
        }
        cout << "channel was closed" << endl;
    };
    function<void()> t2 = []() {
        for (int t = 0; t < 10; t++) {
            int *i = new int(t);
            cout << "t2 send: " << *i << endl;
            intchannel.send(i);
        }
        usleep(100);
        intchannel.close();
    };
    tp->addTask(t1);
    tp->addTask(t2);
    sleep(1);
    tp->shutdownPool();
    delete tp;
    return 0;
}