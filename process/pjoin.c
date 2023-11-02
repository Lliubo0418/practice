#include<stdio.h>
#include<pthread.h>


void *testThread(void *arg){
    printf("this is &d thread\n",(int)arg);
    sleep(20);
    pthread_exit("pthread over");
}


int main(int argc,char *argv[]){
    pthread_t tid[100];
    int i;
    int ret;
    for(i=0;i<100;i++){
    ret=pthread_create(&tid[i],NULL,testThread,(void*)i);

    }
    while(1){
        sleep(1);
    }


}
