/*
@ file_name: userOS.h
@ function: 
@ 实现彩票系统中彩民系统的建立及相关基本等功能；
@ author: wang.
@ modify_time: ------------------------------------------------------------------------------------------------------
*/
#ifndef __LOTTERYOS_H__
#define __LOTTERYOS_H__

#include "userdata.h"
#include "lotterydata.h"
#include "display.h"

#ifndef MAIN_FUNC__
#define lottery_main main
#else
#define lottery_main lottery_func
#endif
/***********************************************************************************************************/
//  系统入口 要求：使用条件编译，使其作为独立系统时仍能编译运行
/***********************************************************************************************************/
int lottery_main(int argc, char **argv);
/***********************************************************************************************************/
//  框架
/***********************************************************************************************************/
// 该系统默认为内部人员使用，因此未设置系统登陆登录等操作
// 进入系统，读取彩票发行信息，
// 获取时间，更新彩票发行信息，
// 循环读入所有彩票发行信息
void lottery_info_framework(GenericList *lotteryList);
void lottery_publish_record(GenericList *lotteryList);
void lottery_sale_record(GenericList *lotteryList);

void lottery_user_framework(void);
void lottery_user_info(GenericList *userList);
void lottery_user_pruchase(GenericList *userList);

/***********************************************************************************************************/
//  基础功能，交叉使用三个及以上或跨文件使用链表的函数功能在此处实现
/***********************************************************************************************************/



#endif

