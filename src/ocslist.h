#ifndef __OC_SINGLE_LIST_H
#define __OC_SINGLE_LIST_H
#include "ocmacro.h"

#define ocslist_get(type, node) *(type*)ocslist_topointer(node)

typedef struct ocslist_node ocslist_node;
typedef struct ocslist ocslist;

//释放用户数据的内存
typedef void(*ocslist_del)(void*);
//遍历链表节点，返回0表示终止遍历，返回1表示继续遍历
typedef int (*ocslist_visitor)(ocslist_node*);

//创建和删除
OCAPI ocslist* ocslist_create();
OCAPI void ocslist_destory(ocslist**);

//绑定
OCAPI void ocslist_bind_del(ocslist*, ocslist_del);

//插入
OCAPI ocslist_node* ocslist_push(ocslist*, void*);
OCAPI ocslist_node* ocslist_insert(ocslist_node*, void*);

//删除
OCAPI void ocslist_pop(ocslist*);
OCAPI void ocslist_erase(ocslist*, ocslist_node*);

//遍历
OCAPI ocslist_node* ocslist_header(ocslist*);
OCAPI ocslist_node* ocslist_next(ocslist_node*);
OCAPI void ocslist_visit(ocslist*, ocslist_visitor);

//辅助函数
OCAPI int ocslist_isempty(ocslist*);
OCAPI void* ocslist_topointer(ocslist_node*);
#endif
