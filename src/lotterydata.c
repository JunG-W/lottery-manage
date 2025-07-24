/*
    日期：4月2日
    完成功能：彩票中心子系统
    1、发行彩票：完成
    2、查看发行记录：完成
    3、查询彩民信息：完成
    4、排序功能：完成
    5、保存功能：完成
    6、自定义功能：必要的查重及彩民权限限制，日期操作相关函数
*/

#include "lotterydata.h"

//  外层彩票发行信息的链表操作
/*vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
//优先完成通用链表的回调函数

/*
@ function_name: isEqual_user
@ argument: 
@       void *userData  ：传入进行相等比较的用户数据
@       void *data      ：与用户数据比较的数据(char *)
@       int n   ：控制回调函数的数据转换选择
@       [1]:期号 |[2]:开奖状态 |[3]:出售数量 |[4]:兑奖日期 |[5]:每注价格 |[6]:奖金总数 |[7]:每注奖金 |
@ function: 输出由n控制选择的学生结构体的成员
@ renturn value: int 相等为1，不等为0
@ author: wang.
@ modify_time: 2025/4/1
*/
int isEqual_lottery(void *lotteryData, void *data, int n)
{
    Lottery *lottery = (Lottery *)lotteryData;
    if(1 == n)  // 期号
    {
        int *issue = (int *)data;
        return (lottery->issue == *issue);
    }
    if(2 == n)  // 开奖状态
    {
        int *lot_status = (int *)data;
        return (lottery->lot_status == *lot_status);
    }
    if(3 == n)  // 出售数量
    {
        int *sale_number = (int *)data;
        return (lottery->sale_number == *sale_number);
    }
    if(4 == n)  // 兑奖日期
    {
        Date *exchange_data = (Date *)data;
        return (lottery->exchange_data.year == exchange_data->year
            && lottery->exchange_data.month == exchange_data->month 
            && lottery->exchange_data.day == exchange_data->day);
    }
    if(5 == n)  // 每注价格
    {
        double *price_per_bet = (double *)data;
        return (lottery->price_per_bet == *price_per_bet);
    }
    if(6 == n)  // 奖金总数
    {
        double *total_bonus = (double *)data;
        return (lottery->total_bonus == *total_bonus);
    }
    if(7 == n)  // 每注奖金
    {
        double *bonus_per_bet = (double *)data;
        return (lottery->bonus_per_bet == *bonus_per_bet );
    }

    return 0;
}

/*
@ function_name: compare_lottery
@ argument: 
@       void *lotteryDataA ：传入进行相等比较的用户数据
@       void *lotteryDataB ：传入进行相等比较的用户数据
@       int n   ：控制回调函数的数据转换选择
@       [1]:期号 |[2]:开奖状态 |[3]:出售数量 |[4]:兑奖日期 |[5]:每注价格 |[6]:奖金总数 |[7]:每注奖金 |
@ function: 输出由n控制选择的学生结构体的成员
@ renturn value: int A大于等于B为1，否则为0
@ author: wang.
@ modify_time: 2025/4/1
*/
int compare_lottery(void *lotteryDataA, void *lotteryDataB, const int n)
{
    Lottery *lotteryA = (Lottery *)lotteryDataA;
    Lottery *lotteryB = (Lottery *)lotteryDataB;
    if(1 == n)  // 期号
    {
        return (lotteryA->issue >= lotteryB->issue);
    }
    if(2 == n)  // 开奖状态
    {
        return (lotteryA->lot_status >= lotteryB->lot_status);
    }
    if(3 == n)  // 出售数量
    {
        return (lotteryA->sale_number >= lotteryB->sale_number);
    }
    if(4 == n)  // 兑奖日期
    {
        if(lotteryA->exchange_data.year > lotteryB->exchange_data.year)
        {
            return 1;
        }
        else if(lotteryA->exchange_data.year == lotteryB->exchange_data.year)
        {
            if(lotteryA->exchange_data.month > lotteryB->exchange_data.month)
            {
                return 1;
            }
            else if(lotteryA->exchange_data.month == lotteryB->exchange_data.month)
            {
                if(lotteryA->exchange_data.day >= lotteryB->exchange_data.day)
                {
                    return 1;
                }
            }
        }
        return 0; 
    }
    if(5 == n)  // 每注价格
    {
        return (lotteryA->price_per_bet >= lotteryB->price_per_bet);
    }
    if(6 == n)  // 奖金总数
    {
        return (lotteryA->total_bonus >= lotteryB->total_bonus);
    }
    if(7 == n)  // 每注奖金
    {
        return (lotteryA->bonus_per_bet >= lotteryB->bonus_per_bet );
    }
    return 0;
}

