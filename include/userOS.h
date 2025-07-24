/*
@ file_name: userOS.h
@ function: 
@ 实现彩票系统中彩民系统的建立及相关基本等功能；
@ author: wang.
@ modify_time: ------------------------------------------------------------------------------------------------------
*/
#ifndef __USEROS_H__
#define __USEROS_H__

#include "userdata.h"
#include "lotterydata.h"
#include "display.h"

#ifndef MAIN_FUNC__
#define user_main main
#else
#define user_main user_func
#endif
/***********************************************************************************************************/
//  系统入口 要求：使用条件编译，使其作为独立系统时仍能编译运行
/***********************************************************************************************************/
int user_main(int argc, char **argv);
/***********************************************************************************************************/
//  框架
/***********************************************************************************************************/
// 登录注册提醒
// 登录读取文件后提醒用户是否有彩票可以开奖，发行的彩票信息等
// 获取时间更新相关信息的状态，如购买的彩票是否过期等
void user_framework(GenericList *userList, GenericNode *userNode);
void user_purchase_lottery(GenericList *lotteryList, GenericNode *userNode);
void user_blance_management(GenericNode *userNode);
void user_purchase_record(GenericNode *userNode);
int user_self_information(GenericList *userList, GenericNode *userNode, GenericList *lotteryList);

/***********************************************************************************************************/
//  基础功能，交叉使用三个及以上或跨文件使用链表的函数功能在此处实现
/***********************************************************************************************************/

// 购买彩票时调用的函数
// 余额不足检测，返回购买数量，并自动扣费
int input_number_of_bets(User *userData, Lottery *lotteryData);
// 彩票标识获取
void get_lot_id(Purch_Info *purch_infoData, Sale_Info *sale_infoData, Lottery *lotteryData);
// 用户下注
void input_bet(Purch_Info *purch_infoData, Sale_Info *sale_infoData, const int number_of_bets);
// 购买彩票 访问彩票发行信息的链表，彩票购买信息的创建与插入
int purchase_lottery(GenericNode *lotteryNode, GenericNode *userNode);

// 账户注销
void cancel_userNode(GenericList *userList, GenericNode *userNode, GenericList *lotteryList);

#endif

