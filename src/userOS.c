/*
    日期：4月3日
    完成功能：彩民+彩票中心
    1、用户购买彩票	：完成
    2、用户查看购买记录及中奖记录：完成
    3、彩票中心查看用户购买记录及中奖情况：完成
*/
#include "userOS.h"

//  gcc -o .\bin\userOS -Iinclude .\src\genericlist.c .\src\datafile.c .\src\publicdata.c .\src\userdata.c .\src\lotterydata.c .\src\display.c .\src\userOS.c
//  make -f userOS.mak
//  .\bin\userOS
// 2025/4/2
int user_main(int argc, char **argv)
{

    GenericList *userList = create_genericList(sizeof(User));
    if(NULL == userList)
    {
        return -1;
    }
    int errorcode = 0;
    errorcode = load_genericList(userList, NULL, 1);
    if( errorcode )
    {
        // return -1;
    }
    else
    {
        printf("    [系统]>用户信息载入成功！\n");
    }

    // 随机数种子载入
    srand(time(NULL));

    int mode_ctrl = -1;
    while (mode_ctrl)
    {
        if(-1 == mode_ctrl)
        {
            clearScreen();
            user_login_menu();// 登录界面
        }
        printf("    [系统]>登录界面>请选择：");
        input_int(&mode_ctrl);

        switch (mode_ctrl)
        {
            case 0:
                break;

            case 1:
                {
                    if(userList->head == userList->head->next)
                    {
                        printf("    [系统]>系统没有用户信息，请先注册！\n");
                        break;
                    }
                    GenericNode *userNode = NULL;
                    userNode = login(userList);
                    if(NULL != userNode)
                    {
                        user_framework(userList, userNode);
                    }
                    else
                    {
                        break;
                    }
                    mode_ctrl = -1;
                }
                break;

            case 2:
                {
                    enroll_userNode(userList);
                    // mode_ctrl = -1;
                }
                break;

            default:
                printf("    [系统]>输入错误！！！\n");
                break;
        }
    }
    
    save_genericList(userList, NULL, 1);
    destroy_genericList(userList);
    return 0;
}


