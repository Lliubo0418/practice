#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

void * testThread(void *arg){
    printf("this is child thread\n");
    //pthread_detach(pthread_self());

    sleep(25);    
    pthread_exit("thread quit");    

}


int main(){
    pthread_t tid[100];
    pthread_attr_t attr;
    int i;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    for(i=0;i<100;i++){
    pthread_create(&tid,NULL,testThread,NULL);
    }
    while (1)
    {
        sleep(1);
    }
    
}
