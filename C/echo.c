#include<stdio.h>

// int main(){
//     char ch;
//     while ((ch=getchar())!='#')             //输入不为#则无限循环
//     {
//         putchar(ch);
//     }
    
//     return 0;
// }




int main(){
    char ch;
    while ((ch=getchar())!=EOF)             //输入不为#则无限循环
    {
        putchar(ch);
    }
    
    return 0;
}