/*vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
// 框架

void user_framework(GenericList *userList, GenericNode *userNode)
{
    // 读取登录用户的购买信息
    GenericList *purch_infoList = create_genericList(sizeof(Purch_Info));
    if(NULL == purch_infoList)
    {
        printf("    [系统]>购买记录链表创建失败！\n");
        return ;
    }
    load_genericList(purch_infoList, userNode, -1);

    User *userData = (User *)userNode->data;
    userData->purch_infor_list = purch_infoList;
    // 用户信息结点 链接 购买信息链表 完成
    // 读取彩票发行链表
    GenericList *lotteryList = create_genericList(sizeof(Lottery));
    if(NULL == lotteryList)
    {
        printf("    [系统]>彩票发行记录链表创建失败！\n");
        return ;
    }
    load_genericList(lotteryList, NULL, 2);
 
    // 更新彩票发行信息
    update_lottery_info(lotteryList);

    int mode_ctrl = -1;
    while (mode_ctrl)
    {
        if(-1 == mode_ctrl)
        {
            clearScreen();
            user_main_menu();
            printf("    [系统]>欢迎用户%s登入系统！\n", userData->user_name);
            // 购买记录更新
            update_purch_info(userData->purch_infor_list);
            // 功能界面
        }
        printf("    [系统]>彩票管理系统>请选择：");
        input_int(&mode_ctrl);

        switch (mode_ctrl)
        {
            case 0:
                break;

            case 1:  // 购买彩票
                {
                    printf("    ===================================\n");
                    user_purchase_lottery(lotteryList, userNode);
                    printf("    ===================================\n");
                    // mode_ctrl = -1;
                }
                break;

            case 2:  // 余额管理
                {
                    printf("    ===================================\n");
                    user_blance_management(userNode);
                    printf("    ===================================\n");
                    // mode_ctrl = -1;
                }
                break;

            case 3:  // 购买记录查询  期号查询，识别码查询，中奖状态查询，每注价格   所有信息按某一数据正序排序输出
                {
                    user_purchase_record(userNode);
                    mode_ctrl = -1;
                }
                break;

            case 4:  // 个人信息管理 查看 修改 注销
                {
                    if(-1 == user_self_information(userList, userNode, lotteryList))
                    {
                        return ;
                    }
                    mode_ctrl = -1;
                }
                break;

            default:
                printf("    [系统]>输入错误！！！\n");
                break;
        }
    }

    save_genericList(purch_infoList, userNode, -1);  // 购买信息链表保存
    save_genericList(lotteryList, NULL, 2);  // 发行信息链表保存
    destroy_genericList(purch_infoList);  // 购买信息链表保存后释放
    destroy_genericList(lotteryList);  // 彩票发行信息链表释放
}

void user_purchase_lottery(GenericList *lotteryList, GenericNode *userNode)
{
    // 获取当前日期
    Date temp_date = {0};
    get_date(&temp_date);
    // 输出正在发行的彩票，购买截止日期前的彩票
    int temp_int = print_lotteryList_data_to_uaer(lotteryList, &temp_date, 0);
    if (-1 == temp_int || -2 == temp_int)
    {
        return ; // 如果没有返回
    }
    // 用户输入期号
    printf("    [系统]>请输入要购买彩票的期号:");
    int input_issue = 0;
    input_int(&input_issue);
    // 查找后调用购买函数
    GenericNode *lotteryNode = find_genericlist(lotteryList, &input_issue, 1, isEqual_lottery);
    if(NULL == lotteryNode)
    {
        printf("    [系统]>没有该期号的彩票!\n");
        return ;
    }
    temp_int = 0;  // 用于判断是否开奖
    Lottery *lotterData = (Lottery *)lotteryNode->data;
    // 判断是否截至，是否已开奖
    if(compare_date(&temp_date, &lotterData->sale_deadline, 0) && ( !isEqual_date(&lotterData->sale_deadline, &temp_date, 0) ) )
    {   
        printf("    [系统]>%08d期的彩票已经停止出售！\n", input_issue);
        return ;
    }
    // else if(isEqual_lottery(lotteryNode->data, &temp_int, 2))
    // {
    //     printf("    [系统]>%08d期的彩票已经开奖！\n", input_issue);
    //     return ;
    // }
    // 结点文件读入
    GenericList *sale_infoList = create_genericList(sizeof(Sale_Info));
    load_genericList(sale_infoList, lotteryNode, -2);
    // 彩票发行结点 链接 销售信息链表 完成
    lotterData->sale_info_list = sale_infoList;
    // 调用购买函数
    if(-1 == purchase_lottery(lotteryNode, userNode))
    {
        return ;
    }
    
    save_genericList(sale_infoList, lotteryNode, -2);  // 购买完成后保存
    destroy_genericList(sale_infoList);  // 链表释放
    lotterData->sale_info_list = NULL;  // 手动置空
    printf("    [系统]>彩票购买成功!\n");  // 设置标志位，清屏后提醒上次操作结果
}

void user_blance_management(GenericNode *userNode)
{
    int input_ctrl = 0;
    User *userData = (User *)userNode->data;
    printf("    [系统]>当前余额：%.2lf￥\n", userData->balance);
    printf("    [系统]>{ [0]:退出 [1]:充值 [2]:提现 }\n");
    printf("    [系统]>余额>请选择：");
    input_int(&input_ctrl);
    if(0 == input_ctrl)
    {
        printf("    [系统]>用户已取消！\n");
        return ;
    }

    if (1 == input_ctrl)
    {
        updata_user_balance(userNode, 1);  // 充值
        printf("    [系统]>操作成功，当前余额：%.2lf￥\n", userData->balance);
    }
    else if(2 == input_ctrl)
    {
        char vCode[6] = {0};
        char input_vCode[6] = {0};
        get_verification_code(vCode, 5);
        printf("    [系统]>请输入验证码[%s]:", vCode);
        scanf("%5s", input_vCode);
        fflush(stdin);
        if (0 == strcmp(vCode, input_vCode))
        {
            if(-2 == updata_user_balance(userNode, -1))  // 提现
            {
                return ;
            }
            printf("    [系统]>操作成功，当前余额：%.2lf￥\n", userData->balance);
        }
        else
        {
            printf("    [系统]>验证码错误！！！\n", vCode);
        }
    }
}

void user_purchase_record(GenericNode *userNode)
{
    User *userData = (User *)userNode->data;
    int mode_ctrl = -1;
    while (mode_ctrl)
    {
        if(-1 == mode_ctrl)
        {
            clearScreen();
            // 功能界面
            user_purchase_record_menu();           
        }
        printf("    [系统]>彩票购买信息查询>请选择：");
        input_int(&mode_ctrl);

        switch (mode_ctrl)
        {
            case 0:
                break;

            case 1:  // 期号查询
                {
                    if (userData->purch_infor_list->head == userData->purch_infor_list->head->next)
                    {
                        printf("    [系统]>中没有彩票购买信息！！！\n");
                        break;
                    }
                    
                    int input_issue = 0;
                    printf("    [系统]>请输入查询彩票的期号（0.取消查询）：");
                    input_int(&input_issue);
                    if(0 == input_issue)
                    {
                        printf("    [系统]>用户取消查询！\n");
                        break;
                    }
                    print_purch_infoList_data(userData->purch_infor_list, &input_issue, -4);
                    // mode_ctrl = -1;
                }
                break;

            case 2:  // 识别码查询
                {
                    if (userData->purch_infor_list->head == userData->purch_infor_list->head->next)
                    {
                        printf("    [系统]>中没有彩票购买信息！！！\n");
                        break;
                    }

                    char input_lid[LENGTH_OF_LOTID + 1];
                    printf("    [系统]>请输入查询彩票的唯一识别码（0.取消查询）：");
                    scanf("%16s", input_lid);
                    fflush(stdin);
                    if(0 == strcmp(input_lid, "0"))
                    {
                        printf("    [系统]>用户取消查询！\n");
                        break;
                    }
                    print_purch_infoList_data(userData->purch_infor_list, input_lid, -1);
                    // mode_ctrl = -1;
                }
                break;

            case 3:  // 中奖状态查询
                {
                    if (userData->purch_infor_list->head == userData->purch_infor_list->head->next)
                    {
                        printf("    [系统]>中没有彩票购买信息！！！\n");
                        break;
                    }

                    int input_status = 0;  // "已开奖", "未开奖", "未中奖", "未兑换", "已兑换", "已过期"
                    printf("    [系统]>{ [0]:退出 [1]:未开奖 [2]:未中奖 [3]:未兑换 [4]:已兑换 [5]:已过期 }\n");
                    printf("    [系统]>彩票状态查询>请选择：");
                    input_int(&input_status);
                    if(0 == input_status)
                    {
                        printf("    [系统]>用户取消查询！\n");
                        break;
                    }
                    if(1 > input_status && 5 < input_status)
                    {
                        printf("    [系统]>错误状态！！！\n");
                    }
                    else
                    {
                        print_purch_infoList_data(userData->purch_infor_list, &input_status, -3);
                    }
                    // mode_ctrl = -1;
                }
                break;

            case 4:  // 排序输出
                {
                    if (userData->purch_infor_list->head == userData->purch_infor_list->head->next)
                    {
                        printf("    [系统]>中没有彩票购买信息！！！\n");
                        break;
                    }

                    int input_num1 = -1;  // 控制排序内容
                    int input_num2 = -1;  // 控制输出放向
                    printf("    [系统]>{ [0]:退出 [1]:期号 [2]:中奖状态 [3]:购买日期 [4]:每注价格 [5]:奖金 [6]:彩票唯一识别码 }\n");
                    printf("    [系统]>信息排序查询>请选择：");
                    input_int(&input_num1);
                    if(0 == input_num1)
                    {
                        printf("    [系统]>用户取消排序！\n");
                        break;
                    }
                    else if(1 > input_num1 || 5 < input_num1)
                    {
                        printf("    [系统]>错误，没有该选项！！！\n");
                    }
                    printf("    [系统]>{ [0]:退出 [1]:正序 [-1]:逆序 }\n");
                    printf("    [系统]>信息排序查询>请选择：");
                    input_int(&input_num2);
                    if(0 == input_num2)
                    {
                        printf("    [系统]>用户取消排序！\n");
                        break;
                    }
                    else if(1 != input_num2 && -1 != input_num2)
                    {
                        printf("    [系统]>错误，没有该选项！！！\n");
                    }
                    sort_genericList(userData->purch_infor_list->head, userData->purch_infor_list->head
                        , userData->purch_infor_list->head->next, userData->purch_infor_list->head->prior
                        , input_num1, compare_purch_info);

                    traverse_genericList(userData->purch_infor_list, input_num2, print_purch_info);
                    // mode_ctrl = -1;
                }
                break;

            default:
                printf("    [系统]>输入错误！！！\n");
                break;
        }
    }
}

int user_self_information(GenericList *userList, GenericNode *userNode, GenericList *lotteryList)
{
    User *userData = (User *)userNode->data;
    int mode_ctrl = -1;
    while (mode_ctrl)
    {
        
        if(-1 == mode_ctrl)
        {
            clearScreen();
            // 功能界面
            print_userNode(userNode); 
            user_self_information_menu();            
        }
        printf("    [系统]>个人信息管理>请选择：");
        input_int(&mode_ctrl);

        switch (mode_ctrl)
        {
            case 0:
                break;

            case 1:  // 修改用户名
                {
                    input_uaer_name(userList, userData);
                    // mode_ctrl = -1;
                }
                break;

            case 2:  // 修改密码
                {
                    input_user_password(userData);
                    // mode_ctrl = -1;
                }
                break;

            case 3:  // 修改其他信息
                {
                    input_user_data(userData);
                    // mode_ctrl = -1;
                }
                break;

            case 4:  // 注销
                {
                    char vCode[6];
                    char input_vCode[6];
                    get_verification_code(vCode, 5);
                    printf("    [系统]>中止注销输入[0]；确认注销请输入验证码[%s]:", vCode);
                    scanf("%5s", input_vCode);
                    fflush(stdin);
                    if(0 == strcmp(input_vCode, "0"))
                    {
                        printf("    [系统]>用户取消注销！\n");
                        break;
                    }
                    else if(0 == strcmp(input_vCode, vCode))
                    {
                        cancel_userNode(userList, userNode, lotteryList);
                        return -1;
                    }
                    printf("    [系统]>验证码错误！\n");
                }
                break;

            default:
                printf("    [系统]>输入错误！！！\n");
                break;
        }
    }
    return 0;
}

/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
/*vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
// 基础功能

/*
@ function_name: input_number_of_bets
@ argument: 
@           User *userData  ：接受输入的字符串
@           Lottery *lotteryData   ：len是字符串的有效长度，最后一位预留存放结束符，用于限制输入长度，超过长度循环输入
@ function: 输入密码，自动线隐藏，输入前会刷新stdin，自动补回车
@ renturn value: void
@ author: wang.
@ modify_time: 2025/3/22
*/
int input_number_of_bets(User *userData, Lottery *lotteryData)
{
    int number_of_bets = 0;
    while (1)
    {
        printf("    [系统]>请输入要购入几注（每张彩票最少一注，最多五注，0、取消购买）：");
        input_int(&number_of_bets);
        if(0 > number_of_bets || 5 < number_of_bets)
        {
            printf("    [系统]>输入错误！！！\n");
            continue;
        }
        else if(0 == number_of_bets)
        {
            printf("    [系统]>用户取消购买！\n");
            return -1;
        }
        else if((lotteryData->price_per_bet * (double)number_of_bets) > userData->balance )
        {
            printf("    [系统]>购买中止！\n");
            printf("    [系统]>余额不足请充值！\n");
            return -1;
        }
        else
        {
            userData->balance -= (lotteryData->price_per_bet * (double)number_of_bets);
            return number_of_bets;
        }
    }
}

