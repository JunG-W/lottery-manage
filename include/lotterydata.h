/*
@ file_name: lotterydata.h
@ function: 
@ 实现彩票系统中彩票中心结构体的建立及相关基本等功能；
@ author: wang.
@ modify_time: ------------------------------------------------------------------------------------------------------
*/
#ifndef __LOTTERYDATA_H__
#define __LOTTERYDATA_H__

#include "publicdata.h"
#include "datafile.h"

/***********************************************************************************************************/
//  外层彩民的链表操作
/***********************************************************************************************************/
//优先完成通用链表的回调函数 
int isEqual_lottery(void *lotteryData, void *data, int n);
int compare_lottery(void *lotteryDataA, void *lotteryDataB, const int n);
int compare_lottery_date(void *lotteryData, void *dateData, const int n);  // 专门用于比较结点时间与给出时间大小
void print_lottery(void *userData);

// 输出表头 // 输出表尾
void print_lotteryList_head(void);
void print_lotteryList_tail(void);
void print_lotteryNode(GenericNode *lotteryNode);  // 按照结点输出
void print_lotteryList(GenericList *lotteryList, int n);  // 按照链表输出
// 根据输入数据批量输出信息
void print_lotteryList_data(GenericList *lotteryList, void *data, int n);
// 用于用户购买时的信息展示，因此要特化输出信息，
void print_lotteryList_head_to_uaer(void);
void print_lotteryList_tail_to_uaer(void);
void print_lottery_to_uaer(void *lotteryData);
int print_lotteryList_data_to_uaer(GenericList *lotteryList, void *data, int n);

int get_lottery_issue(GenericList *lotteryList);
Date get_lottery_data(const int offset);

/***********************************************************************************************************/
//  内层购买信息的链表操作
/***********************************************************************************************************/
//优先完成通用链表的回调函数
int isEqual_sale_info(void *sale_infoData, void *data, int n);
int compare_sale_info(void *sale_infoDataA, void *sale_infoDataB, const int n);
void print_sale_info(void *sale_infoData);

void print_sale_infoListl(GenericList *sale_infoList, int n);
void print_sale_infoList_data(GenericList *sale_infoList, void *data, int n);


/***********************************************************************************************************/
//  内外层链表的公共操作部分
/***********************************************************************************************************/
// 发行新的彩票
void publish_lotteryNode(GenericList *lotteryList);

// 系统进入时调用，更新状态
void update_lottery_info(GenericList *lotteryList);

#endif
