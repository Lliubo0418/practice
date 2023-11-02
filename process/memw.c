#include<stdio.h>
#include<unistd.h>
#include <sys/mman.h>
#include<fcntl.h>


int main(){

    int fd;
    void *addr;

    fd=open("memtest",O_RDWR);
    if(fd<0){
        perror("open");
        return 0;
    }
    int len=lseek(fd,0,SEEK_END);
    addr=mmap(NULL, 2048, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if(addr==MAP_FAILED){
        perror("mmap");
        return 0;
    }
    close(fd);

    memcpy(addr,"this is memw test\n",18);

}