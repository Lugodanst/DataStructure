#include "../src/ocdisorder.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void nodefree(void* data){}

int visit(void* data){
    printf("%d ", *(int*)data);
}

void visit1(ocdisorder* list){
    printf("visit: ");
    ocdisorder_visit(list, visit);
    printf("\n");
}

void visit2(ocdisorder* list){
    printf("for  : ");
    int index;

    for(index = ocdisorder_first(list); index != OC_DISORDER_NOPOS;
                index = ocdisorder_next(list, index)){
        printf("%d ", *(int*)ocdisorder_at(list, index));
    }
    printf("\n");
}



int test_push_and_erase(){
    int arr[20];
    int dt[4];
    int i;

    ocdisorder* list = ocdisorder_create();
    ocdisorder_bind_del(list, nodefree);

    for(i = 0; i < 20; ++i){
        arr[i] = i + 10;
        ocdisorder_push(list, &arr[i]);
    }

    visit1(list);
    visit2(list);

    //want to delete
    printf("want to delete: ");
    for(i = 0; i < 4; ++i){
        dt[i] = 5 * i + rand() % 5;
        printf("%d ", dt[i] + 10);
        ocdisorder_erase(list, dt[i]);
    }
    printf("\n");

    visit1(list);
    visit2(list);

    //push again
    int value = 66;
    for(i = 0; i < 4; ++i){
        ocdisorder_push(list, &value);
        printf("push %d count:\n", i);
        visit1(list);
        visit2(list);
    }

    ocdisorder_destory(&list);
    return 1;
}

int main(int argc, char *argv[])
{
    srand(time(0));
    test_push_and_erase();

    return 0;
}
