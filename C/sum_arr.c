#include<stdio.h>

#define SIZE 10

int sum(int arr[],int n);

int main(){

    int arr[SIZE]={20,10,5,39,4,16,19,26,31,20};
    long answer;
    
    answer=sum(arr,SIZE);

    printf("the total num of arr is %ld\n",answer);
    printf("the size of arr is %zd bytes\n",sizeof arr);


    return 0;
}

int sum(int arr[],int n){
    int i;
    int total=0;              //remember initialize the variable

    for(i=0;i<n;i++){
        total+=arr[i];
    }
    printf("the size of arr is %zd bytes\n",sizeof arr);
    return total;
}