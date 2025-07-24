#include "display.h"

/*
 ┌ ┬ ┐├┼┤└┴┘┌─┐││└─┘─━┄┅┈┉│┃┆┇┊┋┏┳┓┣╋┫┗┻┛┏━┓┃┃┗━┛┎┰┒┠╂┨┖┸┚┍┯┑┝┿┥┕┷┙╔╦╗╠╬╣╚╩╝╔═╗...
 */

void clearScreen(void)
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    
    // printf("\e[1;1H\e[2J");
    //用八进制表示："\033[1;1H\033[2J"
    //用十六进制表示："\x1B[1;1H\x1B[2J"
    /* 

    \e：表示转义字符（ASCII码为27，即ESC），通常用于启动终端控制序列。
        注意：在C语言中，\e 是GCC等编译器的扩展写法，
        更标准的写法是使用八进制（\033）或十六进制（\x1B）。例如：\033[1;1H\033[2J。
    [1;1H：将光标移动到第1行第1列（即屏幕左上角）。
        H 是光标定位命令，格式为 [行号;列号H。
    [2J：清除整个屏幕。
        J 是清屏命令，参数 2 表示清除整个屏幕内容。
    
    */
}

void user_login_menu(void)
{
    printf("┌──────────────────────────────────────────┐\n");
    printf("│                                          │\n");
    printf("│          欢迎使用彩票管理系统！          │\n");
    printf("│                                          │\n");
    printf("│               1、 登录                   │\n");
    printf("│               2、 注册                   │\n");
    printf("│               0、 退出                   │\n");
    printf("│                                          │\n");
    printf("└──────────────────────────────────────────┘\n");
}

void user_main_menu(void)
{
    printf("┌──────────────────────────────────────────┐\n");
    printf("│                                          │\n");
    printf("│           欢迎使用彩票管理系统！         │\n");
    printf("│                                          │\n");
    printf("│              1、  购买彩票               │\n");
    printf("│              2、  余额管理               │\n");
    printf("│              3、  购买记录               │\n");
    printf("│              4、个人信息管理             │\n");
    printf("│              0、  退出登录               │\n");
    printf("│                                          │\n");
    printf("└──────────────────────────────────────────┘\n");
}

void user_purchase_record_menu(void)
{
    printf("┌──────────────────────────────────────────┐\n");
    printf("│                                          │\n");
    printf("│              彩票购买信息查询            │\n");
    printf("│                                          │\n");
    printf("│              1、彩票期号查询             │\n");
    printf("│              2、 识别码查询              │\n");
    printf("│              3、彩票状态查询             │\n");
    printf("│              4、信息排序查询             │\n");
    printf("│              0、  返  回                 │\n");
    printf("│                                          │\n");
    printf("└──────────────────────────────────────────┘\n");
}

void user_self_information_menu(void)
{
    printf("┌──────────────────────────────────────────┐\n");
    printf("│                                          │\n");
    printf("│             个 人 信 息 管 理            │\n");
    printf("│                                          │\n");
    printf("│              1、 修改用户名              │\n");
    printf("│              2、  修改密码               │\n");
    printf("│              3、修改其他信息             │\n");
    printf("│              4、  注  销                 │\n");
    printf("│              0、  返  回                 │\n");
    printf("│                                          │\n");
    printf("└──────────────────────────────────────────┘\n"); 
}

void lottery_main_menu(void)
{
    printf("┌──────────────────────────────────────────┐\n");
    printf("│                                          │\n");
    printf("│             彩票中心管理系统             │\n");
    printf("│                                          │\n");
    printf("│          1、    彩票发行                 │\n");
    printf("│          2、发行信息及销售记录           │\n");
    printf("│          3、彩民信息及购买记录           │\n");
    printf("│          0、     退  出                  │\n");
    printf("│                                          │\n");
    printf("└──────────────────────────────────────────┘\n");
}

void lottery_info_menu(void)
{
    printf("┌──────────────────────────────────────────┐\n");
    printf("│                                          │\n");
    printf("│            发行信息及销售记录            │\n");
    printf("│                                          │\n");
    printf("│          1、所有发行彩票的记录           │\n");
    printf("│          2、某期彩票的销售信息           │\n");
    printf("│          0、     退  出                  │\n");
    printf("│                                          │\n");
    printf("└──────────────────────────────────────────┘\n");
}

void lottery_user_menu(void)
{
    printf("┌──────────────────────────────────────────┐\n");
    printf("│                                          │\n");
    printf("│            彩民信息及购买记录            │\n");
    printf("│                                          │\n");
    printf("│           1、  所有用户信息              │\n");
    printf("│           2、 用户的购买记录             │\n");
    printf("│           0、    退  出                  │\n");
    printf("│                                          │\n");
    printf("└──────────────────────────────────────────┘\n");
}

void office_user_menu(void)
{
    printf("┌──────────────────────────────────────────┐\n");
    printf("│                                          │\n");
    printf("│                公  证  处                │\n");
    printf("│                                          │\n");
    printf("│              1、  开  奖                 │\n");
    printf("│              2、  兑  奖                 │\n");
    printf("│              3、往期中奖记录             │\n");
    printf("│              0、  退  出                 │\n");
    printf("│                                          │\n");
    printf("└──────────────────────────────────────────┘\n");
}

