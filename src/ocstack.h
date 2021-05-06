#ifndef _LUG_OC_STACK_CPH
#define _LUG_OC_STACK_CPH

#include "ocdatautils.h"

typedef struct ocstack ocstack;
typedef void(*ocstack_del)(void*);

//创建，销毁
ocstack* ocstack_static(int size);
ocstack* ocstack_dynamic(int size);
void ocstack_destory(ocstack** stack);

//增加
int ocstack_push(ocstack* stack, void* data);

//删除
int ocstack_pop(ocstack* stack);

//获取数据
void* ocstack_top(ocstack* stack);
/**
 * index为正，表示从栈底开始，0表示栈底
 * index为负，表示从栈顶开始，-1表示栈顶
 * */
void* ocstack_at(ocstack* stack, int index);

//辅助函数
void ocstack_bind_del(ocstack* stack, ocstack_del);
int ocstack_size(ocstack* stack);

#endif //_LUG_OC_STACK_CPH