#include<stdio.h>
#include<string.h>
#include<stdlib.h>


int main(){
    int num=0;
    for(int i=1;i<5;i++){
        for(int j=1;j<5;j++){
            num++;
            printf("i=%d,j=%d,num=%d   ",i,j,num);
        }
        printf("\r\n");
        
        
    }
}