// gcc -o .\obj\datafile.o -Iinclude -c .\src\datafile.c 
#include "datafile.h"

#ifdef _WIN32
    char *relativePath[]={"data\\test.bin","data\\user.bin","data\\lottery.bin"};
#else
    char *relativePath[]={"data\\test.bin","data/user.bin","data/lottery.bin"};
#endif

/*
@ function_name: get_DataPath
@ argument: 
@       char *fullPath  ：完整路径的字符指针
@       GenericNode *genericNode：用于确定文件名
@       int n           ：控制文件的选择
@       [-2]彩票销售记录|[-1]用户购买记录|[0]测试文件|[1]用户文件|[2]彩票文件
@ function: 获取完整的数据存储路径，取决于可执行文件在那个路径下，支持不同系统环境
@ renturn value: int 
@ author: wang.
@ modify_time: 2025/4/1
*/
int get_dataPath(char *fullPath, GenericNode *genericNode, int n)
{
    char *lastSlash = NULL;

    if(n < -2 || 2 < n)
    {
        return -1;
    }
    #ifdef _WIN32

        static char path[MAX_PATH];
        GetModuleFileName(NULL, path, MAX_PATH);
        // 找到 程序路径中最后一个'\\'
        lastSlash = strrchr(path,'\\');
        if(lastSlash != NULL)
        {
            // 替换为\0将其截断
            *lastSlash = '\0';
            lastSlash = NULL;
        }
        lastSlash = strrchr(path,'\\');
        if(lastSlash != NULL)
        {
            // 由于是二级目录因此截断两次
            *lastSlash = '\0';
            lastSlash = NULL;
        }
        // 将截断后的路径和二级目录拼接形成完整路径；
        switch (n)
        {
            case 2:
            case 1:
            case 0:
                snprintf(fullPath, MAX_PATH, "%s\\%s", path, relativePath[n]);
                break;

            case -1:
                {
                    User *userData = (User *)genericNode->data;
                    snprintf(fullPath, MAX_PATH, "%s\\data\\user\\%s.bin", path, userData->user_id);
                }
                break;

            case -2:
                {
                    Lottery *LotData = (Lottery *)genericNode->data;
                    char filename[10];
                    sprintf(filename,"%d", LotData->issue);
                    snprintf(fullPath, MAX_PATH, "%s\\data\\lottery\\%s.bin", path, filename);
                }
                break;
        
            default:
                break;
        }
    #else
        static char path[PATH_MAX];
    
        ssize_t len =readlink("/proc/self/exe", path, PATH_MAX);
        if(len != -1)
        {
            path[len] = '\0';
            lastSlash = strrchr(path,'/');
            if(lastSlash != NULL)
            {
                *lastSlash = '\0';
                lastSlash = NULL;
            }
            lastSlash = strrchr(path,'/');
            if(lastSlash != NULL)
            {
                *lastSlash = '\0';
                lastSlash = NULL;
            }
        }

        switch (n)
        {
            case 2:
            case 1:
            case 0:
                snprintf(fullPath, MAX_PATH, "%s\\%s", path, relativePath[n]);
                break;

            case -1:
                {
                    User *userData = (User *)genericNode->data;
                    snprintf(fullPath, MAX_PATH, "%s\\data\\user\\%s.bin", path, userData->user_id);
                }
                break;

            case -2:
                {
                    Lottery *LotData = (User *)genericNode->data;
                    char filename[10];
                    sprintf(filename,"%d", LotData->issue);
                    snprintf(fullPath, MAX_PATH, "%s\\data\\lottery\\%s.bin", path, filename);
                }
                break;
        
            default:
                break;
        }
    #endif
    
    return 0;    
}