int compare_lottery_date(void *lotteryData, void *dateData, const int n)
{
    Lottery *lottery = (Lottery *)lotteryData;
    Date *date = (Date *)dateData; 
    if(lottery->sale_deadline.year > date->year)
    {
        return 1;
    }
    else if(lottery->sale_deadline.year == date->year)
    {
        if(lottery->sale_deadline.month > date->month)
        {
            return 1;
        }
        else if(lottery->sale_deadline.month == date->month)
        {
            if(lottery->sale_deadline.day >= date->day)
            {
                return 1;
            }
        }
    }
    return 0; 
}

/*
@ function_name: print_lottery
@ argument: void *lotteryData ：传入用户结点数据
@ function: 以表格形式输出用户结点数据
@ renturn value:void
@ author: wang.
@ modify_time: 2025/4/1
*/
void print_lottery(void *lotteryData)
{
    // 数据类型转
    Lottery *lottery = (Lottery *)lotteryData;
    // 期号 每注价格 出售截至日期 兑换日期 开奖状态 售出数量(按注计算) 每注奖金 奖金总数
    printf("  %08d │ %7.2lf￥  │ [%04d-%02d-%02d] │ [%04d-%02d-%02d] │  %3s  │ %8d注 │ %10.2lf￥ │ %.2lf￥ \n"
        , lottery->issue, lottery->price_per_bet
        , lottery->sale_deadline.year, lottery->sale_deadline.month, lottery->sale_deadline.day
        , lottery->exchange_data.year, lottery->exchange_data.month, lottery->exchange_data.day
        , LOTTERY_STATUS[lottery->lot_status], lottery->sale_number
        , lottery->bonus_per_bet, lottery->total_bonus);
}

void print_lotteryList_head(void)
{
    printf(" ──────────┬────────────┬──────────────┬──────────────┬──────────┬────────────┬──────────────┬───────────────\n");
    printf("    期号   │  每注价格  │ 出售截至日期 │   兑换日期   │ 开奖状态 │  售出数量  │   每注奖金   │   奖金总数 \n");
    printf(" ──────────┼────────────┼──────────────┼──────────────┼──────────┼────────────┼──────────────┼───────────────\n");
}
void print_lotteryList_tail(void)
{
    printf(" ──────────┴────────────┴──────────────┴──────────────┴──────────┴────────────┴──────────────┴───────────────\n");
}

/*
@ function_name: print_lotteryNode
@ argument: GenericNode *userNode： 需要输出的结点地址
@ function: 输出单个结点的数据，
@ renturn value: void
@ author: wang.
@ modify_time: 2025/4/1
*/
void print_lotteryNode(GenericNode *lotteryNode)
{
    print_lotteryList_head();
    print_lottery(lotteryNode->data);
    print_lotteryList_tail();
}

/*
@ function_name: print_userList
@ argument: GenericList *userList：链表控制结构体指针
@           int n：[1]、正向遍历；[-1]、逆向遍历
@ function: 遍历输出学生信息，检测是否只有头结点
@ renturn value: void
@ author: wang.
@ modify_time: 2025/4/1
*/
void print_lotteryList(GenericList *lotteryList, int n)
{
    if(lotteryList->head == lotteryList->head->next)
    {
        printf("    [系统]>没有彩票发行信息！！！\n");
        return ;
    }
    print_lotteryList_head();
    traverse_genericList(lotteryList, n, print_lottery);
    print_lotteryList_tail();
}

