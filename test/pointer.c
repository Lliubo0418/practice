
#include<stdio.h>
#include<time.h>
#include<string.h>
/*
int main(int argc,char *argv[]){

    /*  //指针：地址  指针变量：存放地址的变量
    int a=10;
    int *p;
    p=&a;
    printf("a变量的地址为：%p\n",&a);
    printf("p的内容：%d\n",*p);
    printf("*p的地址为：%p\n",&*p);
    return 0;
    */



   /*
    //指针数组

    int *ptr[3];
    int i;
    int var[]={3,4,5};
    for(i=0;i<3;i++){
        ptr[i]=&var[i];
    }
    for(i=0;i<3;i++){
        printf("var[%d]=%d\n",i,*ptr[i]);
    }
    return 0;



    //数组指针存储字符串列表
    int i;
    char *names[]={"liumao","sumao","liusumao"};
    
    for(i=0;i<3;i++){
        
        printf("names[%d]=%s\n",i,names[i]);
    }
    return 0;
    */


   /*
    //指向指针的指针
    int a=888;
    int *p;
    p=&a;
    int **t;
    t=&p;
    printf("a的地址为：%p\n",&a);
    printf("p存储的地址内容为：%d\n",*p);
    printf("p存储的地址为：%p\n",p);
    printf("**t=%d\n",**t);
    printf("**t中存放的地址为：%p\n",t);
    return 0;




    


}
*/




    
    /*
    //传递指针给函数
    void getseconds(unsigned long *p);

    int main(){
        unsigned long sec;
        getseconds(&sec);
        printf("当前时间为：%ld秒\n",sec);
        return 0;

    }

    void getseconds(unsigned long *p){
        *p=time(NULL);
        return;

    }
*/

/*****************************
    //结构体指针

struct person{
    char name[20];
    int age;
    char sex[10];
    float height;
};
void printPerson(struct person *p);

  
    
    
    
    int main(){
        struct person p1;
        struct person p2;

        strcpy(p1.name,"liubo");
        p1.age=25;
        strcpy(p1.sex,"男");
        p1.height=179;
        
        strcpy(p2.name,"suwandi");
        p2.age=22;
        strcpy(p2.sex,"女");
        p2.height=174;


        printPerson(&p1);
        printPerson(&p2);
        return 0;
    }


void printPerson(struct person *p)
{
    printf("姓名：%s\n",p->name);
    printf("年龄：%d岁\n",p->age);
    printf("性别：%s\n",p->sex);
    printf("身高：%fcm\n",p->height);
};
*********************/



/***
int main(){
    int b[4]={9,8,7,6};
    int a[3][4] = {{1,3,5,7},{9,11,13,15},{17,19,21,23}};
    

    printf("%d  %d\n",b[2],&b[2]);
    printf("%d  %d  %d  %d  %d  %d  %d\n",&*a,*a,a,**a,a+1,*(a+1),**(a+1));
    printf("%d  %d  %d  %d\n",&a[0],a[0]+1,*(a[0]+1),&(*(a[0]+1)));
    printf("%d  %d  %d  %d\n",*(*(a+1)+2),*(a+1)+2,*(a[1]+2),&a[1][2]);
    // 二维数组   a[0]==*(a+0)   &a[0]==&*a
    // 加*成为指向列的指针，加&成为指向行的指针
    // 结果输出
    // 7  6422040
    // 6421984  6421984  6421984  1  6422000  6422000  9
    // 6421984  6421988  3  6421988
    // 13  6422008(指针加2，解引用，取出当前指针内存放的地址，返回内存地址中对应的对象) 13  6422008
}
*///


