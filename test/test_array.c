#include "../src/ocarray.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void nodefree(void* data){}

void visit(ocarray* arr){
    int len = ocarray_size(arr);
    int i;
    for(i = 0; i < len; ++i){
        printf("%d ", ocarray_get(int, arr, i));
    }
    printf("\n");
}


int test_push_and_pop(){
    int arr[20];
    int i;

    ocarray* array = ocarray_create();
    ocarray_bind_del(array, nodefree);

    for(i = 0; i < 20; ++i){
        arr[i] = i + 10;
        ocarray_pushback(array, &arr[i]);
    }

    visit(array);

    for(i = 0; i < 10; ++i){
        ocarray_popback(array);
    }
    visit(array);

    ocarray_destory(&array);
    return 1;
}

void test_insert(){

    int arr[20];
    int i;

    ocarray* array = ocarray_create();
    ocarray_bind_del(array, nodefree);

    int ipos = rand()%20;
    int dpos = rand()%20;

    for(i = 0; i < 20; ++i){
        arr[i] = i + 10;
        ocarray_pushback(array, &arr[i]);
    }

    visit(array);

    int iv = 66;
    printf("insert at: %d\n", ipos);
    ocarray_insert(array, ipos, &iv);
    visit(array);

    printf("delete at: %d\n", dpos);
    ocarray_erase(array, dpos);
    visit(array);
    ocarray_destory(&array);
}


int main(int argc, char *argv[])
{
    srand(time(0));
    test_push_and_pop();
    printf("\n\n\n");
    test_insert();
    return 0;
}