/*
@ function_name: print_lotteryList_Data
@ argument: 
@       GenericList *userList：链表控制结构体指针
@       void *data      ：与用户数据比较的数据
@       int n   ：控制回调函数的数据转换选择
@       [1]:期号 |[2]:开奖状态 |[3]:出售数量 |[4]:兑奖日期 |[5]:每注价格 |[6]:奖金总数 |[7]:每注奖金 |
@ function: 遍历输出由n控制选择匹配的输入数据的信息
@ renturn value:void
@ author: wang.
@ modify_time: 2025/4/2
*/
void print_lotteryList_data(GenericList *lotteryList, void *data, int n)
{
    int errorcode = 0;
    errorcode = print_genericList_data(lotteryList, data, n
        , compare_date, print_lottery, print_lotteryList_head, print_lotteryList_tail);

    if(-1 == errorcode)
    {
        printf("    [系统]>没有彩票发行信息！！！\n");
    }
    else if(-2 == errorcode)
    {
        printf("    [系统]>没有相关的彩票发行信息！！！\n");
    }
}


void print_lotteryList_head_to_uaer(void)
{
    printf(" ──────────┬────────────┬──────────────┬──────────────┬────────────\n");
    printf("    期号   │  每注价格  │ 出售截至日期 │   兑换日期   │ 开奖状态  \n");
    printf(" ──────────┼────────────┼──────────────┼──────────────┼────────────\n");
}
void print_lotteryList_tail_to_uaer(void)
{
    printf(" ──────────┴────────────┴──────────────┴──────────────┴────────────\n");
}
/*
@ function_name: print_lotteryList_Data
@ argument: GenericNode *userNode： 需要输出的结点地址
@ function:用于用户购买时的信息展示，因此要特化输出信息，
@ renturn value:void
@ author: wang.
@ modify_time: 2025/4/2
*/
void print_lottery_to_uaer(void *lotteryData)
{
    // 数据类型转
    Lottery *lottery = (Lottery *)lotteryData;
    // 期号 每注价格 出售截至日期 兑换日期 开奖状态
    printf("  %08d │ %7.2lf￥  │ [%04d-%02d-%02d] │ [%04d-%02d-%02d] │  %3s  \n"
        , lottery->issue, lottery->price_per_bet
        , lottery->sale_deadline.year, lottery->sale_deadline.month, lottery->sale_deadline.day
        , lottery->exchange_data.year, lottery->exchange_data.month, lottery->exchange_data.day
        , LOTTERY_STATUS[lottery->lot_status]);
}

int print_lotteryList_data_to_uaer(GenericList *lotteryList, void *data, int n)
{
    int errorcode = 0;
    errorcode = print_genericList_data(lotteryList, data, n
        , compare_lottery_date, print_lottery_to_uaer, print_lotteryList_head_to_uaer, print_lotteryList_tail_to_uaer);

    if(-1 == errorcode)
    {
        printf("    [系统]>没有彩票发行信息！！！\n");
        return -1;
    }
    else if(-2 == errorcode)
    {
        printf("    [系统]>没有相关的彩票发行信息！！！\n");
        return -2;
    }
}

/*
@ function_name: get_lottery_issue
@ argument:GenericList *lotteryList：传入链表用于查重
@ function: 由年月和两位随机数自动生成8位彩票期号如果重复重新生成，需要在调用之前载入随机数种子
@ renturn value:int 返回期号
@ author: wang.
@ modify_time: 2025/4/8
*/
int get_lottery_issue(GenericList *lotteryList)
{
    // 获得时间
    time_t nowtime;
    time( &nowtime );
    // 转为时间结构体tm
    struct tm *time_info;
    time_info = localtime(&nowtime);
    // 其他变量初始化
    GenericNode *lotteryNode = NULL;
    int issue = 0;
    do
    {
        issue = 0;
        // 生成期号
        issue = (time_info->tm_year + 1900) * 10000 + (time_info->tm_mon + 1) * 100 + get_random_num(0, 99);
        // 重复检测
        lotteryNode = find_genericlist(lotteryList, &issue, 1, isEqual_lottery);
        // 判断
    } while (NULL != lotteryNode);

    return issue;
}

