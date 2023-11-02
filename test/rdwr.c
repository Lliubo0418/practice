#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>


int main(int argc,char *argv[]){

    int fd;
    int ret;
    char buf[32];
    
    time_t ctime;
    struct tm *ctimestr;
    int linecount=0;


    fd = open("test.txt",O_RDWR | O_CREAT|O_APPEND, 0664);
    if(fd<0){
       printf("open file err\n");
       return 0;
}
    printf("sucess,fd=%d\n",fd);

    
    while(1){
        ctime=time(NULL);
        ctimestr=localtime(&ctime);
    printf("%04d-%02d-%02d %02d:%02d:%02d\n",ctimestr->tm_year+1900,ctimestr->tm_mon+1,ctimestr->tm_mday,ctimestr->tm_hour,ctimestr->tm_min,ctimestr->tm_sec);
    sprintf(buf,"%d.%04d-%02d-%02d %02d:%02d:%02d\n",linecount+1,ctimestr->tm_year+1900,ctimestr->tm_mon+1,ctimestr->tm_mday,ctimestr->tm_hour,ctimestr->tm_min,ctimestr->tm_sec);
    ret=write(fd,buf,strlen(buf));
    if(ret<0){
       perror("write");
       goto END;
    }
    printf("write count=%d\n",ret);
    lseek(fd,0,SEEK_SET);
    fflush(stdout);
         linecount++;
   sleep(1);
        }
   


END:


   close(fd);
}