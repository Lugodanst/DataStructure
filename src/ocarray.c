#include "ocarray.h"
#include <stdlib.h>

typedef void(*ocarray_del)(void*);

struct ocarray{
    void** arr;
    ocarray_del del;
    int max;
    int cur;

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
    void** new_arr = malloc(sizeof(void*) * arr->max);
    OC_CHECK_POINTER(new_arr);
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

//创建和销毁
ocarray* ocarray_create(){
    ocarray* ret = malloc(sizeof(*ret));
    OC_CHECK_POINTER(ret);
    ret->cur = 0;
    ret->max = 2;
    ret->del = __default_del;
    ret->arr = malloc(sizeof(void*) * 2);
    if(!ret->arr){
        OC_DELETE(ret);
        return NULL;
    }
    return ret;
}

ocarray* ocarray_create_with_size(int size){

    ocarray* ret = malloc(sizeof(*ret));
    OC_CHECK_POINTER(ret);
    ret->cur = 0;
    ret->del = __default_del;
    
    ret->max = 2;
    while(ret->max < size){
        ret->max = (int)(ret->max * 1.5f);
    }
    ret->arr = malloc(sizeof(void*) * ret->max);
    if(!ret->arr){
        OC_DELETE(ret);
        return NULL;
    }
    return ret;
}

void ocarray_destory(ocarray** arr){
    
    ocarray* array = *arr;
    {
        void** a = array->arr;
        ocarray_del del = array->del;
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
    arr->del(arr->arr[arr->cur]);
    arr->arr[arr->cur] = NULL;
    --arr->cur;
}
int ocarray_erase(ocarray* arr, int index){
    ocarray_del del = arr->del;
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
void ocarray_bind_del(ocarray* arr, ocarray_del del){
    if(del == NULL) return;
    arr->del = del;
}
