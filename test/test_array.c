#include "../src/ocarray.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define ocarray_get(type, arr, index) *(type*)ocarray_at(arr, index)

void nodefree(void* data){}

void* copy(void* data){
    int* v = malloc(sizeof(*v));
    *v = *(int*)data;
    return v;
}

int* newD(int data){
    int* v = malloc(sizeof(*v));
    *v = data;
    return v;
}

void visit(ocarray* arr){
    int len = ocarray_size(arr);
    int i;
    for(i = 0; i < len; ++i){
        printf("%d ", ocarray_get(int, arr, i));
    }
    printf("\n");
}


int test_sub(){

    int arr[20];
    int i;

    ocarray* subarr;
    ocarray* array = ocarray_create();
    ocarray_bind_del(array, nodefree);

    for(i = 0; i < 20; ++i){
        arr[i] = i + 10;
        ocarray_pushback(array, &arr[i]);
    }


//----------------------test-----------------
    subarr = ocarray_sub(array, 0, -1, NULL);
    visit(subarr);
    ocarray_destory(&subarr);

    subarr = ocarray_sub(array, 0, 5, NULL);
    visit(subarr);
    ocarray_destory(&subarr);

    subarr = ocarray_sub(array, -8, -2, NULL);
    visit(subarr);
    ocarray_destory(&subarr);

    subarr = ocarray_sub(array, 7, 12, NULL);
    visit(subarr);
    ocarray_destory(&subarr);

    subarr = ocarray_sub(array, 7, 12, copy);
    visit(subarr);
    ocarray_destory(&subarr);

//-------------------------------------------

    ocarray_destory(&array);
    return 1;
}

void test_cat(){
    int arr[20];
    int i;

    ocarray* arr1 = ocarray_create();
    ocarray* arr2 = ocarray_create();
    ocarray* arr3, *arr4, *arr5;
    ocarray_bind_del(arr1, nodefree);
    ocarray_bind_del(arr2, nodefree);

    for(i = 0; i < 20; ++i){
        arr[i] = i + 10;
        ocarray_pushback(arr1, &arr[i]);
        ocarray_pushback(arr2, &arr[i]);
    }

    visit(arr1);
    visit(arr2);

    //此处会有40 * sizeof(int)字节的内存泄露
    arr3 = ocarray_cat(arr1, arr2, copy);
    arr4 = ocarray_cat(arr1, arr2, NULL);
    visit(arr3);
    visit(arr4);

    ocarray_destory(&arr3);
    ocarray_destory(&arr4);
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

void test_puts_merge(){
    ocarray* newa;
    ocarray* arr1 = ocarray_create();
    ocarray_puts(arr1, newD(1), newD(2), newD(3), NULL);

    ocarray* arr2 = ocarray_create();
    ocarray_puts(arr2, newD(4), newD(6), newD(6), NULL);

    ocarray* arr3 = ocarray_create();
    ocarray_puts(arr3, newD(11), newD(12), newD(13), NULL);

    newa = ocarray_merge(NULL, arr1, arr2, arr3, NULL);

    visit(newa);

    ocarray_destory(&newa);
}


int main(int argc, char *argv[])
{
    srand(time(0));
    test_push_and_pop();
    printf("\n\n\n");
    test_insert();

    printf("\n\n\ntest sub\n");
    test_sub();
    printf("\n\n\ntest cat\n");
    test_cat();

    printf("\n\n\ntest merge\n");
    test_puts_merge();

    return 0;
}
