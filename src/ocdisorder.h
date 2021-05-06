#ifndef _LUG_OC_DISORDER_ARRAY_CPH
#define _LUG_OC_DISORDER_ARRAY_CPH

#include "ocdatautils.h"

/**
 * 自定义的数据结构，跟静态链表原理相似
 * 描述: 线性空间大小动态增长的，当储存满后，重新分配内存并拷贝原有数据, 数据未满，插入较为高效。删除指定位置的值直接将该位置标记为空，不移动数据，较为高效
 * 优点：数据未满时，插入较为高效，任何情况下，删除效率较高。遍历速度较为高效
 * 缺点：无法确定数据放入的先后顺序，且数据存储量不得高于65535
 * 
*/

#define OC_DISORDER_NOPOS -1

typedef struct ocdisorder ocdisorder;
typedef void(*ocdisorder_del)(void*);
typedef int(*ocdisorder_visitor)(void*);

//创建，删除
ocdisorder* ocdisorder_create();
void ocdisorder_destory(ocdisorder** list);

//增
void ocdisorder_push(ocdisorder* list, void* data);

//删
int ocdisorder_erase(ocdisorder* list, int index);

//遍历
int ocdisorder_first(ocdisorder* list);
int ocdisorder_last(ocdisorder* list);
int ocdisorder_prev(ocdisorder* list, int index);
int ocdisorder_next(ocdisorder* list, int index);
void ocdisorder_visit(ocdisorder* list, ocdisorder_visitor);

void* ocdisorder_at(ocdisorder* list, int index);

//辅助函数
void ocdisorder_bind_del(ocdisorder* list, ocdisorder_del);
#endif //_LUG_OC_DISORDER_ARRAY_CPH