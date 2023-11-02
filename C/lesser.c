#include<stdio.h>
#include<stdlib.h>
#include<string.h>


int min(int,int);

int main(){
    int a,b;
    printf("please input two num(any character to quit)\n");
   while(scanf("%d %d" ,&a,&b)==2){           //判断输入是否为两个整数，是返回真
    // scanf("%d  %d",&a,&b);
    printf("the lesser of the two num is %d\n",min(a,b));
    printf("please input two num(any character to quit)\n");
    }
    printf("Bye");



    return 0;
}

int min(int m,int n){
    int temp;
    if(m<n){
        temp=m;
    }

    else{
        temp=n;
    }
    return temp;
}