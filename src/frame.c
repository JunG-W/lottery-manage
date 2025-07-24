
#include "frame.h"

void func_farmework(void)
{
    int mode_ctrl = -1;
    while (mode_ctrl)
    {
        if(-1 == mode_ctrl)
        {
            clearScreen();
            // 功能界面
            func_farmework_menu();
        }
        printf("    [系统]>请选择：");
        input_int(&mode_ctrl);

        switch (mode_ctrl)
        {
            case 0:
                break;

            case 1:  // 用户系统
                user_main(0, NULL);
                mode_ctrl = -1;
                break;

            case 2:  // 彩票中心
                lottery_main(0, NULL);
                mode_ctrl = -1;
                break;

            case 3:  // 公证处
                office_main(0, NULL);
                mode_ctrl = -1;
                break;

            case 4:  // 演示时需要的功能
                change_lottery_framework();
                mode_ctrl = -1;
                break;

            default:
                printf("    [系统]>输入错误！！！\n");
                break;
        }
    }
}

void change_lottery_framework(void)
{
    GenericList *lotteryList = create_genericList(sizeof(Lottery));
    load_genericList(lotteryList, NULL, 2);
    update_lottery_info(lotteryList);  // 更新信息

    // 循环读取全部彩票发行信息
    GenericNode *current = lotteryList->head->next;
    while (lotteryList->head != current)
    {
        Lottery *lotteryData = (Lottery *)current->data;
        GenericList *sale_infoList = create_genericList(sizeof(Sale_Info));
        load_genericList(sale_infoList, current, -2);
        lotteryData->sale_info_list = sale_infoList;
        current = current->next;
    }

    /*=============================================================*/
    // 功能区

    int mode_ctrl = -1;
    while (mode_ctrl)
    {
        if(-1 == mode_ctrl)
        {
            clearScreen();
            // 功能界面
            change_lottery_framework_menu();
        }
        printf("    [系统]>请选择：");
        input_int(&mode_ctrl);

        switch (mode_ctrl)
        {
            case 0:
                break;

            case 1:  // 修改购买截至日期
                change_sale_date(lotteryList);
                // mode_ctrl = -1;
                break;

            case 2:  // 修改兑奖日期
                change_exchange_date(lotteryList);
                // mode_ctrl = -1;
                break;

            case 3:  // 修改中奖的注
                change_win_bet(lotteryList);
                // mode_ctrl = -1;
                break;

            default:
                printf("    [系统]>输入错误！！！\n");
                break;
        }
    }

    /*=============================================================*/

    // 循环保存，释放链表
    current = lotteryList->head->next;
    while (lotteryList->head != current)
    {
        Lottery *lotteryData = (Lottery *)current->data;
        save_genericList(lotteryData->sale_info_list, current, -2); // 保存
        destroy_genericList(lotteryData->sale_info_list);  // 释放链表
        lotteryData->sale_info_list = NULL;  // 手动置空
        current = current->next;
    }
    save_genericList(lotteryList, NULL, 2);
    // 释放链表
    destroy_genericList(lotteryList);
}

// 修改购买截至时间
void change_sale_date(GenericList *lotteryList)
{
    print_lotteryList(lotteryList, 1);

    int input_issue = 0;
    printf("    [系统]>请输入发行期号：");
    input_int(&input_issue);
    GenericNode *lotteryNode = find_genericlist(lotteryList, &input_issue, 1, isEqual_lottery);
    if (NULL == lotteryNode)
    {
        printf("    [系统]>没有期号为%08d的彩票被发行！\n", input_issue);
        return ;
    }
    Lottery *lotteryDate = (Lottery *)lotteryNode->data;
    // 定义变量
    Date set_date = {0};
    printf("    [系统]>请输入新的购买截至日期！\n");
    while (1)
    {
        printf("    [系统]>年份：");
        input_int(&set_date.year);
        if (0 < set_date.year)
        {
            break;
        }
        else if (0 == set_date.year)
        {
            printf("    [系统]>取消修改！\n");
            return;
        }
        else
        {
            printf("    [系统]>年份错误！\n");
        }
    }
    while (1)
    {
        printf("    [系统]>月份：");
        input_int(&set_date.month);
        if (0 < set_date.month && 13 > set_date.month)
        {
            break;
        }
        else if (0 == set_date.month)
        {
            printf("    [系统]>取消修改！\n");
            return;
        }
        else
        {
            printf("    [系统]>月份错误！\n");
        }
    }
    while (1)
    {
        printf("    [系统]>日期：");
        input_int(&set_date.day);
        if (0 < set_date.day && get_days(set_date.year, set_date.month) >= set_date.day)
        {
            break;
        }
        else if (0 == set_date.day)
        {
            printf("    [系统]>取消修改！\n");
            return;
        }
        else
        {
            printf("    [系统]>%d月没有%d号！\n", set_date.month, set_date.day);
        }
    }
    // 整体赋值写入
    lotteryDate->sale_deadline = set_date;
    update_lottery_info(lotteryList);  // 更新信息
    printf("    [系统]>修改成功！\n");
}

