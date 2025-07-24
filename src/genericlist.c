// gcc -o .\obj\genericlist.o -Iinclude -c .\src\genericlist.c
#include "genericlist.h"

// 创建链表
GenericList *create_genericList(size_t data_size)
{
    GenericList *genericList = NULL;
    genericList = (GenericList *)malloc(sizeof(GenericList));
    if (genericList == NULL)
    {        
        printf("    [系统]>链表控制结点内存申请失败！\n");
        return NULL;
    }
    genericList->head = NULL;
    genericList->head = (GenericNode *)malloc(sizeof(GenericNode));
    if (genericList->head == NULL)
    {
        free(genericList);
        printf("    [系统]>链表头结点结点内存申请失败！\n");
        return NULL;
    }
    genericList->data_size = data_size;  // 数据大小初始化为data_size
    genericList->count = 0;  // 节点计数器初始化为0

    genericList->head->data = NULL;
    genericList->head->prior = genericList->head;  // 头结点的前驱指针指向自己
    genericList->head->next = genericList->head;  // 头结点的后继指针指向自己

    return genericList;
}

/*
@ function_name: sort_genericList
@ argument：
@       GenericList *genericList：链表控制结构体指针
@       void *data：传入进行比较的数据
@       int n   ：控制回调函数的数据转换选择
@       int (*isEqual_func)(void *, void *, int) ：比较功能函数的函数指针
@ function: 根据data查找结点
@ renturn value：GenericNode *：找到对应结点返回其地址，否则为NULL
@ author: wang.
@ modify_time: 2025/3/28
*/
GenericNode *find_genericlist(GenericList *genericList, void *data, const int n
    , int (*isEqual_func)(void *, void *, const int))
{
    if(genericList->head == genericList->head->next)
    {
        return NULL;
    }
    GenericNode *current = genericList->head->next;

    while(genericList->head != current)
    {
        if( isEqual_func(current->data, data, n) )
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

/*
@ function_name: insert_genericList
@ argument: 
@       GenericList *genericList：链表控制结构体指针
@       GenericNode *genericNode：指定链表中的结点
@       void *data              ：插入节点的数据指针
@ function: 向链表指定结点前添加结点
@ renturn value: int [-1]：结点内存分配失败 [-2]：数据内存分配失败
@ author: wang.
@ modify_time: 2025/3/28
*/
int insert_genericList(GenericList *genericList, GenericNode *genericNode, void *data)
{
    GenericNode *newNode = NULL;
    newNode = (GenericNode *)malloc(sizeof( GenericNode ));
    if(NULL == newNode )
    {
        printf("    [系统]>结点内存分配失败！！！\n");
        return -1;
    }
    newNode->data = NULL;
    newNode->data = malloc(genericList->data_size);
    if(NULL == newNode->data)
    {
        printf("    [系统]>数据内存分配失败！！！\n");
        free(newNode);
        return -2;
    }
    memcpy(newNode->data, data, genericList->data_size);

    genericNode->prior->next = newNode;
    newNode->next = genericNode;
    newNode->prior = genericNode->prior;
    genericNode->prior = newNode;

    genericList->count++;
    return 0;
}

/*
@ function_name: remove_genericList
@ argument: 
@       GenericList *genericList：链表控制结构体指针
@       GenericNode *genericNode：指定链表中的结点
@ function: 从链表中删除指定结点
@ renturn value: void
@ author: wang.
@ modify_time: 2025/3/28
*/
void remove_genericList(GenericList *genericList, GenericNode *genericNode)
{
    genericNode->prior->next = genericNode->next;
    genericNode->next->prior = genericNode->prior;

    free(genericNode->data);
    free(genericNode);
    genericList->count--;
}

/*
@ function_name: traverse_genericList
@ argument: 
@       GenericList *genericList：链表控制结构体指针
@       int flag：[1]、正向遍历；[-1]、逆向遍历
@       void (*print_func)(void *， int)：输出函数
@ function: 遍历输出通用链表
@ renturn value: int [-1]：链表为空或未创建
@ author: wang.
@ modify_time: 2025/3/28
*/
int traverse_genericList(GenericList *genericList, int flag, void (*print_func)(void *))
{
    if(NULL == genericList || genericList->head == genericList->head->next)
    {
        return -1;
    }
    if( 1 == flag)  // 正序遍历
    {
        GenericNode *current = genericList->head->next;
        while(genericList->head != current)
        {
            print_func(current->data);
            current = current->next; 
        }
    }
    else if(-1 == flag) // 逆序遍历
    {
        GenericNode *current = genericList->head->prior;
        while(genericList->head != current)
        {
            print_func(current->data);
            current = current->prior; 
        }
    }

}

/*
@ function_name: destroy_genericList
@ argument：GenericList *genericList：链表控制结构体指针
@ function: 销毁链表，包括链表控制结构体
@ renturn value: void
@ author: wang.
@ modify_time: 2025/3/28
*/
void destroy_genericList(GenericList *genericList)
{
    GenericNode *temp = NULL;
    while(genericList->head != genericList->head->next)
    {
        temp = genericList->head->next;
        genericList->head->next = temp->next;
        free(temp->data);
        free(temp);
        temp = NULL;
    }

    free(genericList->head);
    free(genericList);
    genericList = NULL;
}

// 要求结点A必须由结点B通过前驱遍历找到，且遍历时不经过头结点的位置
void swarp_genericNode(GenericNode **nodeA, GenericNode **nodeB)
{
    if((*nodeA)->next == (*nodeB) || (*nodeB)->prior == (*nodeA))
    {               
        // printf("[Step]>Swap: A is adjacent B\n");        
        GenericNode *nodeAprior = (*nodeA)->prior;
        GenericNode *nodeBnext = (*nodeB)->next;

        nodeAprior->next = (*nodeB);
        (*nodeB)->next = (*nodeA);
        (*nodeA)->next = nodeBnext;

        nodeBnext->prior = (*nodeA);
        (*nodeA)->prior = (*nodeB);
        (*nodeB)->prior = nodeAprior;
        
        (*nodeA) = (*nodeA)->prior;
        (*nodeB) = (*nodeB)->next;
    }
    else
    {
        // printf("[Step]>Swap: A is not adjacent B\n");
        GenericNode *nodeAprior = (*nodeA)->prior;
        GenericNode *nodeAnext = (*nodeA)->next;
        GenericNode *nodeBprior = (*nodeB)->prior;
        GenericNode *nodeBnext = (*nodeB)->next;

        nodeAprior->next = (*nodeB);
        (*nodeB)->next = nodeAnext;
        nodeAnext->prior = (*nodeB);
        (*nodeB)->prior = nodeAprior;

        nodeBprior->next = (*nodeA);
        (*nodeA)->next = nodeBnext;
        nodeBnext->prior = (*nodeA);
        (*nodeA)->prior = nodeBprior;

        (*nodeA) = nodeAprior->next;
        (*nodeB) = nodeBprior->next;
    }
}

/*
@ function_name: partition_genericList
@ argument：
@       GenericNode *low    ：正向遍历靠近首元结点的一端
@       GenericNode *high   ：正向遍历远离首元结点的一端
@       int n   ：控制回调函数的数据转换选择
@       int (*compare_func)(void *, void *, int) ：比较功能函数的函数指针
@ function: 在快速排序中进行逆序、正序排序，确定枢轴的位置并将其返回
@ renturn value: GenericNode *  ：返回枢轴所在位置的地址
@ author: wang.
@ modify_time: 2025/3/28
*/
GenericNode *partition_genericList(GenericNode *low, GenericNode *high
    , const int n, int (*compare_func)(void *, void *, const int))
{
    GenericNode *x = low;
    while(low != high)
    {
        // printf("[Step]>Partition:H\n");
        // 逆序比较
        while (low != high && compare_func(high->data, x->data, n))
        {
            high = high->prior;
        }
        // 如果low != hgih 则需要进行交换
        if(low != high)
        {
            // printf("[Step]>Partition:HS\n");
            swarp_genericNode(&low, &high);
            low = low->next;
        }
        // printf("[Step]>Partition:L\n");
        // 正序比较
        while(low != high && compare_func(x->data, low->data, n))
        {
            low = low->next;
        }
        // 如果low != hgih 则需要进行交换
        if(low != high)
        {
            // printf("[Step]>Partition:LS\n");
            swarp_genericNode(&low, &high);
            high = high->prior;
        }
    }
    return low;  // 返回已经排好序的位置
}

/*
@ function_name: sort_genericList
@ argument：
@       GenericNode *reLow  ：用于在一组逆序、正序排序后重新确定low的位置
@       GenericNode *reHigh ：用于在一组逆序、正序排序后重新确定high的位置
@       GenericNode *low    ：正向遍历靠近首元结点的一端
@       GenericNode *high   ：正向遍历远离首元结点的一端
@       int n   ：控制回调函数的数据转换选择
@       int (*compare_func)(void *, void *, int) ：比较功能函数的函数指针
@ function: 
@           靠近头节点设为低地址，靠近为节点设为高地址
@           参与排序的结点为头和尾之间的所有节点，
@           参与递归排序的节点为枢轴与低地址或高地址之间的结点
@           直到枢轴与低地址或高地址结点相邻或重合
@ renturn value: void
@ author: wang.
@ modify_time: 2025/3/28
*/
void sort_genericList(GenericNode *reLow, GenericNode *reHigh
    , GenericNode *low, GenericNode *high, const int n, int (*compare_func)(void *, void *, const int))
{
    GenericNode *temp = partition_genericList(low, high, n, compare_func);

    low = reLow->next;
    high = reHigh->prior;

    if(low != temp && low != temp->prior)
    {
        // printf("[Step]>Sort:L\n");
        sort_genericList(reLow, temp, low, temp->prior, n, compare_func);
    }
    if(high != temp && high != temp->next)
    {
        // printf("[Step]>Sort:R\n");
        sort_genericList(temp, reHigh, temp->next, high, n, compare_func);
    }
}

/*
@ function_name: print_genericList_data
@ argument: 
@       GenericList *userList：链表控制结构体指针
@       void *data      ：进行比较的数据
@       int n   ：控制回调函数的数据转换类型及比较项
@ function: 遍历输出data筛选的数据
@ renturn value:void
@ author: wang.
@ modify_time: 2025/4/2
*/
int print_genericList_data(GenericList *genericList, void *data, const int n
    , int (*isEqual_func)(void *, void *, const int), void (*print_func)(void *)  // 用于比较的回调函数
    , void (*print_genericList_headLine)(void), void (*print_genericList_tailLine)(void)) 
{
    if(genericList->head == genericList->head->next)
    {
        return -1;
    }
    GenericNode *current = genericList->head->next; 
    int flag = 1;
    while(genericList->head != current)
    {
        if( isEqual_func(current->data, data, n) )
        {
            if(flag)  // 首次进入输出表头
            {
                print_genericList_headLine();
                flag = 0;
            }
            print_func(current->data);
        }
        current = current->next;
    }

    if(0 == flag)
    {
        print_genericList_tailLine();
        return 0;
    }
    else  // 表头未输出说明没有匹配的数据，不输出表尾
    {
        return -2;
    }
}

/*
@ function_name: print_genericList_onlyData
@ argument: 
@       GenericList *userList：链表控制结构体指针
@       void *data      ：进行比较的数据
@       int n   ：控制回调函数的数据转换类型比较项
@ function: 遍历输出data筛选的数据,（数据以表格形式输出时使用）
@ renturn value:void
@ author: wang.
@ modify_time: 2025/4/2
*/
int print_genericList_onlyData(GenericList *genericList, void *data, const int n
    , int (*isEqual_func)(void *, void *, const int), void (*print_func)(void *))
{
    if(genericList->head == genericList->head->next)
    {
        return -1;
    }
    GenericNode *current = genericList->head->next; 
    int flag = 0;
    while(genericList->head != current)
    {
        if( isEqual_func(current->data, data, n) )
        {
            print_func(current->data);
            flag = 1;
        }
        current = current->next;
    }

    if(1 == flag)
    {
        return 0;
    }
    else  // 未输出任何信息说明没有匹配的数据
    {
        return -2;
    }
}

// // 功能测试成功
// void print_int(void *data, int n)
// {
//     int *intData = (int *)data;
//     printf("[Data]:%d\n", *intData);
// }

// int isEqual_int(void *m, void *n, int n)
// {
//     return *( (int *)m ) == *( (int *)n );
// }

// int compare_int(void *m, void *n, int n)
// {
//     return *( (int *)m ) >= *( (int *)n );
// }

// /*
//     gcc -o .\bin\gl .\src\genericlist.c .\src\datafile.c -Iinclude
//     .\bin\gl
// */
// int main(int argc, char **argv)
// {
//     // 测试初始化、插入、输出
//     GenericList *intList = create_genericList(sizeof(int));
//     for(int i  = 10; i > 0 ; i--)
//     {
//         insert_genericList(intList, intList->head, &i);
//     }
    
//     traverse_genericList(intList, 1, 0, print_int);
//     printf("\n");

//     // 测试查找、插入、删除、输出
//     int n = 192;
//     int m = 5;
//     GenericNode *temp =  find_genericlist(intList, &m, 0, isEqual_int);
//     if(NULL != temp)
//     {
//         insert_genericList(intList, temp, &n);
//         insert_genericList(intList, intList->head->next, &m);
//         remove_genericList(intList, temp);
//         temp = NULL;
//     }
//     else
//     {
//         printf("Search failed!\n");
//     }
//     traverse_genericList(intList, 1, 0, print_int);
//     printf("\n");

//     // 测试排序、输出
//     sort_genericList(intList->head, intList->head
//         , intList->head->next, intList->head->prior, 0, compare_int);
//     traverse_genericList(intList, 1, print_int);
//     printf("\n");

//     // 测试销毁、输出
//     destroy_genericList(intList);
//     intList = NULL;
//     if(-1 == traverse_genericList(intList, 1, 0, print_int))
//     {
//         printf("Delete succeed!\n");
//     }
//     else
//     {
//         printf("Delete failed!\n");
//     }

//     return 0;
// }