void get_lot_id(Purch_Info *purch_infoData, Sale_Info *sale_infoData, Lottery *lotteryData)
{
    GenericNode *temp = NULL;
    do
    {
        sprintf(purch_infoData->lot_id, "%08d00000000", lotteryData->issue);
        get_verification_code((purch_infoData->lot_id+8), 8); 
        temp = find_genericlist(lotteryData->sale_info_list, purch_infoData->lot_id, -1, isEqual_sale_info);
    } while (NULL != temp);
    // 同步至销售信息
    strcpy(sale_infoData->lot_id, purch_infoData->lot_id);
}

void input_bet(Purch_Info *purch_infoData, Sale_Info *sale_infoData, const int number_of_bets)
{
    int bet = 0;
    int i = 0;
    for(i = 0; i < number_of_bets; i++)
    {
        int flag = 1;
        while(flag)
        {
            if(1 == flag)
            {
                printf("    [系统]>每注由6位数字组成范围从[000000]到[999999]；可以重复下相同的注\n");
                flag = 2;
            }
            printf("    [系统]>请输入第%d注：", i+1);
            input_int(& bet);

            if( 0 <= bet && 999999 >= bet)
            {
                sprintf(purch_infoData->bet[i], "%06d", bet);
                strcpy(sale_infoData->bet[i], purch_infoData->bet[i]); // 同步至销售信息
                flag = 0;
            }
            else
            {
                printf("    [系统]>第%d注：[%06d]输入错误！！！\n", i+1, bet);
                flag = 1;
            }
        }
    }
    for( i; i < 5; i++)
    {
        sprintf(purch_infoData->bet[i], "未下注");
        strcpy(sale_infoData->bet[i], purch_infoData->bet[i]);
    }
}

