//
// Created by Administrator on 2022-10-5.
//

#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <deque>
#include "pthread.h"
#include <ctime>
#include <vector>
#include <thread>
#include <atomic>


std::deque<int> q;
std::mutex mutex;
int counter = 0;
std::condition_variable cond;
std::atomic<size_t> count(0);



void testFunc()
{
    //lock_guard 互斥锁 作用域内上锁
    std::lock_guard<std::mutex> lockGuard(mutex);

    //函数体
    counter++;
} //函数结束时，作用域结束，自动释放

void function_1() {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "I'm function_1()" << std::endl;
}

void function_2() {
    int count = 10;
    while (count > 0)
    {
        std::unique_lock<std::mutex> locker(mutex);
        q.push_front(count);
        locker.unlock();
        cond.notify_one();  // Notify one waiting thread, if there is one.
        std::this_thread::sleep_for(std::chrono::seconds(1));
        count--;
    }
}

/***
 * 在管理互斥锁的时候，使用的是std::unique_lock而不是std::lock_guard，wait()函数会先调用互斥锁的unlock()函数，然后再将自己睡眠，在被唤醒后，
 * 又会继续持有锁，保护后面的队列操作。lock_guard没有lock和unlock接口，而unique_lock提供了，这就是必须使用unique_lock的原因；
 */
void function_3() {
    int data = 0;
    while (data != 1)
    {
        std::unique_lock<std::mutex> locker(mutex);
        while (q.empty())
            cond.wait(locker); // Unlock mu and wait to be notified
        data = q.back();
        q.pop_back();
        locker.unlock();
        std::cout << "t2 got a value from t1: " << data << std::endl;
    }
}

void threadFun()
{
    for (int i = 0; i < 10000; i++)
        count++;
}


int main() {
    std::thread t1(function_1);
//    t1.detach(); //detach 相当于 守护线程的概念 一旦一个线程被分离了，就不能够再被join了。
     t1.join(); //主线程在这里阻塞 等待子线程执行完毕 谁join 谁要先执行完
    std::cout << "test() finished" << std::endl;

    testFunc();
    std::cout <<"counter = "<<counter <<std::endl;

    //演示条件锁
    std::thread t2(function_2);
    std::thread t3(function_3);
    t2.join();
    t3.join();


    clock_t start_time = clock();

    // 启动多个线程
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; i++)
        threads.emplace_back(threadFun);
    for (auto&thad : threads)
        thad.join();

    // 检测count是否正确 10000*10 = 100000
    std::cout << "count number:" << count << std::endl;

    clock_t end_time = clock();
    std::cout << "耗时：" << end_time - start_time << "ms" << std::endl;

    return 0;
}