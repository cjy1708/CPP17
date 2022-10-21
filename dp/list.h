//
// Created by Administrator on 2022-10-14.
//

#ifndef C17_LIST_H
#define C17_LIST_H


#pragma once


typedef struct list_head
{
    struct list_head* prev;
    struct list_head* next;
} ;

#define LIST_FOR_EACH(pos, head) \
	for(pos=(head)->next;pos!=(head);pos=pos->next)

#define LIST_HEAD_INIT(name) {&(name), &(name)}

static  inline void init_list_head(struct list_head* list)
{
    list->next = list;
    list->prev = list;
}


static inline void __list_add(struct list_head* new_node, struct list_head* prev, struct list_head* next)
{
    next->prev = new_node;
    new_node->next = next;
    new_node->prev = prev;
    prev->next = new_node;
}

/*
new_node增加在head后面
如:
head->A_node
增加B_node
head->B_node->A_node
*/
static inline void list_add(struct list_head* new_node, struct list_head* head)
{
    __list_add(new_node, head, head->next);
}

/*
new_node增加在head的尾部
如:
head->A_node
增加B_node
head->A_node->B_node
*/
static inline void list_add_tail(struct list_head* new_node, struct list_head* head)
{
    __list_add(new_node, head->prev, head);
}

static inline void __list_del(struct list_head* prev, struct list_head* next)
{
    next->prev = prev;
    prev->next = next;
}

static inline void list_del(struct list_head* entry)
{
    __list_del(entry->prev, entry->next);
    entry->next = NULL;
    entry->prev = NULL;
}

#define list_entry(ptr, type, member) \
	((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))


#endif //C17_LIST_H