/*
@ function_name: input_lottery__data
@ argument: const int offset：用户输入时间长度（天，不包括发行当天）
@ function: 以当前时间为基准由用户输入的时间长度计算出日期
@ renturn value:int 日期
@ author: wang.
@ modify_time: 2025/4/1
*/
Date get_lottery_data(const int offset)
{
    Date date = {0};
    get_date(&date);
    int remain_days = offset;

    // 闰年检测，月份天数确定
    while(1)
    {
        int mdays = get_days(date.year, date.month);
        int add_days = mdays-date.day;
        if(remain_days <= add_days)
        {
            date.day += remain_days;
            return date;
        }
        else
        {
            remain_days = remain_days - add_days;
            date.day = 0;
            if(12 == date.month)
            {
                date.year++;
                date.month = 1;
            }
            else
            {
                date.month++;
            }
        }
    }        
}

void get_lottery_win_bet(Lottery *lotteryData)
{
    sprintf(lotteryData->win_bet,"%06d\0", get_random_num(0000000, 999999));
}


/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/








//  内层销售信息的链表操作
/*vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
//优先完成通用链表的回调函数

/*
@ function_name: isEqual_sale_info
@ argument: 
@       void *purch_infoData  ：传入进行相等比较的用户数据
@       void *data      ：与用户数据比较的数据
@       int n   ：控制回调函数的数据转换选择
@       [-5]:中奖状态 |[-4]:账户状态 |[-3]:UID |[-2]:用户名 |[-1]:lot_id |[0~4]:下的注 |
@ function: 输出由n控制选择的学生结构体的成员
@ renturn value: int 相等为1，不等为0
@ author: wang.
@ modify_time: 2025/3/31
*/
int isEqual_sale_info(void *sale_infoData, void *data, int n)
{
    Sale_Info *sale_info = (Sale_Info *)sale_infoData;
    if(-5 == n)
    {
        int *lot_status = (int *)data;
        return (sale_info->lot_status == *lot_status);
    }
    if(-4 == n)
    {
        int *account_status = (int *)data;
        return (sale_info->account_status == *account_status);
    }
    char *str = (char *)data;
    if(-3 == n)
    {
        return ( 0 == strcmp(sale_info->user_id, str) );
    }
    if(-2 == n)
    {
        return ( 0 == strcmp(sale_info->user_name, str) );
    }
    if(-1 == n)
    {
        return ( 0 == strcmp(sale_info->lot_id, str) );
    }
    if(0 <= n && 4 >= n)
    {
        return ( 0 == strcmp(sale_info->bet[n], str) );
    }
    return 0;
}

/*
@ function_name: compare_sale_info
@ argument: 
@       void *purch_infoDataA ：传入进行相等比较的数据
@       void *purch_infoDataB ：传入进行相等比较的数据
@       int n   ：控制回调函数的数据转换选择
@       [1]:中奖状态 |[2]:账户状态 |[3]:UID |[4]:用户名 |[5]:lot_id |
@ function: 输出由n控制选择的学生结构体的成员
@ renturn value: int A大于等于B为1，否则为0
@ author: wang.
@ modify_time: 2025/3/31
*/
int compare_sale_info(void *sale_infoDataA, void *sale_infoDataB, const int n)
{
    Sale_Info *sale_infoA = (Sale_Info *)sale_infoDataA;
    Sale_Info *sale_infoB = (Sale_Info *)sale_infoDataB;
    if(1 == n)
    {
        return (sale_infoA->lot_status >= sale_infoB->lot_status);
    }
    if(2 == n)
    {
        return (sale_infoA->account_status >= sale_infoB->account_status);
    }
    if(3 == n)
    {
        return ( 0 <= strcmp(sale_infoA->user_id, sale_infoB->user_id) );
    }
    if(4 == n)
    {
        return ( 0 <= strcmp(sale_infoA->user_name, sale_infoB->user_name) );
    }
    if(5 == n)
    {
        return ( 0 <= strcmp(sale_infoA->lot_id, sale_infoB->lot_id) );
    }
    return 0;
}

