/*
@ file_name: genericlist.h
@ function: 
@ 实现循环双向通用链表及相关操作，包括插入，查找，删除，修改，排序，输出等功能；
@ author: wang.
@ modify_time: 2025/---------------------------------------------------
*/
#ifndef __PUBLICDATA_H__
#define __PUBLICDATA_H__

#include <stdio.h>
#include <time.h>
#include <conio.h>  // 包含 getch() 函数
#include "genericlist.h"

// // 在头文件之前控制主函数名称
// #ifndef MAIN_FUNC__
// #define MAIN_FUNC__
// #endif

#define LENGTH_OF_UID 10
#define LENGTH_OF_UNAME 20
#define LENGTH_OF_UPASS 20
#define LENGTH_OF_LOTID 16
#define LENGTH_PER_BET 6

typedef struct date
{
    int year;
    int month;
    int day;
}Date;

/***************************************************************************************************************/
// 用户文件
typedef struct purch_info  // 购买信息，作为用户节点的内层链表
{
    int issue;  // 8位数期号 年+月+期数
    int lot_status;  // 彩票中奖状态
    Date Purch_date;
    Date exchange_data;
    double price;  // 每注价格
    double bonus;  // 奖金开奖后赋值
    char lot_id[LENGTH_OF_LOTID + 1];  //  8位期号 + 8位随机数 + '\0'
    char bet[5][LENGTH_PER_BET+1];
}Purch_Info;

typedef struct user  // 外层链表------用户
{
    char user_id[LENGTH_OF_UID + 1];  // uid用户唯一标识，(不可重复使用，注销不回收)好像实现不太了目前来说
    char user_name[LENGTH_OF_UNAME + 1];  // 用户名
    char password[LENGTH_OF_UPASS + 1];  // 密码
    char real_name[LENGTH_OF_UNAME + 1];  // 真实姓名
    char phone_number[12];  // 电话号码
    double balance;  // 余额
    GenericList *purch_infor_list;
}User;

/***************************************************************************************************************/
// 彩票文件
typedef struct sale_info  // 销售信息，作为邮票发行节点的内层链表
{
    int lot_status;  // 彩票中奖状态
    int account_status;  // 账户状态
    char user_id[LENGTH_OF_UID + 1];
    char user_name[LENGTH_OF_UNAME + 1];  // 用户名
    Date sale_date;
    char lot_id[LENGTH_OF_LOTID + 1];  //  8位期号 + 8位随机数 + '\0'
    char bet[5][LENGTH_PER_BET+1];
}Sale_Info;

typedef struct lottery  // 外层链表------彩票信息
{
    int issue;  // 8位数期号
    int lot_status;  // 开奖状态
    int sale_number;  // 售出数量按注计数
    Date sale_deadline;  // 出售截至日期
    Date exchange_data;  // 兑奖日期
    char win_bet[LENGTH_PER_BET + 1];  // 中奖的注+'\0'
    double price_per_bet;  // 每注价格
    double total_bonus;  // 奖金总数
    double bonus_per_bet;  // 每注奖金 开奖或兑换时计算得出
    GenericList *sale_info_list;
}Lottery;

extern char *LOTTERY_STATUS[];
extern char *ACCOUNT_STATUS[];

// 比较日期
int compare_date(void *date_A, void *date_B, const int n);
int isEqual_date(void *date_A, void *date_B, const int n);

void input_passward(char *password, const int len);

// void user_inf_syn(GenericList *userList, GenericList *studentList);

// 健壮性提高的输入
void input_int(int *num);
void input_double(double *num);

// 生成n位验证码
void get_verification_code(char *verif_code, int n);
// 获取日期
void get_date(Date *date);

int get_random_num(int n, int m);

int check_user_password(char *password, int n, int m);

int is_leapYear(int year);

int get_days(int year, int month);

#endif
