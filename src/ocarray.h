#ifndef _LUG_OC_VECTOR_CPH
#define _LUG_OC_VECTOR_CPH

#include "ocdatautils.h"

typedef struct ocarray ocarray;



OCAPI ocarray* ocarray_create();
OCAPI ocarray* ocarray_create_with_size(int size);
OCAPI void ocarray_destory(ocarray** arr);

//增
/**
 * @brief 向数组尾部追加元素
 * @param arr 数组
 * @param data 要追加的元素
 * @return 
 */
OCAPI void ocarray_pushback(ocarray* arr, void* data);


/**
 * @brief 向数组中添加若干个元素
 * @param arr 要操作的数组
 * @param datas 添加的数据，要以NULL 结尾
 * @return -1表述添加失败，其原因为内存申请失败
 */
OCAPI void ocarray_puts(ocarray* arr, void* datas, ...);
/**
 * @brief 在数组指定索引处插入元素，原位置的元素后移一位
 * @param arr 数组
 * @param index 索引
 * @param data 元素
 * @return void
 */
OCAPI int ocarray_insert(ocarray* arr, int index, void* data);

//删
/**
 * @brief 删除数组最后一个元素
 * @param arr 数组
 */
OCAPI void ocarray_popback(ocarray* arr);
/**
 * @brief 删除数组指定索引的元素
 * @param arr 要操作的数组
 * @param index 索引
 * @return index越界，返回-1
 */
OCAPI int ocarray_erase(ocarray* arr, int index);

//遍历
/**
 * @brief 获取数组指定位置的值
 * @param arr 操作的数组
 * @param index 索引
 * @return 对应索引的值
 */
OCAPI void* ocarray_at(ocarray* arr, int index);

/**
 * @brief 获取数组当前大小
 * @param arr 进行操作的数组
 * @return 数组的大小
 */
OCAPI int ocarray_size(ocarray* arr);

//辅助函数
OCAPI void ocarray_bind_del(ocarray* arr, ocdelf del);


/**
 * @brief 指定索引，截取数组，形成一个新的数组. 0指向第一个元素，索引可以为负值，-1和越界指向最后一个元素 
 * @param arr 要操作的数组
 * @param begin 开始截取的地方
 * @param end 结束的地方
 * @param copy 数据拷贝函数，利用该函数拷贝原数组的数据，若为空，则新数组仍用原数组数据，新数组的数据清理函数会被置空
 * @return 新的数组，要使用ocarray_destory释放内存
 */
OCAPI ocarray* ocarray_sub(ocarray* arr, int begin, int end, occopyf copy);

/**
 * @brief 合并两个数组
 * @param arr1 数组1
 * @param arr2 数组2
 * @param copy 数据拷贝函数，若为空，会直接使用要合并的数组内的元素，并释放源数组资源
 * @return 新的数组
 */
OCAPI ocarray* ocarray_cat(ocarray* arr1, ocarray* arr2, occopyf copy);


/**
 * @brief 合并多个数组
 * @param copy 源数组元素拷贝到新数组的方法，若为空，则原数组会被销毁
 * @param arrs 要合并的数组，可变参数，以NULL结束
 * @return 合并的新数组
 */
OCAPI ocarray* ocarray_merge(occopyf copy, ocarray* arrs, ...);



#endif //_LUG_OC_VECTOR_CPH