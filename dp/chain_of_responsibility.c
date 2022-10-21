//
// Created by Administrator on 2022-10-14.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "list.h"

/*
 责任链
 责任链将需要触发的对象组成一条链，发送者将请求发给链的第一个接收者，
 并且沿着这条链传递，直到有一个对象来处理它或者直到最后也没有对象处理而留在链末尾端.
 责任链在C语言里也是实现形式非常明显的模式。
 最典型的责任链有linux内核的中断处理机制的纯软件部分和内核网络netfiler的HOOK机制。
 这两者均强化了责任链机制，重点在引入了责任优先级方法和增加了通过/终结两种处理结果。
 责任链模式的最重要的数据结构是handler链表。
 事件发生时，handler链表上的回调函数会被以此调用。
 优先级决定了那个handler会被先调，哪些会被后调用。
 在扩展特性里，每个handler可以有不处理和处理完之后继续交给下一个handler两种选择。
 如果该事件最后没有被消费，会有一个异常处理函数。如果责任链上任意一个handler消费了事件，
 那么就不传给下一个handler，直接结束。
 逻辑上和责任链模式最相近的一个设计模式为观察者模式。
 流程图如下。观察者模式和责任链模式的最大的差别在于，事件会被通知到每一个平等的handler，而不是逐级处理。
 也不存在优先级的说法，也不会出现事件没有处理需要异常函数收尾。
*/

#define CHAIN_PASS 0
#define CHAIN_STOP 1
typedef int(*chain_func)(char*buf);

struct chain_ops_node
{
    struct list_head list; //内核链表标准结构
    chain_func handler; //handler的回调函数
    int priority; //优先级
};

//全局的责任链
struct list_head chain_global_list;

//具体的处理函数
int chain_handler1(char* buf)
{
    //do something
    printf("enter chain handler1.\n");
    if (1)
    {
        return CHAIN_PASS;
    }
    return CHAIN_STOP;
}

int chain_handler2(char* buf)
{
    //do something
    printf("enter chain handler2.\n");
    if (0)
    {
        return CHAIN_PASS;
    }
    return CHAIN_STOP;
}

//封装成节点
struct chain_ops_node node1;
struct chain_ops_node node2;

void chain_mode1_init()
{
    node1.handler = chain_handler1;
    node1.priority = 0;

    //
    node2.handler = chain_handler2;
    node2.priority = 1;

    //init list
    init_list_head(&chain_global_list);
}

/*
特别注意，一般是需要信号量锁定的，
因为很可能链条上的数据正在执行。内核里喜欢用rcu锁，可以避免资源互斥引起cpu浪费。
*/
int chain_register(struct chain_ops_node* node)
{
    //lock chain_global_list
    //add node into chain_global_list according to priority
    //unlock chain_global_list
    list_add_tail(&node->list, &chain_global_list);
    printf("chain register.\n");
    return 0;
}

int chain_unregister(struct chain_ops_node* node)
{
    //lock chain_global_list
    //delete node into chain_global_list
    //unlock chain_global_list
    list_del(&node->list);
    printf("chain unregister.\n");
    return 0;
}


int main()
{
    struct list_head* node;
    struct chain_ops_node* node_entry;
    char buf[16];
    chain_mode1_init();
    chain_register(&node1);
    chain_register(&node2);

    //something happned,deal with data, should trigger resposibity chain
    //fill buf with event

    LIST_FOR_EACH(node, &chain_global_list)
    {
        node_entry = list_entry(node,struct chain_ops_node, list);
        if (node_entry->handler(buf) == CHAIN_STOP)
        {
            printf("find node,break.\n");
            break;
        }
    }
    getchar();
    return 0;
}