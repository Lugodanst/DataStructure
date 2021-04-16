#include "ocslist.h"
#include <stdlib.h>

#define OC_MEM_CHECK(p) if(!p) return NULL


struct ocslist_node{
    ocslist_node* next;
    void* data;
};
struct ocslist{
    ocslist_node* header;
    ocslist_del del;
};

static void __default_del(void* data){
    free(data);
}

//创建和删除
ocslist* ocslist_create(){
    register ocslist_node* header;
    ocslist* list = (ocslist*)malloc(sizeof(*list));
    OC_MEM_CHECK(list);
    header = malloc(sizeof(*header));
    OC_MEM_CHECK(header);
    header->data = NULL;
    header->next = NULL;
    list->header = header; 
    list->del = __default_del;
    return list;
}


void ocslist_destory(ocslist** list){
    ocslist_node* prev;
    ocslist_del del = (*list)->del;
    {
        register ocslist_node* it;
        prev = it = (*list)->header;
        while((it = it->next)){
            free(prev);
            del(it->data);
            it->data = NULL;
            prev = it;
        }   
        OC_DELETE(prev);
    }
    (*list)->header = NULL;
    (*list)->del = NULL;
    OC_DELETE((*list));
}


void ocslist_bind_del(ocslist* list, ocslist_del del){
    if(del == NULL) return;
    list->del = del;
}

//插入
ocslist_node* ocslist_push(ocslist* list, void* data){
    register ocslist_node* node = malloc(sizeof(*node));
    OC_MEM_CHECK(node);
    node->data = data;
    node->next = list->header->next;
    list->header->next = node;
    return node;
}



ocslist_node* ocslist_insert(ocslist_node* node, void* data){
    ocslist_node* ret = malloc(sizeof(*ret));
    OC_MEM_CHECK(ret);
    ret->data = data;
    ret->next = node->next;
    node->next = ret;
    
    return ret;
}

//删除
void ocslist_pop(ocslist* list){
    ocslist_node* header = list->header;
    ocslist_node* del = header->next;
    if(!del) return;
    header->next = del->next;
    del->next = NULL;
    list->del(del->data);
    del->data = NULL;
    OC_DELETE(del);
}

void ocslist_erase(ocslist* list, ocslist_node* node){
    register ocslist_node* it = list->header;
    do{
        if(it->next == node){
            it->next = node->next;
            node->next = NULL;
            list->del(node->data);
            node->data = NULL;
            OC_DELETE(node);
            break;
        }
    }while((it = it->next));
}

//遍历
ocslist_node* ocslist_header(ocslist* list){
    return list->header->next;
}

ocslist_node* ocslist_next(ocslist_node* node){
    return node->next;
}

void ocslist_visit(ocslist* list, ocslist_visitor vis){
    register ocslist_node* it = list->header;
    while((it = it->next)){
        if(!vis(it)) break;
    }
}

int ocslist_isempty(ocslist* list){
    return list->header->next == NULL;
}

void* ocslist_topointer(ocslist_node* node){
    return node->data;
}



#undef OC_MEM_CHECK
