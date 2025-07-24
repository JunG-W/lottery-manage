// gcc -o .\obj\publicdata.o -Iinclude -c .\src\publicdata.c 
#include "publicdata.h"

char *LOTTERY_STATUS[] = {"已开奖", "未开奖", "未中奖", "未兑换", "已兑换", "已过期"};
char *ACCOUNT_STATUS[] = {"注销", "活跃"};

int compare_date(void *date_A, void *date_B, const int n)
{
    Date *dateA = (Date *)date_A;
    Date *dateB = (Date *)date_B; 
    if(dateA->year > dateB->year)
    {
        return 1;
    }
    else if(dateA->year == dateB->year)
    {
        if(dateA->month > dateB->month)
        {
            return 1;
        }
        else if(dateA->month == dateB->month)
        {
            if(dateA->day >= dateB->day)
            {
                return 1;
            }
        }
    }
    return 0; 
}

int isEqual_date(void *date_A, void *date_B, const int n)
{
    Date *dateA = (Date *)date_A;
    Date *dateB = (Date *)date_B; 
    return (dateA->year == dateB->year
        && dateA->month == dateB->month 
        && dateA->day == dateB->day);
}

/*
@ function_name: input_passward
@ argument: 
@           char *password  ：接受输入的字符串
@           const int len   ：len是字符串的有效长度，最后一位预留存放结束符，用于限制输入长度，超过长度循环输入
@ function: 输入密码，自动线隐藏，输入前会刷新stdin，自动补回车
@ renturn value: void
@ author: wang.
@ modify_time: 2025/3/22
*/
void input_passward(char *password, const int len)
{
    int i = 0;
    char ch;

    fflush(stdin);
    while ((ch = getch()) != '\r') {  // '\r' 表示回车键
        if (ch == '\b') 
        {  // 处理退格键
            if (i > 0) 
            {
                i--;
                printf("\b \b");  // 删除最后一个星号并回退光标
            }
        } 
        else 
        {
            password[i%len] = ch;  // 取模防止写入超限
            if(i < len)
            {
                printf("*");
            }
            i++;
            if(len+1 == i)  // 预留一位存放字符串结束符'\0'
            {
                while(2 < i)  // 循环删除到还有一个星号
                {
                    printf("\b \b");
                    i--;
                }
                i = 1;
            }
        }
    }
    printf("\n");
    password[i] = '\0';  // 结束字符串
}

/*
@ function_name: delUserNode_role
@ argument: 
@           GenericList *userList   ：用户链表头结点
@           GenericList *studentList：学生链表头结点
@           int (*isEqual_func)(void *, void *, int)：查重用回调函数
@ function: 将学生信息同步至用户，同步后的密码将重置
@ renturn value: void
@ author: wang.
@ modify_time: 2025/3/30
*/
// void user_inf_syn(GenericList *userList, GenericList *studentList)
// {    
//     GenericNode *stuNode = studentList->head->next;
//     GenericNode *userNode = NULL;
//     if(studentList->head == stuNode)
//     {
//         printf("    [系统]>没有学生信息无需同步！！！\n");
//         return;
//     }
//     while(studentList->head != stuNode)
//     {
//         Student *stu = (Student *)stuNode->data;
//         userNode = find_genericlist(userList, stu->id, -1, isEqual_user);
//         if(NULL == userNode)
//         {
//             User newUser = {0};
//             strcpy(newUser.userId, stu->id);
//             strcpy(newUser.password, DEFAULT_PWD);
//             newUser.role = 2;
//             insert_genericList(userList, userList->head, &newUser);
//         }
//         else
//         {
//             userNode = NULL;
//         }
//         stuNode = stuNode->next;
//     }
// }

void input_int(int *num)
{
    while(1 != scanf("%d", num))
    {
        printf("    [系统]>输入错误！！！\n");
        printf("    [系统]>请根据提示输入：");
        fflush(stdin);
    }
    fflush(stdin);
}

