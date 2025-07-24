/*
@ file_name: datafile.h
@ function: 
@ 实现循环双向通用链表数据的存储
@ 通过程序目录的定位，确保数据文件路径准确，备用路径文件创建位置由终端所在目录决定；
@ author: wang.
@ modify_time: 2025/4/1
*/
#ifndef __DATAFILE_H__
#define __DATAFILE_H__

#include <stdio.h>
#include <stdlib.h>
#include "publicdata.h"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <limits.h>
    #include <unistd.h>
#endif

// 加载通用链表
int load_genericList(GenericList *genericList, GenericNode *genericNode, int n);

// 保存通用链表
int save_genericList(GenericList *genericList, GenericNode *genericNode, int n);

#endif
