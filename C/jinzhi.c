/*进制问题练习*/

#include<stdio.h>

int main(void){
    int num=100;

    printf("this num in dec=%d; octal=%o; hex=%x\n",num,num,num);       //将十进制以十进制，八进制，十六进制显示 使用 %d   %o   %x
    printf("this num in dec=%d; octal=%#o; hex=%#x\n",num,num,num);     //将十进制以十进制，八进制，十六进制显示并显示前缀 使用 %d   %#o   %#x

    return 0;
}