#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int * func_sum(int n){        //指针函数：根据传入的参数n，来计算从0到n的所有自然数的和，其结果通过指针的形式返回给调用方
    if(n<0){
        printf("error:n must be >0 \n");
        exit(-1);
    }

    static int sum = 0;     //静态局部变量存放在数据段，如果定义为普通的局部变量，在函数结束后，栈区的变量就会被释放掉，因此在定义了指针指向这个变量时，就算返回了该地址的指针，也不一定会得到正确的值
    int *q=&sum;
    for(int i=0;i<=n;i++){
        sum+=i;
    }
    return q;

}

int main(){
    int num = 0;
    printf("please input one number:");
    scanf("%d",&num);
    int *p=func_sum(num);
    printf("sum:%d\n",*p);
    return 0;
}



#if 0
# include <stdio.h>
# include <stdlib.h>

int func_sum2(int n)          //func_sum2函数的功能与指针函数所实现的功能完全一样
{   
    if (n < 0)
    {   
        printf("error:n must be > 0\n");
        exit(-1);
    }
    int sum = 0;
    int i = 0;
    for (i = 0; i < n; i++)
    {   
        sum += i;
    }
    return sum;
}

int main(void)
{
    int num = 0;
    printf("please input one number:");
    scanf("%d", &num);
    int ret = func_sum2(num);
    printf("sum2:%d\n", ret);
    return 0;
}

#endif