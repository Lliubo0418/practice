#include<stdio.h>
#include<string.h>


union DataForm
{
    int i;
    float f;
    char str[20];
};

int main(){
    union DataForm dataform;
    dataform.i=234;
    dataform.f=3.1415926;
    //错误dataform.str="liubo";
    strcpy(dataform.str,"liubo");


    printf("i=%d\n",dataform.i);
    printf("f=%f\n",dataform.f);
    printf("str=%s\n",dataform.str);
    //出现i和f的值损坏，因为最后赋给变量的值占用内存位置
    


    dataform.i=555;
    printf("i=%d\n",dataform.i);

    dataform.f=123.456;
    printf("f=%f\n",dataform.f);

    strcpy(dataform.str,"liubo123");
    printf("str=%s\n",dataform.str);
    //各值完好无损，同一时间只能使用union中的一个成员
    
    return 0;
}