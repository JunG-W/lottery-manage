/*
@ file_name: genericlist.h
@ function: 
@ 实现循环双向通用链表及相关操作，包括插入，查找，删除，修改，排序，输出等功能；
@ author: wang.
@ modify_time: 2025/4/2
*/
#ifndef __GENERICLIST_H__
#define __GENERICLIST_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 节点结构
typedef struct genericNode
{
    void *data;  // 通用数据指针
    struct genericNode *prior;  // 前驱指针
    struct genericNode *next;  // 后继指针
}GenericNode;

// 链表控制结构
typedef struct genericList
{
    GenericNode *head;  // 头结点
    size_t data_size;  // 数据大小
    size_t count;  // 节点计数器
}GenericList;

// 创建链表
GenericList *create_genericList(size_t data_size);

// 查找结点
GenericNode *find_genericlist(GenericList *genericList
    , void *data, int n, int (*isEqual_func)(void *, void *, const int));

// 向链表指定结点前添加结点
int insert_genericList(GenericList *genericList, GenericNode *genericNode, void *data);

// 从链表中删除指定结点
void remove_genericList(GenericList *genericList, GenericNode *genericNode);

// 遍历链表
int traverse_genericList(GenericList *genericList
    , int flag, void (*print_func)(void *));

// 销毁链表
void destroy_genericList(GenericList *genericList);

// 排序
void sort_genericList(GenericNode *reLow, GenericNode *reHigh
    , GenericNode *low, GenericNode *high, const int n, int (*compare_func)(void *, void *, const int));

int print_genericList_data(GenericList *genericList, void *data, const int n
    , int (*isEqual_func)(void *, void *, const int), void (*print_func)(void *)  // 用于比较的回调函数
    , void (*print_genericList_headLine)(void), void (*print_genericList_tailLine)(void));  // 输出表头和表尾

int print_genericList_onlyData(GenericList *genericList, void *data, const int n
    , int (*isEqual_func)(void *, void *, const int), void (*print_func)(void *));

#endif
