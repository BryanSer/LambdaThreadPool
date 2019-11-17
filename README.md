# LambdaThreadPool
通过lambda的方式往线程池里添加任务

## 使用方法:
```c++
#include "lambdathreadpool.cpp"
#include <functional>
// 创建线程池对象
// 如果有需要的话可以创建多个线程池对象 用来管理
ThreadPool * tp = new ThreadPool(10);// 构造器参数为线程池内执行的线程数量
// 假设这是一个往线程池中追加任务的方法
void add(){
    function<void()> func = [](){cout << "hello world" << endl;};// 创建执行用的lambda
    tp->addTask(func);// 追加任务
    // 这个任务将会在线程池空闲时执行
}
```
## 作者
BryanSer: bryan_lzh@njust.edu.cn
本项目作为linux程序设计课程作业 代码为个人原创