/*
@ function_name: get_DataPath
@ argument: 
@       GenericList *genericList：当加载嵌套链表时，应输入该结点下的链表指针
@       GenericNode *genericNode：当加载结点下嵌套的链表时使用，若普通加载输入NULL
@       int n           ：控制文件的选择
@       [-2]彩票销售记录|[-1]用户购买记录|[0]测试文件|[1]用户文件|[2]彩票文件
@ function: 加载通用链表，或结点中的链表
@ renturn value: int 
@ author: wang.
@ modify_time: 2025/4/1
*/
int load_genericList(GenericList *genericList, GenericNode *genericNode, int n)
{
    FILE* listData = NULL;
    
    #ifdef _WIN32
        char fullPath[MAX_PATH];
        get_dataPath(fullPath, genericNode, n);
        listData = fopen(fullPath, "rb");
    #else
        char fullPath[PATH_MAX];
        get_DataPath(fullPath, n);
        listData = fopen(fullPath,"rb");
    #endif

    if(NULL == listData)
    {
        printf("打开失败\n");
        return -1;
    }

    void *data = malloc(genericList->data_size);  // 用于接收文件中的数据
    if(NULL == data)
    {
        printf("临时内存分配失败\n");
        return -2;
    }
    // 当没有有效数据时读取结束
    while(0 < fread(data, genericList->data_size, 1, listData))
    {
        if(1 == n)  // 外层链表读取时需要手动将内层链表的控制指针置为空
        {
            User *userData = (User *)data;
            userData->purch_infor_list = NULL;
        }
        if(2 == n)  // 外层链表读取时需要手动将内层链表的控制指针置为空
        {
            Lottery *lotterData = (Lottery *)data;
            lotterData->sale_info_list = NULL;
        }
        // 插入时内存分配可能出错[-2]彩票销售记录|[-1]用户购买记录|[0]测试文件|[1]用户文件|[2]彩票文件
        int errorCode = insert_genericList(genericList, genericList->head, data);
        if(-1 == errorCode)
        {
            free(data);
            fclose(listData);
            return -3;
        }
        else if(-2 == errorCode)
        {
            free(data);
            fclose(listData);
            return -4;
        }
    }

    free(data);
    fclose(listData);
    return 0; 
}

/*
@ function_name: get_DataPath
@ argument: 
@       GenericList *genericList：当保存嵌套链表时，应输入该结点下的链表指针
@       GenericNode *genericNode：当保存结点下嵌套的链表时使用，若普通加载输入NULL
@       int n           ：控制文件的选择
@       [-2]彩票销售记录|[-1]用户购买记录|[0]测试文件|[1]用户文件|[2]彩票文件
@ function: 保存通用链表，或结点中的链表
@ renturn value: int 
@ author: wang.
@ modify_time: 2025/4/1
*/
int save_genericList(GenericList *genericList, GenericNode *genericNode, int n)
{
    FILE* listData = NULL;

    #ifdef _WIN32
        char fullPath[MAX_PATH];
        get_dataPath(fullPath, genericNode, n);
        listData = fopen(fullPath, "wb");
    #else
        char fullPath[PATH_MAX];
        get_DataPath(fullPath, n);
        listData = fopen(fullPath,"wb");
    #endif

    if(NULL == listData)
    {
        printf("创建失败\n");
        return -1;
    }
    // 定义通用结点指针用于遍历
    GenericNode *current = genericList->head->next;
    // 正向遍历遇到头结点，遍历结束
    while(genericList->head != current)
    {
        fwrite(current->data, genericList->data_size, 1, listData);
        current = current->next;
    }

    fclose(listData);
    return 0; 
}

// // 功能测试成功
// void print_int(void *data)
// {
//     int *intData = (int *)data;
//     printf("[Data]:%d\n", *intData);
// }
// /*
//     gcc -o .\bin\gl .\src\genericlist.c .\src\datafile.c -Iinclude
//     .\bin\gl
// */
// int main(int argc, char **argv)
// {
//     // 测试初始化、插入、输出
//     GenericList *intList = create_genericList(sizeof(int));
//     int errorcode = load_genericList("test.bin", intList, 0);
//     if(-1 == errorcode)
//     {
//         printf("文件读取失败\n");
//         for(int i  = 10; i > 0 ; i--)
//         {
//             insert_genericList(intList, intList->head, &i);
//         }
//     }
//     else if(-2 == errorcode)
//     {
//         printf("内存分配失败\n");
//     }
    
//     traverse_genericList(intList, 1, print_int);
//     printf("\n");

//     // 测试保存
//     errorcode = save_genericList("test.bin", intList, 0);
//     if(-1 == errorcode)
//     {
//         printf("文件创建失败\n");
//     }

//     return 0;
// }