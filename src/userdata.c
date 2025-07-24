/*
    日期：4月1日
    完成功能：（彩民子系统）
    1、用户注册：完成；
    2、用户登陆：完成；
    3、查看用户个人信息：完成；
    4、用户修改密码：完成；
    5、充值：完成；
    6、注销：完成；
    7、自定义类型：
*/

#include "userdata.h"

//  外层彩民的链表操作
/*vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
//优先完成通用链表的回调函数

/*
@ function_name: isEqual_user
@ argument: 
@       void *userData  ：传入进行相等比较的用户数据
@       void *data      ：与用户数据比较的数据(char *)
@       int n   ：控制回调函数的数据转换选择
@       [1]:UID |[2]:用户名 |[3]:真实姓名 |[4]:电话号码 |
@ function: 输出由n控制选择的学生结构体的成员
@ renturn value: int 相等为1，不等为0
@ author: wang.
@ modify_time: 2025/3/31
*/
int isEqual_user(void *userData, void *data, int n)
{
    User *user = (User *)userData;
    char *str = (char *)data;
    if(1 == n)  // UID
    {
        return (0 == strcmp(user->user_id, str) );
    }
    if(2 == n)  // 用户名
    {
        return (0 == strcmp(user->user_name, str) );
    }
    if(3 == n)  // 真实姓名
    {
        return (0 == strcmp(user->real_name, str) );
    }
    if(4 == n)  // 电话号码
    {
        return (0 == strcmp(user->phone_number, str) );
    }
    return 0;
}

/*
@ function_name: compare_user
@ argument: 
@       void *userDataA ：传入进行相等比较的用户数据
@       void *userDataB ：传入进行相等比较的用户数据
@       int n   ：控制回调函数的数据转换选择
@       [1]:UID |[2]:用户名 |[3]:真实姓名 |[4]:电话号码 |[5]:余额 |
@ function: 输出由n控制选择的学生结构体的成员
@ renturn value: int A大于等于B为1，否则为0
@ author: wang.
@ modify_time: 2025/3/31
*/
int compare_user(void *userDataA, void *userDataB, const int n)
{
    User *userA = (User *)userDataA;
    User *userB = (User *)userDataB;
    if(1 == n)  // UID
    {
        return (0 <= strcmp(userA->user_id, userB->user_id) );
    }
    if(2 == n)  // 用户名
    {
        return (0 <= strcmp(userA->user_name, userB->user_name) );
    }
    if(3 == n)  // 真实姓名
    {
        return (0 <= strcmp(userA->real_name, userB->real_name) );
    }
    if(4 == n)  // 电话号码
    {
        return (0 <= strcmp(userA->phone_number, userB->phone_number) );
    }
    if(5 == n)  // 余额
    {
        return (userA->balance >= userB->balance);
    }
    return 0;
}

/*
@ function_name: print_user
@ argument: void *userData ：传入用户结点数据
@ function: 以表格形式输出用户结点数据
@ renturn value:void
@ author: wang.
@ modify_time: 2025/4/1
*/
void print_user(void *userData)
{
    // 数据类型转
    User *user = (User *)userData;
    // uid 用户名 密码 真实姓名 电话号码 余额 购买总数
    printf("  %-10s │ %-12s │ %-10s │ %-12s │ %-12s │ %.2lf￥ \n"
        , user->user_id, user->user_name, user->password
        , user->real_name, user->phone_number
        , user->balance);
}

void print_userList_head(void)
{
    printf(" ────────────┬──────────────┬────────────┬──────────────┬──────────────┬───────────────\n");
    printf("     UID     │   用 户 名   │   密  码   │    姓  名    │   电话号码   │ 账 户 余 额     \n");
    printf(" ────────────┼──────────────┼────────────┼──────────────┼──────────────┼───────────────\n");
}
void print_userList_tail(void)
{
    printf(" ────────────┴──────────────┴────────────┴──────────────┴──────────────┴───────────────\n");
}

