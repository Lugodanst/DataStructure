#ifndef _LUG_OC_VECTOR_CPH
#define _LUG_OC_VECTOR_CPH

#include "ocmacro.h"

typedef struct ocarray ocarray;
typedef void(*ocarray_del)(void*);

#define ocarray_get(type, arr, index) *(type*)ocarray_at(arr, index)

//创建和销毁
OCAPI ocarray* ocarray_create();
OCAPI ocarray* ocarray_create_with_size(int size);
OCAPI void ocarray_destory(ocarray** vec);

//增
OCAPI void ocarray_pushback(ocarray* v, void* data);
OCAPI int ocarray_insert(ocarray* v, int index, void* data);

//删
OCAPI void ocarray_popback(ocarray* v);
OCAPI int ocarray_erase(ocarray* v, int index);

//遍历
OCAPI void* ocarray_at(ocarray* v, int index);
OCAPI int ocarray_size(ocarray* v);

//辅助函数
OCAPI void ocarray_bind_del(ocarray* v, ocarray_del del);


#endif //_LUG_OC_VECTOR_CPH