void input_double(double *num)
{
    while(1 != scanf("%lf", num))
    {
        printf("    [系统]>输入错误！！！\n");
        printf("    [系统]>请根据提示输入：");
        fflush(stdin);
    }
    fflush(stdin);
}

void get_verification_code(char *verif_code, int n)
{
    // 0、1易与o、l混淆不使用
    char str[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    srand(time(NULL));
    for(int i = 0; i < n; i++)
    {
        verif_code[i] = str[rand() % 62];
    }
    verif_code[n] = '\0';
}

void get_date(Date *date)
{
    time_t nowtime;
    time( &nowtime );
    struct tm *time_info;
    time_info = localtime(&nowtime);

    date->year = time_info->tm_year + 1900;
    date->month = time_info->tm_mon + 1;
    date->day = time_info->tm_mday;
}

/*
@ function_name: input_passward
@ argument: int n, int m  ：生成随机数的范围
@ function: 生成从n到m的随机数，需要手动设置随机数种子srand(time(NULL));防止在循环时生成随机数失效
@ renturn value: void
@ author: wang.
@ modify_time: 2025/4/1
*/
int get_random_num(int n, int m)
{
    if( n > m)
    {
        m ^= n;
        n ^= m;
        m ^= n;
    }

    return (rand() % (m - n + 1) ) + n;
}
/*
@ function_name: input_passward
@ argument: char *password, int n, int m  ：密码的限制最短和最长
@ function: 检测密码是否合法；由数字，大小写字母共同组成。
@ renturn value: int [0]合法|[-1]出现其他字符|[-2]字符种类不足|[-3]长度不符合要求|
@ author: wang.
@ modify_time: 2025/4/1
*/
int check_user_password(char *password, int n, int m)
{
    if(n > m)  // 保证n和m大小关系
    {
        n ^= m;
        m ^= n;
        n ^= m;
    }

    int count[4] = {0};  // 数字，大写，小写，总和
    int i = 0;
    while('\0' != password[i])
    {   
        if('0' <= password[i] && '9' >= password[i])
        {
            count[0]++;
            count[3]++;
            // printf("数字：%d:%d\n", count[0], count[3]);
        }
        else if('A' <= password[i] && 'Z' >= password[i])
        {
            count[1]++;
            count[3]++;
            // printf("大写：%d:%d\n", count[1], count[3]);
        }
        else if('a' <= password[i] && 'z' >= password[i])
        {
            count[2]++;
            count[3]++;
            // printf("小写：%d:%d\n", count[2], count[3]);
        }
        else
        {
            return -1;
        }
        i++;
    }

    if(0 == count[0] || 0 == count[1] || 0 == count[2])
    {
        return -2;
    }
    if( n <= count[3] && count[3] <= m )
    {
        return 0;
    }

    return -3;
}

int is_leapYear(int year)
{
    return ( ((0 == year % 4) && (0 != year % 100)) || (0 == year % 400) );
}

int get_days(int year, int month)
{
    if( 0 < month && 13 > month)
    {
        if(2 == month)
        {
            if(is_leapYear(year))
            {
                return 29;
            }
            else
            {
                return 28;
            }
        }
        else if(0 == month % 2)
        {
            return 30;
        }
        else if(1 == month % 2)
        {
            return 31;
        }
    }
    else 
    {
        return -1;
    }   
}


/*
    gcc -o .\bin\publicdata -Iinclude .\src\publicdata.c
    .\bin\publicdata
*/
// int main(void)
// {
//     time_t nowtime;
//     time( &nowtime );
//     struct tm *time_info;
//     time_info = localtime(&nowtime);

//     printf("夏令时：%d 第%03d天 周%d\n"
//     , time_info->tm_isdst, time_info->tm_yday, time_info->tm_wday + 1);

//     printf("[%04d-%02d-%02d %02d:%02d:%02d]\n"
//     , time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday
//     , time_info->tm_hour, time_info->tm_min, time_info->tm_sec);

//     printf("当前时间：%s\n", ctime(&nowtime));

//     return 0;
// }