int purchase_lottery(GenericNode *lotteryNode, GenericNode *userNode)
{
    // 数据转化
    Lottery *lotteryData = (Lottery *)lotteryNode->data;
    User *userData = (User *)userNode->data;
    // 优先更新用户结点的信息，余额检测，下注数量的确定
    int number_of_bets = 0;

    number_of_bets = input_number_of_bets(userData, lotteryData);
    if(-1 == number_of_bets)
    {
        return -1;
    }
    
    // 确认购买后的信息赋值
    Purch_Info new_purch_info = {0};  // 新的购买信息；
    Sale_Info new_sale_info = {0};  // 新的销售信息；
    // 购买信息赋值 销售信息赋值
    new_purch_info.issue = lotteryData->issue;  // 期号赋值
    new_purch_info.lot_status = new_sale_info.lot_status = 1;  // 彩票状态
    get_date(&new_purch_info.Purch_date);
    get_date(&new_sale_info.sale_date);
    new_purch_info.exchange_data = lotteryData->exchange_data;  // 兑换时间赋值
    new_purch_info.bonus = 0;  // 初始化奖金
    new_purch_info.price = lotteryData->price_per_bet;  // 单价赋值
    // 获取彩票唯一标识及查重
    get_lot_id(&new_purch_info, &new_sale_info, lotteryData);
    // 下注
    input_bet(&new_purch_info, &new_sale_info, number_of_bets);

    new_sale_info.account_status = 1;  // 销售信息，账户状态初化为活跃
    strcpy(new_sale_info.user_id, userData->user_id);  // 销售信息，用户id赋值
    strcpy(new_sale_info.user_name, userData->user_name);  // 销售信息，用户名赋值
    // 购买信息插入链表 销售信息插入链表
    insert_genericList(userData->purch_infor_list, userData->purch_infor_list->head, &new_purch_info);
    insert_genericList(lotteryData->sale_info_list, lotteryData->sale_info_list->head, &new_sale_info);
    // 最后更新彩票结点的信息，奖池，销售量等

    lotteryData->total_bonus = lotteryData->total_bonus + ((double)number_of_bets * lotteryData->price_per_bet);
    lotteryData->sale_number += number_of_bets;

    return 0;
}

