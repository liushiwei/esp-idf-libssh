#ifndef __CMD_ARRAY_H
#define __CMD_ARRAY_H
#include "command.h"
#define false  0
#define true 1
struct Arr
{
    cmd_tbl_t *pBase[0];//存储的是数组第一个元素的地址
    int len;//数组能容纳的最大元素的个数

    int cnt;//有效数组个数
            //自动增长因子

} ;
void show_arr(struct Arr *pArr);//显示,分号不能省
void init_arr(struct Arr *pArr, int length);//初始化数组

_Bool is_empty(struct Arr *pArr);//判断是否为空数组

_Bool append_arr(struct Arr *pArr, cmd_tbl_t *val);//追加，可能成功，可能失败
_Bool is_full(struct Arr *pArr);//是否满了

_Bool delete_arr(struct Arr *pArr, int pos);//删除某个元素

_Bool insert_arr(struct Arr *pArr, int pos, cmd_tbl_t * val);     // 插入一条数据 pos的值从1开始

void sort_arr(struct Arr *pArr);//排序

void innversion_arr(struct Arr *pArr);//倒置

#endif