#include "ocarray.h"
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>

typedef void(*ocdelf)(void*);

struct ocarray{
    void** arr;
    int max;
    int cur;
    ocdelf del;
};

static ocarray* oc_relloc(ocarray* arr, int size){
    if(size != 0){
        register int max = arr->max;
        while(max < size){
            max = (int)(max * 1.5f);
        }
        arr->max = max;
    }
    else{
        arr->max = (int)(arr->max * 1.5f);
    }
    void** new_arr = oc_malloc(sizeof(void*) * arr->max);
    {
        register int i;
        int len = arr->cur;
        void** a = arr->arr;
        for(i = 0; i < len; ++i){
            new_arr[i] = a[i];
            a[i] = NULL;
        }
        free(a);
    }
    arr->arr = new_arr;
    return arr;
}

static void __default_del(void* data){
    free(data);
}
static void __nofree(void* data){
    return;
}

//创建和销毁
ocarray* ocarray_create(){
    ocarray* ret = oc_malloc(sizeof(*ret));
    ret->arr = oc_malloc(sizeof(void*) * 2);

    ret->cur = 0;
    ret->max = 2;
    ret->del = __default_del;
    return ret;
}

ocarray* ocarray_create_with_size(int size){

    ocarray* ret = oc_malloc(sizeof(*ret));
    ret->cur = 0;
    ret->del = __default_del;
    
    ret->max = 2;
    while(ret->max < size){
        ret->max = (int)(ret->max * 1.5f);
    }
    ret->arr = oc_malloc(sizeof(void*) * ret->max);
    return ret;
}

void ocarray_destory(ocarray** arr){
    
    if(arr == NULL || *arr == NULL) return;
    ocarray* array = *arr;
    {
        void** a = array->arr;
        ocdelf del = array->del;
        register int i;
        int len;
        for(i = 0, len = array->cur; i < len; ++i){
            del(a[i]);
            a[i] = NULL;
        }
    }
    OC_DELETE(array->arr);
    array->cur = array->max = 0;
    array->del = NULL;
    OC_DELETE(*arr);
}

//增
void ocarray_pushback(ocarray* arr, void* data){
    if(arr->max == arr->cur) oc_relloc(arr, 0);
    arr->arr[arr->cur++] = data;
}

void ocarray_puts(ocarray* arr, void* datas, ...){
    
    int argc = 0;
    va_list ap1, ap;
    va_start(ap1, datas);
    va_copy(ap, ap1);

    //获取要push的数量
    while((++argc) && va_arg(ap1, void*));
    va_end(ap1);

    //尝试是否需要relloc
    if(argc + arr->cur >= arr->max){
        oc_relloc(arr, argc + arr->cur);
    }
    
    //push
    {
        register void* it;
        void** array = arr->arr;
        for(it = datas; it != NULL; it = va_arg(ap, void*)){
            array[arr->cur++] = it;
        }
    }
    va_end(ap);
}

int ocarray_insert(ocarray* arr, int index, void* data){
    if(arr->cur == arr->max) oc_relloc(arr, 0);
    if(index > arr->cur){ 
        return -1;
    }
    else if(index == arr->cur){
        arr->arr[arr->cur++] = data;
    }
    else{
        register int i;
        void** a = arr->arr;
        for(i = arr->cur++; i >= index; --i){
            a[i+1] = a[i];
        }
        a[index] = data;
    }
    return 0;
}

//删
void ocarray_popback(ocarray* arr){
    if(arr->cur == 0) return;
    arr->del(arr->arr[--arr->cur]);
    arr->arr[arr->cur] = NULL;
}
int ocarray_erase(ocarray* arr, int index){
    ocdelf del = arr->del;
    if(index > arr->cur){
        return -1;
    }
    else if(index == arr->cur){
        del(arr->arr[arr->cur]);
        arr->arr[arr->cur] = NULL;
        --arr->cur;
    }
    else{
        register int i;
        int len = --arr->cur;
        void** a = arr->arr;
        del(a[index]);
        for(i = index; i < len; ++i){
            a[i] = a[i+1];
        }
    }
    return 0;
}