/*
@ function_name: cancel_userNode
@ argument: GenericList *userList, GenericNode *userNode
@ function: 注销彩民账户，先内后外，彩票发行后的销售信息链表由外部读入，修改后保存
@ renturn value:
@ author: wang.
@ modify_time: 2025/3/31
*/
void cancel_userNode(GenericList *userList, GenericNode *userNode, GenericList *lotteryList)
{
    User *userData = (User *)userNode->data;

    // 对彩票购买信息销毁需要先保存本次登录做的修改
    save_genericList(userData->purch_infor_list, userNode, -1);

    if(NULL != userData->purch_infor_list)
    {
        destroy_genericList(userData->purch_infor_list);
        userData->purch_infor_list = NULL;
    }

    //需要同步更新 彩票中心 该用户的 彩票购买信息
    GenericNode *current = lotteryList->head->next;
    while (lotteryList->head != current)  // 遍彩票发行信息结点
    {
        // 调整结点下，销售链表中每个结点的信息
        // 文件读入
        GenericList *sale_infoList = create_genericList(sizeof(Sale_Info));
        load_genericList(sale_infoList, current, -2);
        // 遍历
        GenericNode *cur = sale_infoList->head->next;
        while (sale_infoList->head != cur)
        {
            if(isEqual_sale_info(cur->data, userData->user_id, -3))
            {
                Sale_Info *sale_infoData = (Sale_Info *)cur->data;
                sale_infoData->account_status = 0;
            }
            cur = cur->next;
        }
        // 更新完毕后保存
        save_genericList(sale_infoList, current, -2);
        // 释放销售信息链表
        destroy_genericList(sale_infoList);
        current = current->next;
    }
    
    remove_genericList(userList, userNode);  // 删除用户结点
    printf("    [系统]>账户注销成功！\n");

}


/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/



