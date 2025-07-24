/*
    4月7日工作安排：
    内容：公证处
    1、开奖：完成
    2、兑奖：完成
    3、查看往期：完成
*/
#include "officeOS.h"

//  gcc -o .\bin\officeOS -Iinclude .\src\genericlist.c .\src\publicdata.c .\src\datafile.c .\src\userdata.c .\src\lotterydata.c .\src\display.c .\src\officeOS.c
//  make -f officeOS.mak
//  .\bin\officeOS
// 2025/4/5
int office_main(int argc, char **argv)
{
    GenericList *lotteryList = create_genericList(sizeof(Lottery));
    load_genericList(lotteryList, NULL, 2);
    update_lottery_info(lotteryList);

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
   
    int mode_ctrl = -1;
    while (mode_ctrl)
    {
        if(-1 == mode_ctrl)
        {
            clearScreen();
            // 功能界面
            office_user_menu();
        }
        printf("    [系统]>公证处>请选择：");
        input_int(&mode_ctrl);

        switch (mode_ctrl)
        {
            case 0:
                break;

            case 1:  // 开奖：兑换日期判断，输出中奖的注，用户匹配（更新用户购买记录），检测销售的彩票中活跃用户有多少中奖，计算奖金，更新当前用户的购买记录
                office_publish_win_bet_framework(lotteryList);
                break;

            case 2:  // 兑奖：提示请先开奖，用户匹配，检测未兑换彩票（若没有提示先开奖，或未中奖），兑换日期判断（提示过期），
                office_exchange_win_bet_framework(lotteryList);
                break;

            case 3:  // 往期中奖记录，正序期号排序，输出所有中奖的彩票销售信息
                office_win_record(lotteryList);
                break;

            default:
                printf("    [系统]>输入错误！！！\n");
                break;
        }
    }

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

    return 0;
}


