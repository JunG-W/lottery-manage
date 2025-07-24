/*
@ file_name: frame.h
@ function: 
@ 用于测试三个系统，需要编写测试用的函数，并将三个系统整合由一个菜单进入，
@ 测试需要的函数：测试流程：彩民->注册、登录、购买、查看信息、修改信息；彩票中心->发行彩票、查看彩票信息、查看彩民信息；公证处->开奖、兑奖、往期中奖记录
@                1、需要可以修改奖注，用于演示中奖
@                2、需要修改开奖时间，演示开奖操作
@                3、需要修改购买截至日期，用于演示购买截至
@ author: wang.
@ modify_time: ------------------------------------------------------------------------------------------------------
*/
#ifndef __FRAME_H__
#define __FRAME_H__

// // 在头文件之前控制主函数名称
#ifndef MAIN_FUNC__
#define MAIN_FUNC__
#endif

#include "userOS.h"
#include "lotteryOS.h"
#include "officeOS.h"
/*
    当前工作：
        1.用户系统添加提现功能 √
        2.调试各个分系统 
        3.完成演示功能整合
*/ 

/***********************************************************************************************************/
//  框架
/***********************************************************************************************************/
void func_farmework(void);

void change_lottery_framework(void);
/***********************************************************************************************************/
//  基础功能，
/***********************************************************************************************************/

void change_sale_date(GenericList *lotteryList);

void change_exchange_date(GenericList *lotteryList);

void change_win_bet(GenericList *lotteryList);

// 菜单
void func_farmework_menu(void);
void change_lottery_framework_menu(void);


#endif
