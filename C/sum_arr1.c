#include<stdio.h>
#define SIZE 10

int sum(int*,int *);
int main(){


    int arr[SIZE]={20,10,5,39,4,16,19,26,31,20};
    long answer;
    answer=sum(arr,arr+SIZE-1);               //因为下标从0开始，所以arr+SIZE指向数组末尾的下一个位置，这是”越界“指针带来的简洁，指向最后一个元素那就写为现在的arr+SIZE-1

    printf("the total num of arr is %ld\n",answer);

    return 0;
}

int sum(int *start,int *end){
    int total=0;
    while(start<end){
        total+=*start;
        start++;
    }
    return total;
}


