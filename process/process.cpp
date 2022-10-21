//
// Created by Administrator on 2022-10-20.
//
#include<unistd.h>
#include <cstdio>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <unistd.h>

int test();
int base();
int test_vfork();
int test_CLONE();

//fork 读时共享，写时复制

int main(int argc, char** argv) {
//    execv("/usr/bin/ls", argv);
//    base();
//    test();
//    test_vfork();
    test_CLONE();
}

//思考子进程获取了父进程的什么信息 fork创造的子进程是父进程的完整副本，复制了父亲进程的资源，包括内存的内容task_struct内容
int test(){
    std::string message;
    int n;
    pid_t pid = fork();
    if (pid == 0)
    {
        // 新创建的子进程
        {
            message = "child !!!";
            n = 6;
            printf("child : PID = %d, ParentID = %d\n", getpid(), getppid());
            sleep(1);
        }
    }

    else if (pid > 0)
    {
        // 当前进程
        {
            message = "parent !!!";
            n = 3;
            printf("parent: PID = %d, ParentID = %d\n", getpid(), getppid());
            sleep(1);
        }
    }

    for (int i=0;i<n;i++){
        printf("%s", message.c_str());
        sleep(1);
    }

    return 0 ;
}

int base(){
    pid_t pid = fork();
    if (pid == 0)
    {
        // 新创建的子进程
        while (1)
        {
            printf("child : PID = %d, ParentID = %d\n", getpid(), getppid());
            sleep(1);
        }
    }

    else if (pid > 0)
    {
        // 当前进程
        while (1)
        {
            printf("parent: PID = %d, ParentID = %d\n", getpid(), getppid());
            sleep(1);
        }
    }
    return 0;
}

//vfork创建的子进程与父进程共享数据段,而且由vfork()创建的子进程将先于父进程运行.
//        由vfork创造出来的子进程还会导致父进程挂起，除非子进程exit或者execve才会唤起父进程
//
//        由vfok创建出来的子进程共享了父进程的所有内存，包括栈地址，直至子进程使用execve启动新的应用程序为止
//
//        由vfork创建出来得子进程不应该使用return返回调用者，或者使用exit()退出，但是它可以使用_exit()函数来退出
int test_vfork(){

        int count = 1;
        int child;

        printf("Before create son, the father's count is : %d\n", count);

        if((child = vfork())< 0)
        {
            perror("fork error : ");
        }
        else if(child == 0)     //  fork return 0 in the child process because child can get hid PID by getpid( )
        {
            printf("This is son, his count is: %d (%p). and his pid is: %d\n", ++count, &count, getpid());

        }
        else                    //  the PID of the child process is returned in the parent’s thread of execution
        {
            printf("After son, This is father, his count is: %d (%p), his pid is: %d\n", count, &count, getpid());
            sleep(2);
        }
    //https://linux.die.net/man/2/vfork
//        return EXIT_SUCCESS;
        exit(0);
}
#define FIBER_STACK 8192
#include <stdio.h>
#include <malloc.h>
#include <sched.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
int a;
void * stack;
int do_something()
{
    printf("This is son, the pid is:%d, the a is: %d\n", getpid(), ++a);
    free(stack);
    exit(1);
}

int test_CLONE(){
    //int clone(int (fn)(void ), void *child_stack, int flags, void *arg);
//    CLONE_PARENT	创建的子进程的父进程是调用者的父进程，新进程与创建它的进程成了“兄弟”而不是“父子”
//    CLONE_FS	子进程与父进程共享相同的文件系统，包括root、当前目录、umask
//    CLONE_FILES	子进程与父进程共享相同的文件描述符（file descriptor）表
//    CLONE_NEWNS	在新的namespace启动子进程，namespace描述了进程的文件hierarchy
//    CLONE_SIGHAND	子进程与父进程共享相同的信号处理（signal handler）表
//    CLONE_PTRACE	若父进程被trace，子进程也被trace
//    CLONE_VFORK	父进程被挂起，直至子进程释放虚拟内存资源
//    CLONE_VM	子进程与父进程运行于相同的内存空间
//    CLONE_PID	子进程在创建时PID与父进程一致
//    CLONE_THREAD	Linux 2.4中增加以支持POSIX线程标准，子进程与父进程共享相同的线程群
    void * stack;
    a = 1;
    stack = malloc(FIBER_STACK);//为子进程申请系统堆栈

    if(!stack)
    {
        printf("The stack failed\n");
        exit(0);
    }
    printf("creating son thread!!!\n");

    clone(reinterpret_cast<int (*)(void *)>(&do_something), (char *)stack + FIBER_STACK, CLONE_VM | CLONE_VFORK, 0);//创建子线程

    printf("This is father, my pid is: %d, the a is: %d\n", getpid(), a);
    exit(1);

}

