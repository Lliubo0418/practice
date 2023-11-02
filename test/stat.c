#include<stdio.h>
#include<time.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<dirent.h>

int main(int argc,char *argv[]){
   
    DIR *dp;
    struct dirent *dt;
    
    dp=opendir("/home/liubo/practice/IO/sread/");   //打开文件目录
    if(!dp){                 //打开失败，返回错误信息
        perror("opendir");    
        return 0;
    }
    while((dt=readdir(dp))!=NULL){
        printf("%s\n",dt->d_name);     //读取不为空，打印文件名
    
    
   
    struct stat buf;                 //定义结构体buf
    int ret;                        

    ret=stat("/home/liubo/practice/IO/sread/",&buf);         //成功返回0，失败返回-1
    if(ret<0){
        perror("stat");
        return 0;
    }
    if(S_ISREG(buf.st_mode)){
        printf("-");
    }
    if(S_ISDIR(buf.st_mode)){
       printf("d");
   }
   if(S_ISCHR(buf.st_mode)){
       printf("c");
   }
   if(S_ISBLK(buf.st_mode)){
       printf("b");
   }
   if(S_ISFIFO(buf.st_mode)){
       printf("p");
   }
 int i;                                
   for(i=8;i>=0;i--){
       if(buf.st_mode & (1<<i)){          //8-0位保存文件访问权限
          switch(i%3){
          case 2:
              printf("r");
              break;
          case 1:
              printf("w");
              break;
          case 0:
              printf("x");
              break;
          }
       }else{
           printf("-");
       }
   }
   printf("  %d",(int)buf.st_size);
   struct tm *t;
   t=localtime(&buf.st_ctime);
   printf("  %d-%02d-%02d  %02d:%02d:%02d\n",t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);
    }  
}