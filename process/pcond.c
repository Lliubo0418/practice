#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>
pthread_cond_t cond =PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex =PTHREAD_MUTEX_INITIALIZER;

struct taxi
{
    struct taxi *next;
    int num;
};
struct taxi *Head=NULL;

void *taxiarr(void *arg){
    printf("taxi arrive thread\n");
    pthread_detach(pthread_self());
    int i=1;
    struct taxi *tx;
    
    
    while (1)
    {    
        tx=malloc(sizeof(struct taxi));
        tx->num=i++;
        printf("taxi %d comming\n",tx->num);
        pthread_mutex_lock(&mutex);
        tx->next=Head;
        Head=tx;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex); 
        sleep(1);
    }
    pthread_exit(0);
}


void *bytaxi(void *arg){
    printf("take taxi leave thread\n");
    pthread_detach(pthread_self());

    struct taxi *tx;
    while(1){
        pthread_mutex_lock(&mutex);
        while(Head==NULL)
        {
            pthread_cond_wait(&cond,&mutex);
        }
        tx = Head;
        Head=tx->next;
        printf("%d,Take taxi %d\n",(int)arg,tx->num);
        free(tx);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(0);


}

int main(){

    pthread_t tid1,tid2,tid3,tid4;

    pthread_create(&tid1,NULL,taxiarr,NULL);
    //pthread_create(&tid2,NULL,taxiarr,NULL);
    pthread_create(&tid3,NULL,bytaxi,(void*)1);
    pthread_create(&tid4,NULL,bytaxi,(void*)2);


    while (1)
    {
        sleep(1);
    }
    

}