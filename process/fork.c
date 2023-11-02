//创建子进程
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
int main(int argc,char *argv[]){
    pid_t pid;
    pid=fork();
    printf("pid=%d\n",(int)pid);

}
//void exit(int status);

