#ifndef __OC_MACRO_H
#define __OC_MACRO_H

#define OCAPI extern

#define OC_DELETE(p) do{free(p); p = NULL;}while(0)

#define OC_CHECK_POINTER(p) do{ if(!p) return NULL; }while(0)

OCAPI void* oc_malloc(int size);


typedef void (*ocdelf)(void*);
typedef void* (*occopyf)(void*);
typedef int (*occomparef)(void*);

#endif
