#include "ocstack.h"
#include <stdlib.h>

#pragma pack(push, 4)
struct ocstack{
    void** arr;
    int cur;
    int max;
    char dynamic;
    ocstack_del del;
};
#pragma pack(pop)


static ocstack* oc_relloc(ocstack* stack, int size){
    if(size != 0){
        register int max = stack->max;
        while(max < size){
            max = (int)(max * 1.5f);
        }
        stack->max = max;
    }
    else{
        stack->max = (int)(stack->max * 1.5f);
    }
    void** new_stack = malloc(sizeof(void*) * stack->max);
    OC_CHECK_POINTER(new_stack);
    {
        register int i;
        int len = stack->cur;
        void** a = stack->arr;
        for(i = 0; i < len; ++i){
            new_stack[i] = a[i];
            a[i] = NULL;
        }
        free(a);
    }
    stack->arr = new_stack;
    return stack;
}



static void __defaule_del(void* data){
    free(data);
}
//创建，销毁
ocstack* ocstack_static(int size){
    ocstack* stack = malloc(sizeof(*stack));
    OC_CHECK_POINTER(stack);
    stack->arr=  malloc(sizeof(void*) * size);
    if(stack->arr == NULL){
        OC_DELETE(stack);
        return NULL;
    }

    stack->cur = 0;
    stack->max = size;
    stack->dynamic = 0;
    
    stack->del = __defaule_del;
    return stack;
}
ocstack* ocstack_dynamic(int size){

    ocstack* stack = malloc(sizeof(*stack));
    OC_CHECK_POINTER(stack);
    {
        register int max = 2;
        while(max < size){
            max = (int)(max * 1.5f);
        }
        size = max;
    }

    stack->arr=  malloc(sizeof(void*) * size);
    if(stack->arr == NULL){
        OC_DELETE(stack);
        return NULL;
    }
    stack->cur = 0;
    stack->max = size;
    stack->dynamic = 1;
    
    stack->del = __defaule_del;
    return stack;
}

void ocstack_destory(ocstack** stack){
    ocstack* s = *stack;
    void** arr = s->arr;
    ocstack_del del = s->del;
    {
        register int i;
        for(i = 0; i < s->cur; ++i){
            del(arr[i]);
            arr[i] = NULL;
        }
    }
    OC_DELETE(s->arr);
    s->cur = s->max = 0;
    s->dynamic = 0;
    s->del = NULL;
    OC_DELETE(*stack);
}

//增加
int ocstack_push(ocstack* stack, void* data){
    if(stack->cur == stack->max){
        if(stack->dynamic == 1){
            oc_relloc(stack, 0);
        }
        else{
            return -1;
        }
    }
    stack->arr[stack->cur++] = data;
    return 0;
}

//删除
int ocstack_pop(ocstack* stack){
    if(stack->cur == 0) return -1;
    stack->del(stack->arr[stack->cur]);
    stack->arr[stack->cur--] = NULL;
    return 0;
}

//获取数据
void* ocstack_top(ocstack* stack){
    if(stack->cur == 0) return NULL;
    return stack->arr[stack->cur - 1];
}
void* ocstack_at(ocstack* stack, int index){
    if(index < 0){
        if(-index > stack->cur){
            return NULL;
        }
        else{
            return stack->arr[stack->cur + index];
        }
    }
    else{
        if(index < stack->cur){
            return stack->arr[index];
        }
        else{
            return NULL;
        }
    }
}

//辅助函数
void ocstack_bind_del(ocstack* stack, ocstack_del del){
    if(del == NULL) return;
    stack->del = del;
}

int ocstack_size(ocstack* stack){
    return stack->cur;
}
