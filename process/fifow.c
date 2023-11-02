#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

int main(){
    int re;
    int fd;
    char buf[32];

    re=mkfifo("/fifo",0666);
    if(re<0){
        perror("mkfifo");
        return 0;
    }


    fd=open("/fifo",O_WRONLY);
    if(fd<0){
        perror("open");
        return 0;
    }
    while(1){
        fgets(buf,32,stdin);
        write(fd,buf,strlen(buf));
    }


}