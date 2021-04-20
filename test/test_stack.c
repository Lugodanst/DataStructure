#include "../src/ocstack.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void nodefree(void* data){}


void test_types(ocstack* stack){

    int arr[20];
    int i;

    ocstack_bind_del(stack, nodefree);

    printf("try to push: ");
    for(i = 0; i < 20; ++i){
        arr[i] = rand()%100;
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    printf("push:\n");
    for(i = 0; i < 20; ++i){
        if(ocstack_push(stack, &arr[i]) == -1){
            printf("stack overflow\n");
        }
    }

    printf("pop:\n");
    int* data;
    for(i = 0; i < 20; ++i){
        data = (int*)ocstack_top(stack);
        if(data == NULL){
            printf("already empty\n");
            break;
        }
        printf("%d ", *data);
        ocstack_pop(stack);
    }
    printf("\n");
    ocstack_destory(&stack);
}

void test_at(){
    
    int arr[10];
    int i;
    ocstack* stack = ocstack_static(8);

    ocstack_bind_del(stack, nodefree);

    printf("try to push: ");
    for(i = 0; i < 10; ++i){
        arr[i] = rand()%100;
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    printf("push:\n");
    for(i = 0; i < 10; ++i){
        if(ocstack_push(stack, &arr[i]) == -1){
            printf("stack overflow\n");
        }
    }

    printf("+:\n");
    int len = ocstack_size(stack);
    for(i = 0; i < len; ++i){
        printf("%d ", *(int*)ocstack_at(stack, i));
    }
    printf("\n");

    printf("-:\n");
    len = -ocstack_size(stack);
    for(i = -1; i >= len; --i){
        printf("%d ", *(int*)ocstack_at(stack, i));
    }
    printf("\n");

    ocstack_destory(&stack);
}

int main(int argc, char *argv[])
{
    srand(time(0));
    printf("test dynamic:\n");
    test_types(ocstack_dynamic(5));
    printf("\n");

    printf("test static:\n");
    test_types(ocstack_static(8));
    printf("\n");

    printf("test at:\n");
    test_at();

    return 0;
}