// 修改开奖及兑换时间
void change_exchange_date(GenericList *lotteryList)
{
    print_lotteryList(lotteryList, 1);

    int input_issue = 0;
    printf("    [系统]>请输入发行期号：");
    input_int(&input_issue);
    GenericNode *lotteryNode = find_genericlist(lotteryList, &input_issue, 1, isEqual_lottery);
    if (NULL == lotteryNode)
    {
        printf("    [系统]>没有期号为%08d的彩票被发行！\n", input_issue);
        return ;
    }
    Lottery *lotteryDate = (Lottery *)lotteryNode->data;
    // 定义变量
    Date set_date = {0};
    printf("    [系统]>请输入新的兑换日期！\n");
    while (1)
    {
        printf("    [系统]>年份：");
        input_int(&set_date.year);
        if (0 < set_date.year)
        {
            break;
        }
        else if (0 == set_date.year)
        {
            printf("    [系统]>取消修改！\n");
            return;
        }
        else
        {
            printf("    [系统]>年份错误！\n");
        }
    }
    while (1)
    {
        printf("    [系统]>月份：");
        input_int(&set_date.month);
        if (0 < set_date.month && 13 > set_date.month)
        {
            break;
        }
        else if (0 == set_date.month)
        {
            printf("    [系统]>取消修改！\n");
            return;
        }
        else
        {
            printf("    [系统]>月份错误！\n");
        }
        
    }
    while (1)
    {
        printf("    [系统]>日期：");
        input_int(&set_date.day);
        if (0 < set_date.day && get_days(set_date.year, set_date.month) >= set_date.day)
        {
            break;
        }
        else if (0 == set_date.day)
        {
            printf("    [系统]>取消修改！\n");
            return;
        }
        else
        {
            printf("    [系统]>%d月没有%d号！\n", set_date.month, set_date.day);
        }
    }

    lotteryDate->exchange_data = set_date;
    update_lottery_info(lotteryList);  // 更新信息
    printf("    [系统]>修改成功！\n");
}

// 打印正在开奖的彩票修改其奖注
void change_win_bet(GenericList *lotteryList)
{
    int flag_int = 0;  // 标志位
    int lottery_status = 0;  // 状态临时变量"已开奖", "未开奖", "未中奖", "未兑换", "已兑换", "已过期"
    // 遍历彩票发行链表
    GenericNode *current_lotteryNode = lotteryList->head->next;
    while (lotteryList->head != current_lotteryNode)
    {
        // // 开奖日期已到
        // if (isEqual_lottery(current_lotteryNode->data, &lottery_status, 2))
        // {
            // 输出中奖的注
            if (0 == flag_int)
            {   // 表头
                printf(" ──────────┬─────────────\n");
                printf("    期号   │ 中奖的注 \n");
                printf(" ──────────┼─────────────\n");
                flag_int = 1;
            }
            Lottery *current_lotteryData = (Lottery *)current_lotteryNode->data;
            printf("  %08d │ [%s] \n", current_lotteryData->issue, current_lotteryData->win_bet);
        // }
        current_lotteryNode = current_lotteryNode->next;
    }
    if(1 == flag_int)
    {
        printf(" ──────────┴─────────────\n");
    }
    else  // 未输出任何信息说明没有匹配的数据
    {
        printf("    [系统]>今天没有彩票进行开奖！！！\n");
        return ;
    }

    // 查找
    int input_issue = 0;
    printf("    [系统]>请输入发行期号：");
    input_int(&input_issue);
    GenericNode *lotteryNode = find_genericlist(lotteryList, &input_issue, 1, isEqual_lottery);
    if (NULL == lotteryNode)
    {
        printf("    [系统]>没有期号为%08d的彩票被发行！\n", input_issue);
        return ;
    }
    Lottery *lotteryDate = (Lottery *)lotteryNode->data;

    // 修改操作
    int bet = 0;
    int flag = 1;
    while(flag)
    {
        if(1 == flag)
        {
            printf("    [系统]>每注由6位数字组成范围从[000000]到[999999]；可以重复下相同的注\n");
            flag = 2;
        }
        printf("    [系统]>请输入修改后的注：");
        input_int(& bet);

        if( 0 <= bet && 999999 >= bet)
        {
            sprintf(lotteryDate->win_bet, "%06d", bet);
            flag = 0;
        }
        else
        {
            printf("    [系统]>[%06d]输入错误！！！\n", bet);
            flag = 1;
        }
    }
    printf("    [系统]>修改成功！\n");
    printf(" ──────────┬─────────────\n");
    printf("    期号   │ 中奖的注 \n");
    printf(" ──────────┼─────────────\n");
    printf("  %08d │ [%s] \n", lotteryDate->issue, lotteryDate->win_bet);
    printf(" ──────────┴─────────────\n");
}

void func_farmework_menu(void)
{
    printf("┌──────────────────────────────────────────┐\n");
    printf("│                                          │\n");
    printf("│               彩票系统三合一             │\n");
    printf("│                                          │\n");
    printf("│              1、 彩民 系统               │\n");
    printf("│              2、 彩票 中心               │\n");
    printf("│              3、 公 证 处                │\n");
    printf("│              4、 外挂 模块               │\n");
    printf("│              0、  退  出                 │\n");
    printf("│                                          │\n");
    printf("└──────────────────────────────────────────┘\n"); 
}

void change_lottery_framework_menu(void)
{
    printf("┌──────────────────────────────────────────┐\n");
    printf("│                                          │\n");
    printf("│               外 挂 模 块                │\n");
    printf("│                                          │\n");
    printf("│              1、购买截止日期             │\n");
    printf("│              2、 兑换  日期              │\n");
    printf("│              3、修改中奖的注             │\n");
    printf("│              0、  退    出               │\n");
    printf("│                                          │\n");
    printf("└──────────────────────────────────────────┘\n"); 
}



