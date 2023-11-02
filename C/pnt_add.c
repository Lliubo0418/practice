#include<stdio.h>

#define SIZE 4

int main(){
    short data[SIZE];
    short *pti;
    short index;
    double bills[SIZE];
    double *ptf;

    pti=data;    //把数组地址赋值给指针
    ptf=bills;

    printf("%23s  %15s\n","short","double");  //n空格后打印显示

    for(index=0;index<SIZE;index++){
        printf("pointers + %d :%10p %10p\n",index,pti+index,ptf+index);
    }


    return 0;
}