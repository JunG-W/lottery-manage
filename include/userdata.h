/*
@ file_name: userdata.h
@ function: 
@ 实现彩票系统中彩民结构体的建立及相关基本等功能；
@ author: wang.
@ modify_time: ------------------------------------------------------------------------------------------------------
*/
#ifndef __USERDATA_H__
#define __USERDATA_H__

#include "publicdata.h"
#include "datafile.h"

/***********************************************************************************************************/
//  外层彩民的链表操作
/***********************************************************************************************************/
//优先完成通用链表的回调函数 
int isEqual_user(void *userData, void *data, int n);
int compare_user(void *userDataA, void *userDataB, const int n);
void print_user(void *userData);

// 输出表头 // 输出表尾
void print_userList_head(void);
void print_userList_tail(void);
void print_userNode(GenericNode *userNode);
void print_userList(GenericList *userList, int n);
// 根据输入数据批量输出信息
void print_userList_data(GenericList *userList, void *data, int n);

void get_uaer_id(GenericList *userList, User *userData);
int input_uaer_name(GenericList *userList, User *userData);
int input_user_password(User *userData);
int input_user_data(User *userData);

int updata_user_balance(GenericNode *userNode, const int n);

GenericNode *login(GenericList *userList);

/***********************************************************************************************************/
//  内层购买信息的链表操作
/***********************************************************************************************************/
//优先完成通用链表的回调函数
int isEqual_purch_info(void *purch_infoData, void *data, int n);
int compare_purch_info(void *purch_infoDataA, void *purch_infoDataB, const int n);
void print_purch_info(void *purch_infoData);

void print_purch_infoListl(GenericList *purch_infoList, int n);
void print_purch_infoList_data(GenericList *purch_infoList, void *data, int n);

void update_purch_info(GenericList *purch_infoList);

/***********************************************************************************************************/
//  内外层链表的公共操作部分
/***********************************************************************************************************/
void enroll_userNode(GenericList *userList);

#endif