void print_sale_info(void *sale_infoData)
{
    // 数据类型转
    Sale_Info *sale_info = (Sale_Info *)sale_infoData;
    // 期号 状态 日期 日期 奖金 标识 注
    printf("\n\
        vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv\n\
         LID:[%-16s]\t[%s]\t购买日期:[%04d-%02d-%02d]\n\
        ──────────────────────────────────────────────────────────────────────\n\
         用户名:[%-20s]  UID:[%10s]  账户状态:[%s]\n\
        ──────────────────────────────────────────────────────────────────────\n\
           A: [%s]  B: [%s]  C: [%s]  D: [%s]  E: [%s] \n\
        ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\n"
    , sale_info->lot_id, LOTTERY_STATUS[sale_info->lot_status]
    , sale_info->sale_date.year, sale_info->sale_date.month, sale_info->sale_date.day
    , sale_info->user_name, sale_info->user_id, ACCOUNT_STATUS[sale_info->account_status]
    , sale_info->bet[0]
    , sale_info->bet[1]
    , sale_info->bet[2]
    , sale_info->bet[3]
    , sale_info->bet[4] );
}

void print_sale_infoListl(GenericList *sale_infoList, int n)
{
    if(-1 == traverse_genericList(sale_infoList, n, print_sale_info))
    {
        printf("    [系统]>系统中没有彩票销售信息！！！\n");
        return ;
    }
}

void print_sale_infoList_data(GenericList *sale_infoList, void *data, int n)
{
    int errorcode = 0;
    errorcode = print_genericList_onlyData(sale_infoList, data, n, isEqual_sale_info, print_sale_info);

    if(-1 == errorcode)
    {
        printf("    [系统]>系统中没有彩票销售信息！！！\n");
    }
    else if(-2 == errorcode)
    {
        printf("    [系统]>没有所查找的彩票销售信息！！！\n");
    }
}

/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/


//  内外层链表的公共操作部分
/*vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/

/*
@ function_name: publish_lotteryNode
@ argument: GenericList *lotteryList：传入彩票发行链表用于查重和插入
@ function: 发行新的彩票
@ renturn value:void
@ author: wang.
@ modify_time: 2025/4/2
*/
void publish_lotteryNode(GenericList *lotteryList)
{
    Lottery newLottery = {0};
    newLottery.sale_info_list = NULL;
    newLottery.bonus_per_bet = 0;
    newLottery.lot_status = 1;  // "已开奖", "未开奖", "未中奖", "未兑换", "已兑换", "已过期"
    newLottery.sale_number = 0;
    newLottery.total_bonus = 0.0;
    double price = 0.0;
    while(1)
    {
        printf("    [系统]>请输入本期彩票每注价格(0、取消发行）：");
        input_double(&price);
        if(0 > price)
        {
            printf("    [系统]>价格必须大于0！！！\n");
            continue;
        }
        else if(0 == price) // if(-0.00001 < price && 0.00001 > price)
        {
            printf("    [系统]>彩票发行取消！\n");
            return ;
        }
        else
        {   
            newLottery.price_per_bet = price;
            break;
        }
    }
    // 时间设置
    int exchange_date_offset = 0;
    int sale_date_offset = 0;
    while(1)
    {
        printf("    [系统]>请输入本期售卖天数(0、取消发行）：");
        input_int(&sale_date_offset);
        if(0 > sale_date_offset || 365 < sale_date_offset)
        {
            printf("    [系统]>天数必须大于0且最大不得超过365！！！\n");
            continue;
        }
        else if(0 == sale_date_offset)
        {
            printf("    [系统]>彩票发行取消！\n");
            return ;
        }
        else
        {
            newLottery.sale_deadline = get_lottery_data(sale_date_offset);
            break;
        }    
    }
    while(1)
    {
        printf("    [系统]>请输入本期彩票售卖截止后几天开奖兑换(0、取消发行）：");
        input_int(&exchange_date_offset);
        if(0 > exchange_date_offset || 7 < sale_date_offset)
        {
            printf("    [系统]>天数必须大于0且最大不得超过7！！！\n");
            continue;
        }
        else if(0 == exchange_date_offset)
        {
            printf("    [系统]>彩票发行取消！\n");
            return ;
        }
        else
        {
            newLottery.exchange_data = get_lottery_data(exchange_date_offset + sale_date_offset);
            break;
        }
    }
    // 期号
    newLottery.issue = get_lottery_issue(lotteryList);
    get_lottery_win_bet(&newLottery);
    
    GenericList *sale_info_list = NULL;
    sale_info_list = create_genericList(sizeof(Purch_Info));
    if(NULL == sale_info_list)
    {
        printf("    [系统]>警告：彩票销售信息链表创建失败！\n");
    }

    newLottery.sale_info_list = sale_info_list;

    insert_genericList(lotteryList, lotteryList->head, &newLottery);
    save_genericList(lotteryList, NULL, 2);
    printf("    [系统]>彩票发行成功！\n");
}