//遍历
void* ocarray_at(ocarray* arr, int index){
    if(index >= arr->cur) return NULL;
    return arr->arr[index];
}

int ocarray_size(ocarray* arr){
    return arr->cur;
}

//辅助函数
void ocarray_bind_del(ocarray* arr, ocdelf del){
    if(del == NULL) return;
    arr->del = del;
}

static void* __default_copy(void* data){
    return data;
}

ocarray* ocarray_sub(ocarray* arr, int begin, int end, occopyf copy){

    int size;
    ocarray* ret;

    //参数处理
    if(end < 0) end = arr->cur + end;
    if(end >= arr->cur) end = arr->cur - 1;
    if(begin < 0) begin = arr->cur + begin;
    
    //范围检测
    if(end < begin || arr == NULL){
        errno = EINVAL;
        return NULL;
    }

    //数据操作
    size = end - begin + 1;
    ret = ocarray_create_with_size(size);
    if(ret == NULL) return NULL;

    if(copy == NULL){
        copy = __default_copy;
        ret->del = __nofree;
    }

    ret->cur = size;
    {
        register void** src = &arr->arr[begin];
        register void** dst = ret->arr;
        void** pend = &arr->arr[end];

        for(; src != pend; ++dst, ++src){
            *dst = copy(*src);
        }
        *dst = copy(*src);
    }
    return ret;
}

ocarray* ocarray_cat(ocarray* arr1, ocarray* arr2, occopyf copy){
    int size;
    ocarray* ret;

    //参数检查
    if(arr1 == NULL || arr2 == NULL || arr1->del != arr2->del){
        errno = EINVAL;
        return NULL;
    }

    //创建容量足够的空数组
    size = arr1->cur + arr2->cur;
    ret = ocarray_create_with_size(size);
    if(ret == NULL) return NULL;

    //参数处理
    ret->del = arr1->del;
    if(copy == NULL){
        copy = __default_copy;
        arr1->del = arr2->del = __nofree;
    }

    //新数组操作
    ret->cur = size;
    {
        int i;
        register void **dst, **it;
        dst = ret->arr;
        for(it = arr1->arr, i = arr1->cur; i > 0; ++it, ++dst, --i){
            *dst = copy(*it);
        }
        for(it = arr2->arr, i = arr2->cur; i > 0; ++it, ++dst, --i){
            *dst = copy(*it);
        }
    }

    if(copy == __default_copy){
        ocarray_destory(&arr1);
        ocarray_destory(&arr2);
    }

    return ret;
}



ocarray* ocarray_merge(occopyf copy, ocarray* arrs, ...){
    
    ocarray *newarr = NULL;
    ocarray* args = NULL;
    ocarray* arr = NULL;
    ocdelf del = NULL;

    if(arrs == NULL){
        errno = EINVAL;
        return NULL;
    }
    del = arrs->del;

    va_list arglist, argcpy;
    va_start(arglist, arrs);
    va_copy(argcpy, arglist); //构造新数组

    //构造arg数组、初始化后的新数组
    {
        int count = 0; //要合并的所有数组大小之和
        args = ocarray_create_with_size(6);
        args->del = __nofree; //防止double free
        for (arr = arrs; arr != NULL; arr = va_arg(arglist, ocarray*)){
            if(del != arr->del){//参数所给的数组存储的对象不同
                errno = EINVAL;
                return NULL;
            }
            count += arr->cur;
            if(copy == NULL){
                arr->del = __nofree; //防止doublefree
            }
            ocarray_pushback(args, arr);
        }
        newarr = ocarray_create_with_size(count);
        va_end(arglist);
    }

    //构造新数组
    {
        int i, j, len, argc = args->cur;
        for(i = 0; i < argc; ++i){
            arr = args->arr[i];
            len = arr->cur;
            for(j = 0; j < len; ++j){
                if(copy == NULL){
                    newarr->arr[newarr->cur++] = arr->arr[j];
                }
                else{
                    newarr->arr[newarr->cur++] = copy(arr->arr[j]);
                }
            }
            ocarray_destory(&arr);
        }
    }

    ocarray_destory(&args);
    newarr->del = del;
    return newarr;
}
