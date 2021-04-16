#include "../src/ocslist.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int* newInt(int v){
    int* r = malloc(sizeof(*r));
    *r = v;
    return r;
}

void nodefree(void* data){

}

int visit(ocslist_node* node){
    printf("%d ", ocslist_get(int, node));
    return 1;
}

int test_push_and_pop(){
    int arr[60];
    int i;

    ocslist* list = ocslist_create();
    ocslist_bind_del(list, nodefree);

    for(i = 0; i < 60; ++i){
        arr[i] = i + 10;
        ocslist_push(list, &arr[i]);
    }
    ocslist_visit(list, visit);
    printf("\n");

    for(i = 0; i < 30; ++i){
        ocslist_pop(list);
        ocslist_visit(list, visit);
        printf("\n");
    }
    ocslist_visit(list, visit);
    printf("\n");

    ocslist_destory(&list);
    return 1;
}


int main(int argc, char *argv[])
{
    ocslist* list = ocslist_create();
    ocslist_node* n;

    ocslist_push(list, newInt(10));    
    ocslist_push(list, newInt(20));    
    ocslist_push(list, newInt(30));    
    ocslist_push(list, newInt(40));    
    n = ocslist_push(list, newInt(50));    
    ocslist_push(list, newInt(60));    

    ocslist_erase(list, n);

    ocslist_visit(list, visit);
    printf("\n");

    ocslist_destory(&list); 
    test_push_and_pop();
    return 0;
}
