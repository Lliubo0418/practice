#include<stdio.h>

// void num_change(int*,int*);            //通过指针实现交换
// int main(){

//     int x=5,y=10;

//     printf("old num x=%d,y=%d\n",x,y);

//     num_change(&x,&y);

//     printf("new num x=%d,y=%d\n",x,y);


//     return 0;
// }

// void num_change(int *x,int *y){
//     int temp;
//     temp=*x;
//     *x=*y;
//     *y=temp;

// }
void num_change(int,int);               //
int main(){

    int x=5,y=10;

    printf("old num x=%d,y=%d\n",x,y);

    num_change(x,y);

    printf("new num x=%d,y=%d\n",x,y);


    return 0;
}

void num_change(int x,int y){
    int temp;
    printf("old num x=%d,y=%d\n",x,y);
    temp=x;
    x=y;
    y=temp;
    printf("old num x=%d,y=%d\n",x,y);
}
