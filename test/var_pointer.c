#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(){
    int n=12;
    int *p=NULL;
    p=&n;

    printf("n的内容为%d\n",n);
    printf("n的地址为%p\n",&n);
    printf("n所占字节数为：%d\n",sizeof(n));                   //int占4个字节，32位操作系统也同样占4个字节
    printf("指针p的内容为：%p\n",p);
    printf("指针p指向的地址的内容为：%d\n",*p);
    printf("指针p的地址为：%p\n",&p);
    printf("指针所占字节数为：%d\n",sizeof(p));               //64位操作系统，指针占8个字节
}