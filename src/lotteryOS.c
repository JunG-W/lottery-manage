/*

*/
#include "lotteryOS.h"

//  gcc -o .\bin\lotteryOS -Iinclude .\src\genericlist.c .\src\publicdata.c .\src\datafile.c .\src\userdata.c .\src\lotterydata.c .\src\display.c .\src\lotteryOS.c
//  make -f lotteryOS.mak
//  .\bin\lotteryOS
// 2025/4/3
int lottery_main(int argc, char **argv)
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

    // 随机数种子载入
    srand(time(NULL));

    int mode_ctrl = -1;
    while (mode_ctrl)
    {
        if(-1 == mode_ctrl)
        {
            clearScreen();
            // 功能界面
            lottery_main_menu();
        }
        printf("    [系统]>彩票中心管理系统>请选择：");
        input_int(&mode_ctrl);

        switch (mode_ctrl)
        {
            case 0:
                break;

            case 1:  // 发行彩票
                publish_lotteryNode(lotteryList);
                break;

            case 2:  // 发行信息及销售记录
                lottery_info_framework(lotteryList);
                mode_ctrl = -1;
                break;

            case 3:  // 彩民信息及购买记录
                lottery_user_framework();
                mode_ctrl = -1;
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

void lottery_info_framework(GenericList *lotteryList)
{
    int mode_ctrl = -1;
    while (mode_ctrl)
    {
        if(-1 == mode_ctrl)
        {
            clearScreen();
            // 功能界面
            lottery_info_menu();
        }
        printf("    [系统]>发行信息及销售记录>请选择：");
        input_int(&mode_ctrl);

        switch (mode_ctrl)
        {
            case 0:
                break;

            case 1:  // 所有发行彩票的记录
                if (lotteryList->head == lotteryList->head->next)
                {
                    printf("    [系统]>没有彩票发行信息！！！\n");
                    break;
                }
            
                lottery_publish_record(lotteryList);
                break;

            case 2:  // 某期彩票的销售信息
                if (lotteryList->head == lotteryList->head->next)
                {
                    printf("    [系统]>没有彩票发行信息！！！\n");
                    break;
                }

                lottery_sale_record(lotteryList);
                break;

            default:
                printf("    [系统]>输入错误！！！\n");
                break;
        }
    }
}

void lottery_publish_record(GenericList *lotteryList)
{
    int input_num1 = -1;  // 控制排序内容
    int input_num2 = -1;  // 控制输出放向

    while (1)
    {
        printf("    [系统]>{ [0]:退出 [1]:期号 [2]:开奖状态 [3]:出售数量 [4]:兑奖日期 [5]:每注价格 [6]:奖金总数 [7]:每注奖金 }\n");
        printf("    [系统]>请选择输出方式：");
        input_int(&input_num1);
        if(0 == input_num1)
        {
            printf("    [系统]>用户取消信息查询！\n");
            return ;
        }
        else if(1 > input_num1 && 7 < input_num1)
        {
            printf("    [系统]>错误，没有该选项！！！\n");
        }
        else
        {
            break;
        }
    }
    while (1)
    {
        printf("    [系统]>{ [0]:退出 [1]:正序 [-1]:逆序 }\n");
        printf("    [系统]>请选择：");
        input_int(&input_num2);
        if(0 == input_num2)
        {
            printf("    [系统]>用户取消信息查询！\n");
            return ;
        }
        else if(1 != input_num2 && -1 != input_num2)
        {
            printf("    [系统]>错误，没有该选项！！！\n");
        }
        else
        {
            break;
        }
    }

    sort_genericList(lotteryList->head, lotteryList->head
        , lotteryList->head->next, lotteryList->head->prior
        , input_num1, compare_lottery);

    print_lotteryList(lotteryList, input_num2);
 
}

void lottery_sale_record(GenericList *lotteryList)
{
    int input_issue = 0;
    printf("    [系统]>请输入发行期号：");
    input_int(&input_issue);
    GenericNode *lotteryNode = find_genericlist(lotteryList, &input_issue, 1, isEqual_lottery);
    if (NULL == lotteryNode)
    {
        printf("    [系统]>没有期号为%08d的彩票被发行！\n", input_issue);
        return ;
    }
    
    Lottery *lotteryData = (Lottery *)lotteryNode->data;

    if(lotteryData->sale_info_list->head == lotteryData->sale_info_list->head->next)
    {
        printf("    [系统]>该彩票暂时没有销售记录！！！\n");
        return ;
    }

    int input_num1 = -1;  // 控制排序内容
    int input_num2 = -1;  // 控制输出放向

    while (1)
    {
        printf("    [系统]>{ [0]:退出 [1]:中奖状态 [2]:账户状态 [3]:UID [4]:用户名 [5]:彩票唯一标识 }\n");
        printf("    [系统]>请选择输出方式：");
        input_int(&input_num1);
        if(0 == input_num1)
        {
            printf("    [系统]>用户取消信息查询！\n");
            return ;
        }
        else if(1 > input_num1 && 7 < input_num1)
        {
            printf("    [系统]>错误，没有该选项！！！\n");
        }
        else
        {
            break;
        }
    }
    while (1)
    {
        printf("    [系统]>{ [0]:退出 [1]:正序 [-1]:逆序 }\n");
        printf("    [系统]>请选择：");
        input_int(&input_num2);
        if(0 == input_num2)
        {
            printf("    [系统]>用户取消信息查询！\n");
            return ;
        }
        else if(1 != input_num2 && -1 != input_num2)
        {
            printf("    [系统]>错误，没有该选项！！！\n");
        }
        else
        {
            break;
        }
    }

    sort_genericList(lotteryData->sale_info_list->head, lotteryData->sale_info_list->head
        , lotteryData->sale_info_list->head->next, lotteryData->sale_info_list->head->prior
        , input_num1, compare_sale_info);

    print_sale_infoListl(lotteryData->sale_info_list, input_num2);
}

void lottery_user_framework(void)
{
    GenericList *userList = create_genericList(sizeof(User));
    load_genericList(userList, NULL, 1);

    int mode_ctrl = -1;
    while (mode_ctrl)
    {
        if(-1 == mode_ctrl)
        {
            clearScreen();
            // 功能界面
            lottery_user_menu();
        }
        printf("    [系统]>彩民信息及购买记录>请选择：");
        input_int(&mode_ctrl);

        switch (mode_ctrl)
        {
            case 0:
                break;

            case 1:  // 所有用户信息
                lottery_user_info(userList);
                break;

            case 2:  // 用户的购买记录
                lottery_user_pruchase(userList);
                break;

            default:
                printf("    [系统]>输入错误！！！\n");
                break;
        }
    }

    save_genericList(userList, NULL, 1);
    destroy_genericList(userList);
}

void lottery_user_info(GenericList *userList)
{
    int input_num1 = -1;  // 控制排序内容
    int input_num2 = -1;  // 控制输出放向

    while (1)
    {
        printf("    [系统]>{  [0]:退出 [1]:UID [2]:用户名 [3]:真实姓名 [4]:电话号码 [5]:余额 }\n");
        printf("    [系统]>请选择输出方式：");
        input_int(&input_num1);
        if(0 == input_num1)
        {
            printf("    [系统]>用户取消信息查询！\n");
            return ;
        }
        else if(1 > input_num1 && 7 < input_num1)
        {
            printf("    [系统]>错误，没有该选项！！！\n");
        }
        else
        {
            break;
        }
    }
    while (1)
    {
        printf("    [系统]>{ [0]:退出 [1]:正序 [-1]:逆序 }\n");
        printf("    [系统]>请选择：");
        input_int(&input_num2);
        if(0 == input_num2)
        {
            printf("    [系统]>用户取消信息查询！\n");
            return ;
        }
        else if(1 != input_num2 && -1 != input_num2)
        {
            printf("    [系统]>错误，没有该选项！！！\n");
        }
        else
        {
            break;
        }
    }

    sort_genericList(userList->head, userList->head
        , userList->head->next, userList->head->prior
        , input_num1, compare_user);

    print_userList(userList, input_num2);
}

void lottery_user_pruchase(GenericList *userList)
{
    char input_user_name[LENGTH_OF_UNAME + 1] = {0};
    printf("    [系统]>请输入用户名：");
    scanf("%20s", input_user_name);
    fflush(stdin);
    GenericNode *userNode = find_genericlist(userList, input_user_name, 2, isEqual_user);
    if (NULL == userNode)
    {
        printf("    [系统]>没有用户名为%s的用户！\n", input_user_name);
        return ;
    }
    // 信息读入
    GenericList *purch_infoList = create_genericList(sizeof(Purch_Info));
    load_genericList(purch_infoList, userNode, -1);

    update_purch_info(purch_infoList);

    int input_num1 = -1;  // 控制排序内容
    int input_num2 = -1;  // 控制输出放向

    while (1)
    {
        printf("    [系统]>{ [0]:退出 [1]:期号 [2]:中奖状态 [3]:购买日期 [4]:每注价格 [5]:奖金 [6]:彩票唯一标识 }\n");
        printf("    [系统]>请选择输出方式：");
        input_int(&input_num1);
        if(0 == input_num1)
        {
            printf("    [系统]>用户取消信息查询！\n");
            return ;
        }
        else if(1 > input_num1 && 7 < input_num1)
        {
            printf("    [系统]>错误，没有该选项！！！\n");
        }
        else
        {
            break;
        }
    }
    while (1)
    {
        printf("    [系统]>{ [0]:退出 [1]:正序 [-1]:逆序 }\n");
        printf("    [系统]>请选择：");
        input_int(&input_num2);
        if(0 == input_num2)
        {
            printf("    [系统]>用户取消信息查询！\n");
            return ;
        }
        else if(1 != input_num2 && -1 != input_num2)
        {
            printf("    [系统]>错误，没有该选项！！！\n");
        }
        else
        {
            break;
        }
    }

    sort_genericList(purch_infoList->head, purch_infoList->head
        , purch_infoList->head->next, purch_infoList->head->prior
        , input_num1, compare_purch_info);

    print_purch_infoListl(purch_infoList, input_num2);

    // 查询完毕释放链表
    destroy_genericList(purch_infoList);
}

/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
/*vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
// 基础功能


/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/



