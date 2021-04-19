#include "ocdisorder.h"
#include <stdlib.h>

#define OC_BORDER 0xffff

struct ocdisorder{
    void** arr;
    int cur;
    int max;
    int pos;//指向删除的节点, 小于OC_BORDER
    ocdisorder_del del;
};

static ocdisorder* oc_relloc(ocdisorder* list, int size){
    if(size != 0){
        register int max = list->max;
        while(max < size){
            max = (int)(max * 1.5f);
        }
        list->max = max;
    }
    else{
        list->max = (int)(list->max * 1.5f);
    }
    void** new_arr = malloc(sizeof(void*) * list->max);
    OC_CHECK_POINTER(new_arr);
    {
        register int i;
        int len = list->cur;
        void** a = list->arr;
        for(i = 0; i < len; ++i){
            new_arr[i] = a[i];
            a[i] = NULL;
        }
        free(a);
    }
    list->arr = new_arr;\
    return list;
}

static void __default_del(void* data){
    free(data);
}

//创建，删除
ocdisorder* ocdisorder_create(){
    ocdisorder* ret = malloc(sizeof(*ret));
    OC_CHECK_POINTER(ret);
    ret->cur = 0;
    ret->max = 2;
    ret->pos = OC_DISORDER_NOPOS;
    ret->del = __default_del;
    {
        register void** arr = malloc(sizeof(void*) * 2);
        if(arr == NULL){
            OC_DELETE(ret);
            return NULL;
        }
        ret->arr = arr;
    }

    return ret;
}

void ocdisorder_destory(ocdisorder** list){
    ocdisorder* p = *list;
    {
        register int i;
        int len = p->cur;
        void** arr = p->arr;

        for(i = 0; i < len; ++i){
            if((long long)arr[i] > OC_BORDER){
                p->del(arr[i]);
            }
            arr[i] = NULL;
        }
    }
    OC_DELETE(p->arr);
    p->cur = p->max = p->pos = 0;
    p->del = NULL;
    OC_DELETE(*list);
}

//增
void ocdisorder_push(ocdisorder* list, void* data){
    if(list->cur == list->max) oc_relloc(list, 0);
    void** arr = list->arr;
    if(list->pos == OC_DISORDER_NOPOS){
        arr[list->cur++] = data;
        return;
    }
    int pos = list->pos;
    list->pos = (int)arr[list->pos];//可能出现精度损失, 必须严格控制
    arr[pos] = data;
}

//删
int ocdisorder_erase(ocdisorder* list, int index){
    void** arr = list->arr;
    if((long long)arr[index] > OC_BORDER){
        list->del(arr[index]);
        arr[index] = (void*)(long long)(list->pos);
        list->pos = index;
        return 0;
    }
    else{
        return -1;
    }
}

//遍历
int ocdisorder_first(ocdisorder* list){
    void** arr = list->arr;
    int len = list->cur;
    {
        register int i;
        for(i = 0; i < len; ++i){
            if((long long)arr[i] > OC_BORDER){
                return i;
            }
        }
    }
    return OC_DISORDER_NOPOS;
}

int ocdisorder_last(ocdisorder* list){
    void** arr = list->arr;
    register int i = list->cur - 1;
    for(i = list->cur - 1; i > 0; --i){
        if((long long)arr[i] > OC_BORDER){
                return i;
        }
    }
    return OC_DISORDER_NOPOS;
}

int ocdisorder_prev(ocdisorder* list, int index){
    void** arr = list->arr;
    register int i;
    for(i = index - 1; i > 0; --i){
        if((long long)arr[i] > OC_BORDER){
                return i;
        }
    }
    return OC_DISORDER_NOPOS;
}


int ocdisorder_next(ocdisorder* list, int index){

    void** arr = list->arr;
    int len = list->cur;
    {
        register int i;
        for(i = index + 1; i < len; ++i){
            if((long long)arr[i] > OC_BORDER){
                    return i;
            }
        }
    }
    return OC_DISORDER_NOPOS;
}


void ocdisorder_visit(ocdisorder* list, ocdisorder_visitor vis){
    void** arr = list->arr;
    int len = list->cur;
    {
        register int i;
        for(i = 0; i < len; ++i){
            if((long long)arr[i] > OC_BORDER){
                    if (!vis(arr[i])) break; 
            }
        }
    }
}

void* ocdisorder_at(ocdisorder* list, int index){
    if((long long)(list->arr[index]) > OC_BORDER){
        return list->arr[index];
    }
    else{
        return NULL;
    }
}

//辅助函数
void ocdisorder_bind_del(ocdisorder* list, ocdisorder_del del){
    if(del == NULL) return;
    list->del = del;
}


#undef OC_BORDER