/*
@ function_name: print_userNode
@ argument: GenericNode *userNode： 需要输出的结点地址
@ function: 输出单个用户的信息，
@ renturn value: void
@ author: wang.
@ modify_time: 2025/4/1
*/
void print_userNode(GenericNode *userNode)
{
    print_userList_head();
    print_user(userNode->data);
    print_userList_tail();
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
void print_userList(GenericList *userList, int n)
{
    if(userList->head == userList->head->next)
    {
        printf("    [系统]>没有用户信息！！！\n");
        return ;
    }
    print_userList_head();
    traverse_genericList(userList, n, print_user);
    print_userList_tail();
}

/*
@ function_name: print_userList_Data
@ argument: 
@       GenericList *userList：链表控制结构体指针
@       void *data      ：与用户数据比较的数据(char *)
@       int n   ：控制回调函数的数据转换选择
@       [1]:UID |[2]:用户名 |[3]:真实姓名 |[4]:电话号码 |
@ function: 遍历输出由n控制选择匹配的输入数据的用户信息
@ renturn value:void
@ author: wang.
@ modify_time: 2025/4/2
*/
void print_userList_data(GenericList *userList, void *data, int n)
{
    int errorcode = 0;
    errorcode = print_genericList_data(userList, data, n
        , isEqual_user, print_user, print_userList_head, print_userList_tail);

    if(-1 == errorcode)
    {
        printf("    [系统]>系统中没有用户信息！！！\n");
    }
    else if(-2 == errorcode)
    {
        printf("    [系统]>没有所查找的用户的信息！！！\n");
    }
}

/*
@ function_name: get_uaer_id
@ argument: 
@       GenericList *userList：传入用户链表用于UID查重
@       User *userData：插入结点的数据域
@ function: 由随机数自动生成10位UID如果重复重新生成
@ renturn value:void
@ author: wang.
@ modify_time: 2025/4/1
*/
void get_uaer_id(GenericList *userList, User *userData)
{
    GenericNode *temp = NULL;
    char user_id[LENGTH_OF_UID + 1];
    do
    {
        int id = get_random_num(0, 2111111111);
        sprintf(user_id,"%010d", id);
        temp = NULL;
        temp = find_genericlist(userList, user_id, 1, isEqual_user);
    } while (NULL != temp);
    
    strcpy(userData->user_id, user_id);
}

/*
@ function_name: input_uaer_name
@ argument: 
@       GenericList *userList：传入用户链表用于查重
@       User *userData：插入结点的数据域
@ function: 用户名输入，查重，合法性检测
@ renturn value:int [-1]用户中止输入
@ author: wang.
@ modify_time: 2025/4/8
*/
int input_uaer_name(GenericList *userList, User *userData)
{
    char user_name[LENGTH_OF_UNAME + 1];
    int flag = 1;
    while (flag)
    {
        printf("    [系统]>用户名设置要求：长度大于6且小于20个字符宽度;\n\
                           其中一个中文字符或标点占2个字符宽度;\n");
        printf("    [系统]>请输入用户名（0、退出）：");
        scanf("%21s", user_name);
        fflush(stdin);
        if(0 == strcmp(user_name, "0"))
        {
            printf("    [系统]>用户取消！\n");
            return -1;
        }
        int i = 0;
        while('\0' != user_name[i++]) ;
        if(6 > (i-1) || LENGTH_OF_UNAME < (i-1) )
        {
            printf("    [系统]>用户名\"%s\"长度不合法，请重新输入！\n", user_name);
            flag = 2;
        }
        else
        {
            GenericNode *temp = NULL;
            temp = find_genericlist(userList, user_name, 2, isEqual_user);  // 查重，若无重复应返回NULL；
            if(NULL != temp)
            {
                printf("    [系统]>用户名\"%s\"已被占用，请重新输入！\n", user_name);
                flag = 3;
            }
            else
            {
                strcpy(userData->user_name, user_name);
                printf("    [系统]>用户名设置成功！\n");
                flag = 0;
            }    
        }
    }

    return 0;
}

/*
@ function_name: input_user_password
@ argument:User *userData：插入结点的数据域
@ function: 密码设置，合法性检测
@ renturn value:int [-1]用户中止输入
@ author: wang.
@ modify_time: 2025/4/1
*/
int input_user_password(User *userData)
{
    char password1[LENGTH_OF_UPASS + 1];
    char password2[LENGTH_OF_UPASS + 1];
    int flag = 0;
    do
    {
        flag = 0;
        printf("    [系统]>密码设置要求：长度大于等于8位且小于等于20位;\n\
                         密码只能由数字、大小写字母组成，且必须包含数字、大小写字母;\n");
        printf("    [系统]>请输入密码（0、退出）：");
        input_passward(password1, LENGTH_OF_UPASS);
        if(0 == strcmp(password1, "0"))
        {
            printf("    [系统]>用户取消！\n");
            return -1;
        }
        // 密码合法性检测
        flag = check_user_password(password1, 8, 20);
        if(-1 == flag)
        {
            printf("    [系统]>密码设置错误：密码只能由数字、大小写字母组成，请重新输入！\n");
            continue;
        }
        else if(-2 == flag)
        {
            printf("    [系统]>密码设置错误：密码必须包含数字、大小写字母，请重新输入！\n");
            continue;
        }
        else if(-3 == flag)
        {
            printf("    [系统]>密码设置错误：密码长度错误，请重新输入！\n");
            continue;
        }

        printf("    [系统]>请再次输入密码（0、退出）：");
        input_passward(password2, LENGTH_OF_UPASS);
        if(0 == strcmp(password2, "0"))
        {
            printf("    [系统]>用户取消注册！\n");
            return -1;
        }
        if(0 != strcmp(password1, password2) )
        {
            printf("    [系统]>两次密码输入不一致，请重新输入！\n");
            flag = 1;
        } 
    } while (flag);
    printf("    [系统]>密码设置成功！\n");
    strcpy(userData->password, password1);
    return 0;
}

/*
@ function_name: input_user_data
@ argument:User *userData：插入结点的数据域
@ function: 用户名输入其他信息，真实姓名，电话号码（只有简单的11位检测）
@ renturn value:int [-1]用户中止输入
@ author: wang.
@ modify_time: 2025/4/8
*/
int input_user_data(User *userData)
{
    int flag = 1;
    char phone_num[12];
    printf("    [系统]>请输入您的真实姓名：");
    scanf("%20s", userData->real_name);
    fflush(stdin);
    while (flag)
    {
        flag = 0;
        printf("    [系统]>请输入您的手机号（0、退出）：");
        scanf("%11s", phone_num);
        fflush(stdin);
        if(0 == strcmp(phone_num, "0"))
        {
            printf("    [系统]>用户取消！\n");
            return -1;
        }
        int i = 0;
        while ('\0' != phone_num[i])
        {
            if ('0' > phone_num[i] || '9' < phone_num[i])
            {
                break;
            }
            i++;
        }
        if(11 != i )
        {
            printf("    [系统]>请输入正确的11位手机号！\n");
            flag = 1;
        }
        else
        {
            strcpy(userData->phone_number, phone_num);
            flag = 0;
        }
    } 
    userData->balance = 0.0;
}

/*
@ function_name: updata_user_balance
@ argument: 
@       GenericNode *userNode：传入用户链表用于查重
@       const int n：[1]、充值 [2]、提现
@ function: 
@ renturn value:void
@ author: wang.
@ modify_time: 2025/4/6
*/
int updata_user_balance(GenericNode *userNode, const int n)
{
    User *user = (User *)userNode->data;
    double money = 0.0;

    if (1 == n)
    {
        while(1)
        {
            printf("    [系统]>请输入您要充值的金额，每次充值限额：5000；（0、取消操作）：");
            input_double(&money);
            if(0 > money || 5000 < money)
            {
                printf("    [系统]>您的输入有误！\n");
            }
            else if (0 == money)
            {
                printf("    [系统]>用户取消充值！\n");
                return -2;
            }
            else
            {
                printf("    [系统]>充值成功！\n");
                user->balance += money;
                return 0;
            }
        }
    }
    else if (-1 == n)
    {
        while(1)
        {
            printf("    [系统]>请输入您要提现的金额（0、取消操作）：");
            input_double(&money);
            if(0 > money)
            {
                printf("    [系统]>您的输入有误！\n");
            }
            else if (0 == money)
            {
                printf("    [系统]>用户取消提现！\n");
                return -1;
            }
            else
            {
                if (user->balance >= money)
                {
                    printf("    [系统]>提现成功！\n");
                    user->balance -= money;
                    return 0;
                }
                else
                {
                    printf("    [系统]>您的余额不足！\n");
                    return -2;
                }
            }
        }
    }
}

/*
@ function_name: login
@ argument: 
@           user *head          ：用户链表头结点
@           const int mode      ：登陆角色
@           const char *userid  ：登录用户
@ function: 用户登录，限制登录次数为4次，超过退出登录函数
@ renturn value: GenericNode *:成功登录返回用户对应的结点
@ author: wang.
@ modify_time: 2025/4/1
*/
GenericNode *login(GenericList *userList)
{
    int counter = 4;
    char user_name[LENGTH_OF_UNAME + 1];
    char user_password[LENGTH_OF_UPASS+1];
    GenericNode *userNode = NULL;
    User *user = NULL;
    while(counter)
    {
        printf("    [系统]>请输入用户名（0、中止登录）：");
        scanf("%20s", user_name);
        fflush(stdin);
        if(0 == strcmp(user_name, "0"))
        {
            printf("    [系统]>用户取消登录！\n");
            return NULL;
        }
        userNode = find_genericlist(userList, user_name, 2, isEqual_user);
        if(NULL == userNode)
        {
            printf("    [系统]>用户%s不存在，请先注册！\n", user_name);
            counter--;
            printf("    [系统]>还有%d次机会！！！\n", counter);
            continue;
        }
        
        user = (User *)userNode->data;
        printf("    [系统]>请输入密码：");
        input_passward(user_password, LENGTH_OF_UPASS);

        if( !strcmp(user->password, user_password) )
        {
            char get_verif_code[6];
            char input_verif_code[6];
            get_verification_code(get_verif_code, 5);
            printf("    [系统]>请输入验证码[%s]：", get_verif_code);
            scanf("%5s", input_verif_code);
            fflush(stdin);
            if( 0 == strcmp(get_verif_code, input_verif_code) )
            {
                printf("    [系统]>欢迎！！！\n");
                return userNode;
            }
            else
            {
                printf("    [系统]>验证码错误！！！\n");
                counter--;
                printf("    [系统]>还有%d次机会！！！\n", counter);
            }
        }
        else
        {
            printf("    [系统]>密码错误！！！\n");
            counter--;
            printf("    [系统]>还有%d次机会！！！\n", counter);
        }
         
    }
    return NULL;    
}


/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/


//  内层购买信息的链表操作
/*vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
//优先完成通用链表的回调函数

/*
@ function_name: isEqual_user
@ argument: 
@       void *purch_infoData  ：传入进行相等比较的用户数据
@       void *data      ：与用户数据比较的数据
@       int n   ：控制回调函数的数据转换选择
@       [-4]:期号 |[-3]:中奖状态 |[-2]:兑换日期 [-1]:lot_id |[0~4]:下的注 |
@ function: 输出由n控制选择的学生结构体的成员
@ renturn value: int 相等为1，不等为0
@ author: wang.
@ modify_time: 2025/3/31
*/
int isEqual_purch_info(void *purch_infoData, void *data, int n)
{
    Purch_Info *purch_info = (Purch_Info *)purch_infoData;
    if(-4 == n)
    {
        int *issue = (int *)data;
        return (purch_info->issue == *issue);
    }
    if(-3 == n)
    {
        int *lot_status = (int *)data;
        return (purch_info->lot_status == *lot_status);
    }
    if(-2 == n)
    {
        Date *exchange_data = (Date *)data;
        return (purch_info->exchange_data.year == exchange_data->year 
            && purch_info->exchange_data.month == exchange_data->month  
            && purch_info->exchange_data.day == exchange_data->day );
    }
    if(-1 == n)
    {
        char *lot_id = (char *)data;
        return ( 0 == strcmp(purch_info->lot_id, lot_id) );
    }
    if(0 <= n && 4 >= n)
    {
        char *bet = (char *)data;
        return ( 0 == strcmp(purch_info->bet[n], bet) );
    }
    return 0;
}

/*
@ function_name: compare_purch_info
@ argument: 
@       void *purch_infoDataA ：传入进行相等比较的数据
@       void *purch_infoDataB ：传入进行相等比较的数据
@       int n   ：控制回调函数的数据转换选择
@       [1]:期号 |[2]:中奖状态 |[3]:购买日期 |[4]:每注价格 |[5]:奖金 |[6]:lot_id |
@ function: 输出由n控制选择的学生结构体的成员
@ renturn value: int A大于等于B为1，否则为0
@ author: wang.
@ modify_time: 2025/3/31
*/
int compare_purch_info(void *purch_infoDataA, void *purch_infoDataB, const int n)
{
    Purch_Info *purch_infoA = (Purch_Info *)purch_infoDataA;
    Purch_Info *purch_infoB = (Purch_Info *)purch_infoDataB;
    if(1 == n)
    {
        return (purch_infoA->issue >= purch_infoB->issue);
    }
    if(2 == n)
    {
        return (purch_infoA->lot_status >= purch_infoB->lot_status);
    }
    if(3 == n)
    {
        if(purch_infoA->Purch_date.year > purch_infoB->Purch_date.year)
        {
            return 1;
        }
        else if(purch_infoA->Purch_date.year == purch_infoB->Purch_date.year)
        {
            if(purch_infoA->Purch_date.month > purch_infoB->Purch_date.month)
            {
                return 1;
            }
            else if(purch_infoA->Purch_date.month == purch_infoB->Purch_date.month)
            {
                if(purch_infoA->Purch_date.day >= purch_infoB->Purch_date.day)
                {
                    return 1;
                }
            }
        }
        return 0;
    }
    if(4 == n)
    {
        return (purch_infoA->price >= purch_infoB->price);
    }
    if(5 == n)
    {
        return (purch_infoA->bonus >= purch_infoB->bonus);
    }
    if(6 == n)
    {
        return ( 0 <= strcmp(purch_infoA->lot_id, purch_infoB->lot_id) );
    }
    return 0;
}

void print_purch_info(void *purch_infoData)
{
    // 数据类型转
    Purch_Info *purch_info = (Purch_Info *)purch_infoData;
    // 期号 状态 日期 日期 奖金 标识 注
    printf("\n\
        vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv\n\
         期号:[%08d] LID:[%-16s] [%s] 奖金:[%.2lf￥]\n\
        ──────────────────────────────────────────────────────────────────────\n\
          购买日期：[%04d-%02d-%02d] 兑奖日期：[%04d-%02d-%02d] 每注价格:[%.2lf￥]\n\
        ──────────────────────────────────────────────────────────────────────\n\
           A: [%s]  B: [%s]  C: [%s]  D: [%s]  E: [%s]   \n\
        ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\n"
    , purch_info->issue, purch_info->lot_id, LOTTERY_STATUS[purch_info->lot_status], purch_info->bonus
    , purch_info->Purch_date.year, purch_info->Purch_date.month, purch_info->Purch_date.day
    , purch_info->exchange_data.year, purch_info->exchange_data.month, purch_info->exchange_data.day
    , purch_info->price
    , purch_info->bet[0]
    , purch_info->bet[1]
    , purch_info->bet[2]
    , purch_info->bet[3]
    , purch_info->bet[4] );
}

void print_purch_infoListl(GenericList *purch_infoList, int n)
{
    if(purch_infoList->head == purch_infoList->head->next)
    {
        printf("    [系统]>系统中没有彩票销售信息！！！\n");
        return ;
    }
    traverse_genericList(purch_infoList, n, print_purch_info);
}

/*
@ function_name: print_purch_infoList_data
@ argument: 
@       GenericList *purch_infoList：链表控制结构体
@       void *data      ：与用户数据比较的数据
@       int n   ：控制回调函数的数据转换选择
@       [-4]:期号 |[-3]:中奖状态 |[-2]:兑换日期 [-1]:lot_id |[0~4]:下的注 |
@ function: 输出由n控制选择的学生结构体的成员
@ renturn value: int 相等为1，不等为0
@ author: wang.
@ modify_time: 2025/3/31
*/
void print_purch_infoList_data(GenericList *purch_infoList, void *data, int n)
{
    int errorcode = 0;
    errorcode = print_genericList_onlyData(purch_infoList, data, n, isEqual_purch_info, print_purch_info);

    if(-1 == errorcode)
    {
        printf("    [系统]>系统中没有彩票购买信息！！！\n");
    }
    else if(-2 == errorcode)
    {
        printf("    [系统]>没有所查找的彩票购买信息！！！\n");
    }
}

void update_purch_info(GenericList *purch_infoList)
{
    Date nowDate;
    get_date(&nowDate);

    int counter[4] = {0}; // 简单记录几张已过期，几张已经到开奖日期，几张未到开奖日期和几张未兑换的彩票，
    GenericNode *current = purch_infoList->head->next;

    while (purch_infoList->head != current)
    { 
        int temp_int0 = 1;  // 临时变量 "已开奖", "未开奖", "未中奖", "未兑换", "已兑换", "已过期"
        // 检测未开奖的购买记录，
        if(isEqual_purch_info(current->data, &temp_int0, -3))
        {
            Purch_Info *purch_infoData = (Purch_Info *)current->data;
            if( isEqual_date(&purch_infoData->exchange_data, &nowDate, 0) )  // 判断是否到开奖日期
            {
                counter[1]++;
            }
            else if(compare_date(&nowDate, &purch_infoData->exchange_data, 0) )  // 判断是否已经超期
            {
                counter[0]++;
                purch_infoData->lot_status = 5;  // 超期修改状态
            }
            else  // 未到日期
            {
                counter[2]++;
            }
        }
        temp_int0 = 3;  // 检测未兑换 "已开奖", "未开奖", "未中奖", "未兑换", "已兑换", "已过期"
        if(isEqual_purch_info(current->data, &temp_int0, -3))
        {
            Purch_Info *purch_infoData = (Purch_Info *)current->data;
            if( isEqual_date(&purch_infoData->exchange_data, &nowDate, 0) )  // 判断是否到开奖日期
            {
                counter[3]++;
            }
            else if(compare_date(&nowDate, &purch_infoData->exchange_data, 0) )  // 判断是否已经超期
            {
                counter[0]++;
                purch_infoData->lot_status = 5;  // 超期修改状态
            }

        }
        // 打印公告提醒用户
        current = current->next;
    }

    char *temp[] = {"已过期！！！", "已到开奖日期！","未到开奖日期。","未兑换，请及时去公证处兑奖！！！"};
    for (int i = 0; i < 4; i++)
    {
        if (0 < counter[i])
        {
            printf("    [公告]>您有%d张彩票%s\n", counter[i], temp[i]);
        }
    }
}


/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/


//  内外层链表的公共操作部分
/*vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/

/*
@ function_name: enroll_userNode
@ argument: 
@       GenericList *userList：传入用户链表用于查重
@       User *userData：插入结点的数据域
@ function: 注册，即用户结点插入链表的完整功能，包括对购买信息链表的创建
@ renturn value:void
@ author: wang.
@ modify_time: 2025/4/1
*/
void enroll_userNode(GenericList *userList)
{
    User userData = {0};
    userData.purch_infor_list = NULL;
    
    if(-1 == input_uaer_name(userList, &userData) ) { return; }
    if(-1 == input_user_password(&userData) ) { return; }
    if(-1 == input_user_data(&userData) ) { return; }
    get_uaer_id(userList, &userData);

    GenericList *purch_info_list = create_genericList(sizeof(Purch_Info));
    if(NULL == purch_info_list)
    {
        printf("    [系统]>警告：彩票购买信息链表创建失败！\n");
    }

    userData.purch_infor_list = purch_info_list;

    insert_genericList(userList, userList->head, &userData);
    printf("    [系统]>用户\"%s\"注册成功！\n", userData.user_name);
}


/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/


/*
    gcc -o .\bin\user -Iinclude .\src\genericlist.c .\src\publicdata.c .\src\datafile.c .\src\userdata.c
    .\bin\user
*/
// #include "datafile.h"
// int main(void)
// {
//     User user = {"0123456789", "0123456789", "0123456789", "0123456789", "0123456789", 0.0 };

//     GenericList *userList = create_genericList(sizeof(User));
//     insert_genericList(userList, userList->head, &user);

//     Purch_Info purch_info1 = {20250401, 1, {0, 0, 0}, {0, 0, 0}, 1125.0, 10000000.0, "2025040112345678", {"012345", "012345", "012345", "012345", "012345"}};
//     Purch_Info purch_info2 = {20250401, 1, {0, 0, 0}, {0, 0, 0}, 2.5, 15.0, "2025040198765432", {"012345", "012345", "012345", "012345", "012345"}};

//     GenericList *purch_infoList = create_genericList(sizeof(Purch_Info));
    
//     get_date(&purch_info1.sale_date);
//     get_date(&purch_info1.exchange_data);
//     insert_genericList(purch_infoList, purch_infoList->head, &purch_info1);

//     get_date(&purch_info2.sale_date);
//     get_date(&purch_info2.exchange_data);
//     insert_genericList(purch_infoList, purch_infoList->head, &purch_info2);

//     User *test = (User *)userList->head->next->data;
//     test->purch_infor_list = purch_infoList;


//     load_genericList(test->purch_infor_list, userList->head->next, -1);

//     print_userNode(userList->head->next);

//     print_purch_info(test->purch_infor_list->head->next->data);
//     print_purch_info(test->purch_infor_list->head->next->next->data);

//     // input_uaer_name(userList, &user);
//     // input_user_password(&user);

//     save_genericList(test->purch_infor_list, userList->head->next, -1);


//     return 0;
// }