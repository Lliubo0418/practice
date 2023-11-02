/**
 * 回调函数就是一个通过指针函数调用的函数。其将函数指针作为一个参数，传递给另一个函数。
 * 回调函数并不是由实现方直接调用，而是在特定的事件或条件发生时由另外一方来调用的。
**/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

size_t i;

void p_array(int *array,size_t arraySize,int (*getValue)(void)){
    for(size_t i=0;i<arraySize;i++);
    array[i]=getValue();
}

int getRandomValue(void){
    int a=0;
    a=rand();
    return 0;
}

int main(){
    int i;
    int myarray[10];
    p_array(myarray,10,getRandomValue);
    for(i=0;i<10;i++){
        printf("getValue=%d\n",myarray[i]);
    }
    return 0;
}


#if 0
#include<stdio.h>
#include<stdlib.h>

//函数功能：实现累加求和
int func_sum(int n)
{
        int sum = 0;
        if (n < 0)
        {
                printf("n must be > 0\n");
                exit(-1);
        }
        for (int i = 0; i < n; i++)
        {
                sum += i;
        }
        return sum;
}

//这个函数是回调函数，其中第二个参数为一个函数指针，通过该函数指针来调用求和函数，并把结果返回给主调函数
int callback(int n, int (*p)(int))
{
        return p(n);
}

int main(void)
{
        int n = 0;
        printf("please input number:");
        scanf("%d", &n);
        printf("the sum from 0 to %d is %d\n", n, callback(n, func_sum));       //此处直接调用回调函数，而不是直接调用func_sum函数
        return 0;
}
//回调函数callback无需关心func_sum是怎么实现的，只需要去调用即可。
//这样的好处就是，如果以后对求和函数有优化，比如新写了个func_sum2函数的实现，我们只需要在调用回调函数的地方将函数指针指向func_sum2即可，而无需去修改callback函数内部。

#endif