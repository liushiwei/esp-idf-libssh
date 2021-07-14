#include "array_list.h"
#include "command.h"
#include "esp_log.h"
void innversion_arr(struct Arr *pArr)//倒置
{
    int i = 0;

    int j = pArr->cnt - 1;

    cmd_tbl_t * t;

    while (i<j)

    {

        t = pArr->pBase[i];

        pArr->pBase[i] = pArr->pBase[j];

        pArr->pBase[j] = t;

        ++i;
        --j;

    }

    return;
}

void sort_arr(struct Arr *pArr)//排序,冒泡排序，比较相邻的2个元素大小，再替换位置

{

    int i, j;
    void * t;

    for (i = 0; i < pArr->cnt; ++i)

    {

        for (j = i + 1; j < pArr->cnt; ++j)

        {

            if (pArr->pBase[i] > pArr->pBase[j])

            {

                t = pArr->pBase[i];

                pArr->pBase[i] = pArr->pBase[j];

                pArr->pBase[j] = t;

            }

        }

    }
}

_Bool insert_arr(struct Arr *pArr, int pos, cmd_tbl_t * val) {      //pos的值从1开始
    int  i;
    if (is_full(pArr))
        return false;

    if (pos<1 || pos>pArr->cnt + 1)    //判断要插入的位置是否小于1 或者大于有效数组个数加一的(就是不连续的插入位置)  返回false
        return false;


    for (i = pArr->cnt - 1; i >= pos - 1; --i)//插入数据时从最后一个开始到pos-1数组的位置向后面移动一个位置
    {
        pArr->pBase[i + 1] = pArr->pBase[i];//pos位置之后的向后移动一个位置

    }

    pArr->pBase[pos - 1] = val; //插入新增的数据

    (pArr->cnt)++; //有效个数加一
    return true;
}

_Bool delete_arr(struct Arr *pArr, int pos) {
    int i;
    if (is_empty(pArr))
        return false;//指针为空
    if (pos<1 || pos > pArr->cnt)
        return false;//没有这个位置

    for (i = pos; i<pArr->cnt; ++i)
    {
        pArr->pBase[i - 1] = pArr->pBase[i];//pos位置之后的向前移动一个位置

    }

    (pArr->cnt)--; //删除一个元素后有效个数减去一个
    return true;
}

_Bool append_arr(struct Arr *pArr, cmd_tbl_t * val)//追加，可能成功，可能失败
{



    //满时返回false

    if (is_full(pArr))

        return false;

    //不满时追加

    pArr->pBase[pArr->cnt] = val;

    (pArr->cnt)++;

    return true;

}
_Bool is_full(struct Arr *pArr)//是否满了
{

    if (pArr->cnt == pArr->len) {
        ESP_LOGI("", "array list is full!\n");
        return true;

    }
    else
    {

        return false;

    }



}


void show_arr(struct Arr *pArr)//显示
{

    //      if(数组为空)

    //               提示用户数组为空

    //      else

    //               输出数组有效内容

    if (is_empty(pArr))    //

    {
        ESP_LOGI("",  "array list is empty!\n");

    }

    else

    {

        for (int i = 0; i<pArr->cnt; i++)

            ESP_LOGI("",  "%s\n", pArr->pBase[i]->name);

       ESP_LOGI("",  " \n");



    }

}

_Bool is_empty(struct Arr *pArr)//是否为空
{
    if (0 == pArr->cnt)

        return true;

    else

        return false;



}

void init_arr(struct Arr *pArr, int length) {
    //分配初始化数组
    // pArr->pBase = (void *)malloc(sizeof(void *)*length);
    // if (NULL == pArr->pBase)

    // {
    //     ESP_LOGI("",  "free error\n");

    //     exit(-1);//终止整个程序
    // }

    // else

    // {
    //     pArr->len = length;

    //     pArr->cnt = 0;

    //     ESP_LOGI("",  "free \n");

    // }

    return;

}