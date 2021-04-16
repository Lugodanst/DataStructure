#include "../src/ocdlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void nodefree(void* data){}

int visit(ocdlist_node* node){
    printf("%d ", ocdlist_get(int, node));
    return 1;
}

int test_push_and_pop(){
    int arr[20];
    int i;

    ocdlist* list = ocdlist_create();
    ocdlist_bind_del(list, nodefree);

    for(i = 0; i < 20; ++i){
        arr[i] = i + 10;
        if(i < 10){
            ocdlist_pushback(list, &arr[i]);
        }
        else{
            ocdlist_pushfront(list, &arr[i]);
        }
    }
    ocdlist_visit(list, visit);
    printf("\n");

    for(i = 0; i < 10; ++i){
        if(i & 1){
            ocdlist_popback(list);
        }
        else{
            ocdlist_popfront(list);
        }
    }
    ocdlist_visit(list, visit);
    printf("\n");


    ocdlist_destory(&list);
    return 1;
}

void test_insert(){

    int arr[20];
    int i;

    ocdlist* list = ocdlist_create();
    ocdlist_bind_del(list, nodefree);

    int ipos = rand()%20;
    int dpos = rand()%20;
    ocdlist_node *insert, *delete;
    for(i = 0; i < 20; ++i){
        arr[i] = i + 10;
        if(ipos == i)
            insert = ocdlist_pushback(list, &arr[i]);
        else if(dpos == i)
            delete = ocdlist_pushback(list, &arr[i]);
        else
            ocdlist_pushback(list, &arr[i]);
    }
    ocdlist_visit(list, visit);
    printf("\n");

    int iv = 66;
    printf("insert before, node: %d\n", ocdlist_get(int, insert));
    ocdlist_insert_before(insert, &iv);
    ocdlist_visit(list, visit);
    printf("\n");

    printf("insert after, node: %d\n", ocdlist_get(int, insert));
    ocdlist_insert_after(insert, &iv);
    ocdlist_visit(list, visit);
    printf("\n");

    printf("delete, node: %d\n", ocdlist_get(int, delete));
    ocdlist_erase(list, delete);
    ocdlist_visit(list, visit);
    printf("\n");

    ocdlist_destory(&list);
}


int main(int argc, char *argv[])
{
    srand(time(0));
    test_push_and_pop();
    printf("\n\n\n");
    test_insert();
    return 0;
}