/*vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
// 框架

// 兑换日期判断，输出中奖的注，
// 检测销售的彩票中活跃用户有多少中奖，计算奖金，
// 用户匹配，更新当前用户的购买记录 2025/4/5
void office_publish_win_bet_framework(GenericList *lotteryList)
{
    GenericNode *userNode = NULL;
    // 读入用户链表
    GenericList *userList = create_genericList(sizeof(User));
    load_genericList(userList, NULL, 1);

    // 用户名输入，用于得到用户购买信息链表，开奖更新状态
    while (1)
    {
        char user_name[LENGTH_OF_UNAME + 1];
        printf("    [系统]>请输入您的用户名（0、取消开奖）：");
        scanf("%20s", user_name);
        fflush(stdin);
        if(0 == strcmp(user_name, "0"))
        {
            printf("    [系统]>用户取消开奖！\n");
            destroy_genericList(userList);
            return ;
        }
        userNode = find_genericlist(userList, user_name, 2, isEqual_user);
        if(NULL == userNode)
        {
            printf("    [系统]>用户%s不存在，请先注册！\n", user_name);
        }
        else
        {
            break;
        }
    }

    // 读入购买信息
    GenericList *purch_infoList = create_genericList(sizeof(User));
    load_genericList(purch_infoList, userNode, -1);
    // 更新状态
    update_purch_info(purch_infoList);

    // 日期变量
    Date nowDate = {0};
    get_date(&nowDate);
    
    int flag_int = 0;  // 标志位
    int counter_user_bets = 0;  // 记录用户一共中了多找注
    // 遍历彩票发行链表
    GenericNode *current_lotteryNode = lotteryList->head->next;
    while (lotteryList->head != current_lotteryNode)
    {
        // 开奖日期已到
        if (isEqual_lottery(current_lotteryNode->data, &nowDate, 4))
        {
            // 输出中奖的注
            if (0 == flag_int)
            {   // 表头
                printf(" ──────────┬─────────────\n");
                printf("    期号   │ 中奖的注 \n");
                printf(" ──────────┼─────────────\n");
                flag_int = 1;
            }
            Lottery *lotteryData = (Lottery *)current_lotteryNode->data;
            printf("  %08d │ [%s] \n", lotteryData->issue, lotteryData->win_bet);
            
            /*vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
            // 更新销售信息状态
            int counter_bets = 0;  // 定义计数器，记录有多少用户活跃的注中奖，计算奖金
            int user_status = 1;  // 用户状态变量，控制只有活跃用户才能更新销售信息  "注销", "活跃"
            // 遍历销售记录更新状态
            GenericNode *current_sale_infoNode = lotteryData->sale_info_list->head->next;
            while (lotteryData->sale_info_list->head != current_sale_infoNode)
            {
                Sale_Info *sale_infoData = current_sale_infoNode->data;
                int lottery_status = 1;  //
                if (isEqual_sale_info(current_sale_infoNode->data, &lottery_status, -5))  // 只检测未开奖
                {
                    int temp_counter_bets = 0; // 记录每张彩票有几注中奖，用于更新状态
                    for(int i = 0; i < 5; i++)
                    {
                        if (isEqual_sale_info(current_sale_infoNode->data, lotteryData->win_bet, i) // 下注的检测
                        && isEqual_sale_info(current_sale_infoNode->data, &user_status, -4) )  // "活跃"状态的用户
                        {
                            counter_bets++;
                            temp_counter_bets++;
                            sale_infoData->lot_status = 3;  // "已开奖", "未开奖", "未中奖", "未兑换", "已兑换", "已过期"
                        }
                    }
                    if(0 == temp_counter_bets)  // 给没中奖的彩票销售记录更新状态
                    {
                        sale_infoData->lot_status = 2;
                    }
                }                
                current_sale_infoNode = current_sale_infoNode->next;
            }
            save_genericList(lotteryData->sale_info_list, current_lotteryNode, -2);// 保存一下

            // 更新彩票发行结点信息
            lotteryData->lot_status = 0;
            if (counter_bets)
            {
                lotteryData->bonus_per_bet = lotteryData->total_bonus / (double)counter_bets;  // 有人中奖
            }
            else
            {
                lotteryData->bonus_per_bet = lotteryData->total_bonus;  // 没人中奖
            }          
            /*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
            /*vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
            // 更新用户的购买信息
            counter_user_bets = 0;  // 记录用户一共中了多找注
            // 遍历购买信息
            GenericNode *current_purch_infoNode = purch_infoList->head->next;
            while (purch_infoList->head != current_purch_infoNode)
            {
                Purch_Info *purch_infoData = current_purch_infoNode->data;
                int temp_counter_bets = 0;  // 记录每张彩票有几注中奖，用于计算中奖金额
                int purch_info_ststus = 1;  // 记录查询变量，防止重复开奖 "已开奖", "未开奖", "未中奖", "未兑换", "已兑换", "已过期"
                if( isEqual_purch_info(current_purch_infoNode->data, &lotteryData->issue, -4)  // 查询期号，
                && isEqual_purch_info(current_purch_infoNode->data, &purch_info_ststus, -3))  // 查询必须未开奖，防止反复开奖
                {
                    for (int i = 0; i < 5; i++)
                    {
                        if (isEqual_purch_info(current_purch_infoNode->data, lotteryData->win_bet, i) )
                        {
                            temp_counter_bets++;
                            counter_user_bets++;
                        }
                    }
                    if (0 == temp_counter_bets)  // 没中奖
                    {
                        purch_infoData->lot_status = 2;  // "已开奖", "未开奖", "未中奖", "未兑换", "已兑换", "已过期"
                    }
                    else
                    {
                        purch_infoData->lot_status = 3;  // "已开奖", "未开奖", "未中奖", "未兑换", "已兑换", "已过期"
                        purch_infoData->bonus = lotteryData->bonus_per_bet * (double)temp_counter_bets;  // 更新奖金
                    }
                }

                current_purch_infoNode = current_purch_infoNode->next;
            }
            /*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/ 
        }
        current_lotteryNode = current_lotteryNode->next;
    }

    if(1 == flag_int)
    {
        printf(" ──────────┴─────────────\n");
        if (counter_user_bets)
        {
            printf("    [系统]>您一共有%d注中奖！！！\n", counter_user_bets);
        }
        else
        {
            printf("    [系统]>很遗憾，您没有中奖。\n");
        }
    }
    else  // 未输出任何信息说明没有匹配的数据
    {
        printf("    [系统]>今天没有彩票进行开奖！！！\n");
    }
    // 这个发行信息链表也保存一下
    save_genericList(lotteryList, NULL, 2);
    // 保存释放空间
    save_genericList(purch_infoList, userNode, -1);
    destroy_genericList(purch_infoList);
    destroy_genericList(userList);
}

// 提示请先开奖，用户匹配，检测未兑换彩票（若没有提示先开奖，或未中奖），兑换日期判断（提示过期） 2025/4/5
void office_exchange_win_bet_framework(GenericList *lotteryList)
{
    GenericNode *userNode = NULL;
    // 读入用户链表
    GenericList *userList = create_genericList(sizeof(User));
    load_genericList(userList, NULL, 1);
    // 提示一下
    printf("    [系统]>请先开奖再兑换！！！\n");
    // 用户名输入，用于得到用户购买信息链表，开奖更新状态
    while (1)
    {
        char user_name[LENGTH_OF_UNAME + 1];
        printf("    [系统]>请输入用户名（0、取消兑奖）：");
        scanf("%20s", user_name);
        fflush(stdin);
        if(0 == strcmp(user_name, "0"))
        {
            printf("    [系统]>用户取消开奖！\n");
            destroy_genericList(userList);
            return ;
        }
        userNode = find_genericlist(userList, user_name, 2, isEqual_user);
        if(NULL == userNode)
        {
            printf("    [系统]>用户%s不存在，请先注册！\n", user_name);
        }
        else
        {
            break;
        }
    }

    // 读入购买信息
    GenericList *purch_infoList = create_genericList(sizeof(User));
    load_genericList(purch_infoList, userNode, -1);
    // 更新状态
    update_purch_info(purch_infoList);

    /*vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
    int flag_int = 0;// 标志位，检测是否有兑换
    int purch_info_ststus = 3;  // 记录查询变量 "已开奖", "未开奖", "未中奖", "未兑换", "已兑换", "已过期"
    // 用户购买记录遍历
    GenericNode *current_purch_infoNode = purch_infoList->head->next;
    while (purch_infoList->head != current_purch_infoNode)
    {
        Purch_Info *purch_infoData = (Purch_Info *)current_purch_infoNode->data;
        // 判断是否未兑换 过期彩票在载入更新时已经全部更新，因此不与要再次判断
        if (isEqual_purch_info(current_purch_infoNode->data, &purch_info_ststus, -3))
        {
            // 根据期号查找彩票发行信息结点，
            GenericNode *lotteryNode = find_genericlist(lotteryList, &purch_infoData->issue, 1, isEqual_lottery);
            Lottery *lotteryData = (Lottery *)lotteryNode->data;
            // 根据唯一识别找到销售信息更新状态
            GenericNode *sale_infoNode = find_genericlist(lotteryData->sale_info_list, purch_infoData->lot_id, -1, isEqual_sale_info);
            // 兑换，更新用户信息，购买记录信息
            // 购买记录更新为已兑换
            Sale_Info *sale_infoData = (Sale_Info *)sale_infoNode->data;
            sale_infoData->lot_status = 4;  // "已开奖", "未开奖", "未中奖", "未兑换", "已兑换", "已过期"
            // 销售记录更新为已兑换
            purch_infoData->lot_status = 4;
            // 用户余额增加奖金
            User *userData = (User *)userNode->data;
            userData->balance += purch_infoData->bonus;
            // 兑奖标志位置位
            flag_int = 1;
        }
        current_purch_infoNode = current_purch_infoNode->next;
    }

    // 没有未兑换的彩票，告知用户
    if (flag_int)
    {
        printf("    [系统]>您中奖彩票已经成功兑换至您的余额！\n");
    }
    else
    {
        printf("    [系统]>您没有中奖未兑换的彩票！\n");
    }
    /*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
    
    // 保存释放空间
    save_genericList(purch_infoList, userNode, -1);
    save_genericList(userList, NULL, 1);
    destroy_genericList(purch_infoList);
    destroy_genericList(userList);
}

// 输出往期中奖记录 2025/4/3
void office_win_record(GenericList *lotteryList)
{
    int flag_int = 1;
    Date nowDate = {0};
    get_date(&nowDate);
    // 按期号正序排序
    sort_genericList(lotteryList->head, lotteryList->head
        , lotteryList->head->next, lotteryList->head->prior, 1, compare_lottery);

    GenericNode *current = lotteryList->head->next;

    while (lotteryList->head != current)
    {
        Lottery *lotteryData = (Lottery *)current->data;

        if (compare_date(&nowDate, &lotteryData->exchange_data, 0))  // 判断是否已过开奖日期
        {
            printf("    ────────────────────────────────────────────\n");
            printf("        期号：[%08d]    中奖的注：[%s]    \n", lotteryData->issue, lotteryData->win_bet);
            printf("    ────────────────────────────────────────────\n");

            //"已开奖", "未开奖", "未中奖", "未兑换", "已兑换", "已过期"
            int temp_int = 4;
            print_sale_infoList_data(lotteryData->sale_info_list, &temp_int, -5);
            flag_int = 0;
        }
        current = current->next;
    }
    if (flag_int)
    {
        printf("    [系统]>没有彩票进行过开奖！\n");
    }
    
}

/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
/*vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
// 基础功能


/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/



