//
// Created by Administrator on 2022-10-13.
//

#include <mutex>
#include <condition_variable>
#include <iostream>

/***
* 读写锁（readers-writer lock），又称为多读单写锁（multi-reader single-writer lock，或者MRSW lock），共享互斥锁（shared-exclusive lock），以下简称RW lock。
读写锁用来解决读写操作并发的问题。多个线程可以并行读取数据，但只能独占式地写或修改数据。

write-mode和read-mode
RW lock有两种模式：write-mode，read-mode。

write-mode
在write-mode下，一个writer取得RW lock。当writer写数据时，其他所有writer或reader将阻塞，直到该writer完成写操作；
read-mode
在read-mode下，至少一个reader取得RW lock。当reader读数据时，其他reader也能同时读取数据，但writer将阻塞，直到所有reader完成读操作;
RW lock升级与降级
当writer取得RW lock，进入write-mode，对数据进行写操作时，进入read-mode进行读操作。我们把这个称为锁降级（downgraded RW lock）。
当reader取得RW lock，进入read-mode，对数据进行读操作时，进入write-mode进行写操作。我们把这个称为锁升级（upgradable RW lock）。
锁降级是安全的；而锁升级是不安全的，容易造成死锁，应当避免。
 #include <pthread.h>
int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);    销毁RW lock
int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock,const pthread_rwlockattr_t *restrict attr); 初始化RW lock
pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;   直接赋值方式初始化RW lock
int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);    取得读锁，进入read-mode
int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock); 尝试取得读锁，失败立即返回
int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock); 取得写锁，进入write-mode
int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);    尝试取得写锁，失败立即返回
int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);     释放读/写锁
*/

class RWLock {
public:
    RWLock() : rw_nwaitreaders(0), rw_nwaitwriters(0), rw_refcount(0) { }
    ~RWLock() = default;
    RWLock(const RWLock&) = delete;
    RWLock& operator=(const RWLock&) = delete;

public:
    void rdlock();    /* wait for reader lock */
    bool tryrdlock(); /* try to get reader lock */
    void wrlock();    /* wait for writer lock */
    bool trywrlock(); /* try to get writer lock */
    void unlock();    /* release reader or writer lock */

private:
    std::mutex rw_mutex;
    std::condition_variable_any rw_condreaders;
    std::condition_variable_any rw_condwriters;
    int rw_nwaitreaders;                        /* the number of waiting readers */
    int rw_nwaitwriters;                        /* the number of waiting writers */
    int rw_refcount; /* 0: not locked; -1: locked by one writer; > 0: locked by rw_refcount readers */
};
// 阻塞获取读锁
void RWLock::rdlock()
{
    rw_mutex.lock();
    {
        /* give preference to waiting writers */
        while (rw_refcount < 0 || rw_nwaitwriters > 0) { // 写优先
            rw_nwaitreaders++;
            rw_condreaders.wait(rw_mutex);
            rw_nwaitreaders--;
        }
        rw_refcount++;  /* another reader has a read lock */
    }
    rw_mutex.unlock();
}
// 尝试获取读锁，失败立即返回
bool RWLock::tryrdlock()
{
    bool res = true;
    rw_mutex.lock();
    {
        if (rw_refcount < 0 || rw_nwaitwriters > 0) { // 写优先
            res = false; /* held by a writer or waiting writers */
        }
        else {
            rw_refcount++; /* increment count of reader locks */
        }
    }
    rw_mutex.unlock();
    return res;
}
// 阻塞获取写锁
void RWLock::wrlock()
{
    rw_mutex.lock();
    {
        while (rw_refcount != 0) { /* wait other readers release the rd or wr lock */
            rw_nwaitwriters++;
            rw_condwriters.wait(rw_mutex);
            rw_nwaitwriters--;
        }
        rw_refcount = -1; /* acquire the wr lock */
    }
    rw_mutex.unlock();
}
// 尝试获取写锁，失败立即返回
bool RWLock::trywrlock()
{
    bool res = true;
    rw_mutex.lock();
    {
        if (rw_refcount != 0) /* the lock is busy */
            res = false;
        else
            rw_refcount = -1; /* acquire the wr lock */
    }
    rw_mutex.unlock();
    return res;
}
// 释放写锁或读锁
void RWLock::unlock()
{
    rw_mutex.lock();
    {
        if (rw_refcount > 0)
            rw_refcount--;
        else if (rw_refcount == -1)
            rw_refcount = 0;
        else
            // unexpected error
            fprintf(stderr, "RWLock::unlock unexpected error. rw_refcount = %d\n", rw_refcount);

        /* give preference to waiting writers over waiting readers */
        if (rw_nwaitwriters > 0) {
            if (rw_refcount == 0) {
                rw_condwriters.notify_one();
            }
        }
        else if (rw_nwaitreaders > 0) {
            rw_condreaders.notify_all(); /* rw lock is shared */
        }
    }
    rw_mutex.unlock();
}

#include <thread>
#include <mutex>
#include <iostream>
#include <vector>
using namespace std;

volatile int v = 0;
RWLock rwlock;

void WriteFunc()
{
    this_thread::sleep_for(chrono::milliseconds(10)); // 为了演示效果，先让write线程休眠10ms
    rwlock.wrlock();
    {
        v++;
        cout << "Write:" << v << endl;
    }
    rwlock.unlock();
}

void ReadFunc()
{
    rwlock.rdlock();
    {
        cout << "Read:" << v << endl;
    }
    rwlock.unlock();
}

void test_rwlock()
{
    vector<thread> writers;
    vector<thread> readers;

    for (int i = 0; i < 20; ++i) {
        writers.push_back(thread(WriteFunc));
    }
    for (int i = 0; i < 200; ++i) {
        readers.push_back(thread(ReadFunc));
    }

    for (auto & e : writers) {
        e.join();
    }
    for (auto & e : readers) {
        e.join();
    }

    getchar();
}

int main(){
    test_rwlock();
}