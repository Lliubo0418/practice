#include<stdio.h>
#include<string.h>

/*
struct Books{
    char title[50];
    char author[50];
    float price;
    int book_id;
}book;

Books:结构体标签
int book_id:结构体中变量定义
book:结构变量

一般情况下，这三部分至少要出现两个

*/




/*
//此结构体的声明包含了其他的结构体
struct COMPLEX
{
    char string[100];
    struct Books book;
};


//此结构体的声明包含了指向自己的指针
struct NODE
{
    char string[100];
    struct NODE *node;
};
*/

//如果两个结构体互相包含，则需要对其中一个结构体进行不完整声明
/*
struct B;

struct A
{
    struct B *partner;
    //other members
};

//结构体B中包含指向结构体A的指针，在A声明完之后，B可以随之声明
struct B
{
    struct A *partner;
    //other members
};

*/


/*
//结构体变量的初始化

struct Books{
    char title[50];
    char author[50];
    float price;
    int book_id;
}book={"c programme","liubo",88.8,666};

int main(){
    printf(" title:%s\n author:%s\n price:%0.2f\n book_id:%d\n",book.title,book.author,book.price,book.book_id);
    return 0;
}
*/


/*
访问结构体成员
struct Books{
    char title[50];
    char author[50];
    float price;
    int book_id;
};

int main(){
    struct Books b1;
    struct Books b2;

    strcpy(b1.title,"C与指针");
    strcpy(b1.author,"liubo");
    b1.price=66.6;
    b1.book_id=888;


    strcpy(b2.title,"编程之道");
    strcpy(b2.author,"liubo");
    b2.price=88.8;
    b2.book_id=666;
    
    printf("b1.title:%s\n",b1.title);
    printf("b1.author:%s\n",b1.author);
    printf("b1.price:%0.2f\n",b1.price);
    printf("b1.book_id:%d\n",b1.book_id);

    printf("b2.title:%s\n",b2.title);
    printf("b2.author:%s\n",b2.author);
    printf("b2.price:%0.2f\n",b2.price);
    printf("b2.book_id:%d\n",b2.book_id);

    return 0;
}
*/

/*
//结构作为函数参数
struct Books{
    char title[50];
    char author[50];
    float price;
    int book_id;
};

//声明函数
void printBook(struct Books book);

int main(){
    struct Books b1;
    struct Books b2;

    strcpy(b1.title,"C与指针");
    strcpy(b1.author,"liubo");
    b1.price=66.6;
    b1.book_id=888;


    strcpy(b2.title,"编程之道");
    strcpy(b2.author,"liubo");
    b2.price=88.8;
    b2.book_id=666;


    printBook(b1);
    printBook(b2);

    return 0;
}

    void printBook(struct Books book)
    {
    printf("book.title:%s\n",book.title);
    printf("book.author:%s\n",book.author);
    printf("book.price:%0.2f\n",book.price);
    printf("book.book_id:%d\n",book.book_id);  
    }
*/


//指向结构的指针

typedef struct Books{
    char title[50];
    char author[50];
    float price;
    int book_id;
    
    union Filter
{
    int i;
    int range;
    int filter_none;
}F;

    
}Mybook;


//声明函数


void printBook(struct Books *book);

int main(){
    struct Books b1;
    struct Books b2;

    

    strcpy(b1.title,"C与指针");
    strcpy(b1.author,"liubo");
    b1.price=66.6;
    b1.book_id=888;
    

    strcpy(b2.title,"编程之道");
    strcpy(b2.author,"liubo");
    b2.price=88.8;
    b2.book_id=666;


    printBook(&b1);
    printBook(&b2);

    return 0;
}
//定义指向结构的指针，方式与定义指向其他类型变量的指针相似
//struct Books *struct_pointer;

//在上述定义的指针变量中存储结构变量的地址。为了查找结构变量的地址，请把 & 运算符放在结构名称的前面
//struct_pointer=&b1;

//使用指向该结构的指针访问结构的成员，您必须使用 -> 运算符
//struct_pointer->title;

    void printBook(struct Books *book)
    {
    printf("book.title:%s\n",book->title);
    printf("book.author:%s\n",book->author);
    printf("book.price:%0.2f\n",book->price);
    printf("book.book_id:%d\n",book->book_id);  
    }