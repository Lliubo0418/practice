/*递归*/     //尾递归？

#include<stdio.h>

void up_and_down(int m);
int main(){

    up_and_down(1);

    return 0;
}

void up_and_down(int m){
    printf("Level %d : m location %p\n",m,&m);
    if(m<4){
        
        up_and_down(m+1);
        printf("Level %d : m location %p\n",m,&m);
    }
}