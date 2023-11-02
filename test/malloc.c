#include <stdio.h>
#include<string.h>
#include<stdlib.h>


typedef struct Book
{
    float price;
    int count;
}book;


int main(){
    book *BOOK;
    BOOK = (book *)malloc(sizeof(book));         //没有申请空间的情况下，此时只申请了一个指针的空间，放入数据会产生段错误
    BOOK->count=32;
    BOOK->price=32.8;


    printf("the book's count: %d\n",BOOK->count);
    printf("the book's price: %f\n",BOOK->price);
    printf("地址为：%p \n",BOOK);
    printf("地址大小为：%d\n",sizeof(BOOK));       //8字节 64位操作系统
}
