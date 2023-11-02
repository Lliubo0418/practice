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

    fd=open("/fifo",O_RDONLY);
    if(fd<0){
        perror("open");
        return 0;
    }
    while(1){
        
        re=read(fd,buf,32);
        if(re>0){
            printf("read fifo=%s\n",buf);
        }
        else if(re==0){
            exit(0);
        }
    }


}