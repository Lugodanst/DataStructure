#ifndef __OC_DOUBLE_LINKLIST_H
#define __OC_DOUBLE_LINKLIST_H

#include "ocmacro.h"

typedef struct ocdlist_node ocdlist_node;
typedef struct ocdlist ocdlist;

typedef void(*ocdlist_del)(void*);
typedef int (*ocdlist_visitor)(ocdlist_node*);

#define ocdlist_get(type, node) *(type*)ocdlist_topointer(node)

//创建销毁
OCAPI  ocdlist* ocdlist_create();
OCAPI  void ocdlist_destory(ocdlist**);

//插入
OCAPI ocdlist_node* ocdlist_pushback(ocdlist*, void*);
OCAPI ocdlist_node* ocdlist_pushfront(ocdlist*, void*);
OCAPI ocdlist_node* ocdlist_insert_before(ocdlist_node* ,void*);
OCAPI ocdlist_node* ocdlist_insert_after(ocdlist_node* ,void*);

//删除
OCAPI void ocdlist_popback(ocdlist*);
OCAPI void ocdlist_popfront(ocdlist*);
OCAPI void ocdlist_erase(ocdlist*, ocdlist_node*);

//遍历
OCAPI ocdlist_node* ocdlist_header(ocdlist*);
OCAPI ocdlist_node* ocdlist_tail(ocdlist*);
OCAPI ocdlist_node* ocdlist_next(ocdlist*, ocdlist_node*);
OCAPI ocdlist_node* ocdlist_prev(ocdlist*, ocdlist_node*);
OCAPI void ocdlist_visit(ocdlist*, ocdlist_visitor);

//辅助函数
OCAPI int ocdlist_isempty(ocdlist*);
OCAPI void* ocdlist_topointer(ocdlist_node*);
OCAPI void ocdlist_bind_del(ocdlist*, ocdlist_del);

#endif
