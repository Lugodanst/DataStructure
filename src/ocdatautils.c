#include "ocdatautils.h"
#include <stdlib.h>
void* oc_malloc(int size){
    void* p = malloc(size);
    if(p == NULL){
        abort();
    }
    return p;
}
