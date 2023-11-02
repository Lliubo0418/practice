#include<stdio.h>
#include<stdlib.h>

int main(){
    int age=0;
    int days=0;

    printf("请输入你的年龄：\n");
    scanf("%d",&age);            //根本不熟悉，原来写 scanf("%d\n",&age); 一直卡着

    days=age*365;

    printf("您已经出生了%d天\n",days);
    return 0;
}

