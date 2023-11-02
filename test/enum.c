#include<stdio.h>
#include<string.h>



/*
//enum 枚举名{枚举元素1，枚举元素2，……}
//先枚举再定义
enum Day{
    MON,TUE,WED,THU,FRI,SAT,SUN
};
enum Day day;

//定义同时枚举
enum Day{
    MON,TUE,WED,THU,FRI,SAT,SUN
}day;

//省略枚举名称，直接定义枚举变量
enum {
    MON,TUE,WED,THU,FRI,SAT,SUN
}day;

*/
enum Day{
    MON,TUE,WED,THU,FRI,SAT,SUN
};
int main(){
enum Day day;
day=WED;
printf("%d\n",day);

for (day=MON;day<=SUN;day++){
    printf("enum element:%d\n",day);
}
return 0;



}