void update_lottery_info(GenericList *lotteryList)
{
    Date nowDate;
    get_date(&nowDate);

    GenericNode *current = lotteryList->head->next;

    while (lotteryList->head != current)
    {
        Lottery *lotteryData = (Lottery *)current->data;
        int temp_int0 = 1;
        // 检测未开奖的发行记录，
        if( isEqual_lottery(current->data, &temp_int0, 2) )
        {
            // 与当前日期比较更新彩票状态，先找出未开奖，但是时间已过的结点（包括开奖当天）
            if ( compare_date(&nowDate, &lotteryData->exchange_data, 0) )
            {
                // 调整结点状态为已开奖
                lotteryData->lot_status = 0;
                if ( !isEqual_lottery(current->data, &nowDate, 4))  // 如果过日期不是开奖当天，则说明已过期
                {
                    // 调整结点下，销售链表中每个结点的信息
                    // 文件读入
                    GenericList *sale_infoList = create_genericList(sizeof(Sale_Info));
                    load_genericList(sale_infoList, current, -2);
                    // 遍历
                    GenericNode *cur = sale_infoList->head->next;
                    while (sale_infoList->head != cur)
                    {   // 定义临时变量查询，彩票购买信息状态
                        int temp_int1 = 1;  // "已开奖", "未开奖", "未中奖", "未兑换", "已兑换", "已过期"
                        int temp_int2 = 3;
                        // 查询未开奖和未兑换的信息
                        if(isEqual_sale_info(cur->data, &temp_int1, -5) || isEqual_sale_info(cur->data, &temp_int2, -5))  // 找未开奖的、未兑换的
                        {
                            Sale_Info *sale_infoData = (Sale_Info *)cur->data;
                            sale_infoData->lot_status = 5;  // 更新状态为过期
                        }
                        cur = cur->next;
                    }
                    save_genericList(sale_infoList, current, -2);  // 更新完毕后保存
                    destroy_genericList(sale_infoList);  // 释放销售信息链表
                } 
            } 
        }
        current = current->next;
    }
}

/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/




/*
    gcc -o .\bin\lottery -Iinclude .\src\genericlist.c .\src\publicdata.c .\src\datafile.c .\src\lotterydata.c
    .\bin\lottery
*/
// #include "datafile.h"
// int main(void)
// {
//     // 彩票发行信息链表测试
//     GenericList *lotterylist = create_genericList(sizeof(Lottery));
//     Lottery lotteryData= {20250401, 1, 10, {0, 0, 0}, {0, 0, 0}, "012345", 2.5, 61182.5, 12.5};
//     publish_lotteryNode(lotterylist);
//     lotteryData.sale_info_list = NULL;
    
//     get_date(&lotteryData.exchange_data);
//     get_date(&lotteryData.sale_deadline);

//     insert_genericList(lotterylist, lotterylist->head, &lotteryData);

//     print_lotteryList(lotterylist ,1);
//     // print_lotteryNode_to_uaer(lotterylist->head->next);
//     Lottery *l1 = (Lottery *)lotterylist->head->next->data;
//     Lottery *l2 = (Lottery *)lotterylist->head->next->next->data;
//     printf("%s:%s", l1->win_bet, l2->win_bet);
//     // 彩票销售信息链表测试
//     GenericList *sale_info_list = create_genericList(sizeof(Sale_Info));
//     Lottery *test = (Lottery *)lotterylist->head->next->data;
//     test->sale_info_list = sale_info_list;
//     Sale_Info sale_infoData = {1, 1, "9999999999", "99999999999999999999", {0, 0, 0}, "2025040212345678", {"012345","012345","012345","012345","012345"}};
//     get_date(&sale_infoData.sale_date);
    
//     insert_genericList(test->sale_info_list, test->sale_info_list->head, &sale_infoData);
//     print_sale_infoListl(test->sale_info_list, 1);

//     return 0;
// }