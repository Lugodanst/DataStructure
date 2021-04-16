#include "ocdlist.h"
#include <stdlib.h>

struct ocdlist_node{
    ocdlist_node* prev;
    ocdlist_node* next;
    void* data;
};
struct ocdlist{
    ocdlist_node* sentry;
    ocdlist_del del;

};

typedef void(*ocdlist_del)(void*);
typedef int (*ocdlist_visitor)(ocdlist_node*);

static void __default_del(void* data){
    free(data);
}

//创建销毁
ocdlist* ocdlist_create(){
    ocdlist* list = malloc(sizeof(*list));
    OC_CHECK_POINTER(list);
    list->del = __default_del;
    {
        register ocdlist_node* sentry = malloc(sizeof(*sentry));
        OC_CHECK_POINTER(sentry);
        sentry->data = NULL;
        sentry->next = sentry;
        sentry->prev = sentry;
        list->sentry = sentry;
    }
    return list;
}
void ocdlist_destory(ocdlist** list){
    register ocdlist* p = *list; 
    register ocdlist_node* it = p->sentry->next; 
    ocdlist_node* tmp = NULL;
    
    while(it != p->sentry){
        tmp = it->next;
        p->del(it->data);
        it->data = NULL;
        it->prev = it->next = NULL;
        free(it);
        it = tmp;
    }
    p->sentry->prev = p->sentry->next = NULL;

    OC_DELETE(p->sentry);
    free(p);
    *list = NULL;
}

//插入
ocdlist_node* ocdlist_pushback(ocdlist* list, void* data){
    ocdlist_node* node = malloc(sizeof(*node));
    OC_CHECK_POINTER(node);
    node->data = data;
    {
        register ocdlist_node* target = list->sentry->prev;
        node->next = target->next;
        node->prev = target;
        target->next = node;
        node->next->prev = node;
    }
    return node;
}

ocdlist_node* ocdlist_pushfront(ocdlist* list, void* data){

    ocdlist_node* node = malloc(sizeof(*node));
    OC_CHECK_POINTER(node);
    node->data = data;
    {
        register ocdlist_node* target = list->sentry;
        node->next = target->next;
        node->prev = target;
        target->next = node;
        node->next->prev = node;
    }
    return node;

}

ocdlist_node* ocdlist_insert_before(ocdlist_node* node, void* data){

    ocdlist_node* ret = malloc(sizeof(*node));
    OC_CHECK_POINTER(ret);
    ret->data = data;

    ret->next = node;
    ret->prev = node->prev;
    node->prev = ret;
    ret->prev->next = ret;
    return ret;

}
ocdlist_node* ocdlist_insert_after(ocdlist_node* node, void* data){

    ocdlist_node* ret = malloc(sizeof(*node));
    OC_CHECK_POINTER(ret);
    ret->data = data;

    ret->next = node->next;
    ret->prev = node;
    node->next = ret;
    ret->next->prev = ret;
    return ret;
}

//删除
void ocdlist_popback(ocdlist* list){
    
    ocdlist_node* del = list->sentry->prev;
    del->prev->next = del->next;
    del->next->prev = del->prev;
    del->next = del->prev = NULL;
    list->del(del->data);
    del->data = NULL;
    OC_DELETE(del);

}

void ocdlist_popfront(ocdlist* list){

    ocdlist_node* del = list->sentry->next;
    del->prev->next = del->next;
    del->next->prev = del->prev;
    del->next = del->prev = NULL;
    list->del(del->data);
    del->data = NULL;
    OC_DELETE(del);

}

void ocdlist_erase(ocdlist* list, ocdlist_node* node){

    node->prev->next = node->next;
    node->next->prev = node->prev;
    node->prev = node->next = NULL;
    list->del(node->data);
    node->data = NULL;
    OC_DELETE(node);
}

//遍历
ocdlist_node* ocdlist_header(ocdlist* list){
    register ocdlist_node* sentry = list->sentry;
    return sentry->next == sentry ? NULL : sentry->next;
}

ocdlist_node* ocdlist_tail(ocdlist* list){
    register ocdlist_node* sentry = list->sentry;
    return sentry->prev == sentry ? NULL : sentry->prev;
}

ocdlist_node* ocdlist_next(ocdlist* list, ocdlist_node* node){
    return node->next == list->sentry ? NULL : node->next;
}

ocdlist_node* ocdlist_prev(ocdlist* list, ocdlist_node* node){
    return node->prev == list->sentry ? NULL : node->prev;
}

void ocdlist_visit(ocdlist* list, ocdlist_visitor vis){
    ocdlist_node* sentry = list->sentry;
    register ocdlist_node* it = sentry;
    while((it = it->next) != sentry){
        if(!vis(it)) break;
    }
}

//辅助函数
int ocdlist_isempty(ocdlist* list){
    return list->sentry->next == list->sentry;
}

void* ocdlist_topointer(ocdlist_node* node){
    return node->data;
}

void ocdlist_bind_del(ocdlist* list, ocdlist_del del){
    if(del == NULL) return;
    list->del = del;
}

