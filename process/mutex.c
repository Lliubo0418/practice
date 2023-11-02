#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<string.h>

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

FILE *fp;

void *test1(void *arg){
    pthread_detach(pthread_self());
    printf("this is test1 thread\n");
    char str[]="test1 thread write\n";
    char c;
    int i;
    while(1){
    pthread_mutex_lock(&mutex);
    for(i=0;i<strlen(str);i++){
        c=str[i];
        fputc(c,fp);
        usleep(1);
    }
    pthread_mutex_unlock(&mutex);
    i=0;
    usleep(1);
    }
    pthread_exit("test1 quit");
}

void *test2(void *arg){
    pthread_detach(pthread_self());
    printf("this is test2 thread\n");
    char str[]="test2 thread wwwww\n";
    char c;
    int i;
    while(1){
    pthread_mutex_lock(&mutex);
    for(i=0;i<strlen(str);i++){
        c=str[i];
        fputc(c,fp);
        usleep(1);
    }
    pthread_mutex_unlock(&mutex);
    i=0;
    usleep(1);
    }
    pthread_exit("test2 quit");
}

int main(){

    fp=fopen("1.txt","a+");
    if(fp==NULL){
        perror("fopen");
        return 0;
    }
    pthread_t tid1,tid2;

    pthread_create(&tid1,NULL,test1,NULL);


    pthread_create(&tid2,NULL,test2,NULL);

    while (1)
    {
        sleep(1);
    }
    
}