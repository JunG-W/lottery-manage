#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <stdio.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <limits.h>
    #include <unistd.h>
#endif

void clearScreen(void);
void user_login_menu(void);
void user_main_menu(void);
void user_purchase_record_menu(void);
void user_self_information_menu(void);

void lottery_main_menu(void);
void lottery_info_menu(void);
void lottery_user_menu(void);

void office_user_menu(void);


#endif
