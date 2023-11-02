#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main(){
    double begin,end;
    begin=clock();
    for (int i=0;i<5;i++){
        printf("i的值为：%d\n",i);
    }
    end=clock();
    printf("花费的时间为：%fms \n",end-begin);
    printf("begin:%f\n",begin);
    printf("end:%f\n",end);

}