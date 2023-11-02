#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>

#define POOL_NUM 10                            //线程池大小

typedef struct Task{                           //构造任务结构体
    void *(*func) (void *arg);                 //函数
    void *arg;                                 //函数参数
    struct Task *next;                         //结构体指针
}Task;

typedef struct ThreadPool{                     //构造线程池
    pthread_mutex_t mutex;                  //临界资源
    pthread_cond_t newtask;                  //条件变量

    pthread_t tid[POOL_NUM];                 //10个线程
    Task *queue_head;                         //头节点
    int busywork;                             
}ThreadPool;


ThreadPool *pool;                    //线程池pool

void *workthread(void *arg){                     //工作线程的实现
    while(1){
        pthread_mutex_lock(&pool->mutex);
        pthread_cond_wait(&pool->newtask,&pool->mutex;

        Task *ptask=pool->queue_head;
        pool->queue_head=pool->queue_head->next;
        pthread_mutex_unlock(&pool->mutex);

        ptask->func(ptask->arg);                  
        pool->busywork--;
    }
}

void *realwork(void *arg){
    printf("Finish work %d\n",(int)arg);       

}

void pool_add_task(int arg){                      //线程池添加任务
    Task *newtask;

    pthread_mutex_lock(&pool->mutex);
    while(pool->busywork>=POOL_NUM){
        pthread_mutex_unlock(&pool->mutex);
        sleep(1);
        pthread_mutex_lock(&pool->mutex);

    }
    pthread_mutex_unlock(&pool->mutex);

    newtask=malloc(sizeof(Task));
    newtask->func=realwork;
    newtask->arg=arg;

    pthread_mutex_lock(&pool->mutex);
    Task *task=pool->queue_head;
    if(task==NULL){
        pool->queue_head=newtask;
    }
    else{
        while(task->next!=NULL){
            task=task->next;

        }
        task->next=newtask;
    }
    pool->busywork++;
    pthread_cond_signal(&pool->newtask);


    pthread_mutex_unlock(&pool->mutex);

}

void pool_init(){                           //线程池初始化
    pool=malloc(sizeof(ThreadPool));         //创建线程池
    pthread_mutex_init(&pool->mutex,NULL);   //互斥锁
    pthread_cond_init(&pool->newtask,NULL);   //条件变量
    pool->queue_head=NULL;
    pool->busywork=0;
    int i;

    for(i=0;i<POOL_NUM;i++){
        pthread_create(&pool->tid[i],NULL,workthread,NULL);        //创建线程

    }

}

void pool_destroy(){                         //线程池的销毁
    Task *head;
    while(pool->queue_head!=NULL){           //删除任务队列链表所有节点 
        head=pool->queue_head;                
        pool->queue_head=pool->queue_head->next;
        free(head);
    }
    pthread_mutex_destroy(&pool->mutex);      //删除所有的互斥锁
    pthread_cond_destroy(&pool->newtask);     //删除所有的条件变量
    free(pool);                               //释放空间

}

int main(){
    pool_init();
    sleep(20);
    for(int i=0;i<=20;i++){
        pool_add_task(i);
        printf("task %d add\n",i);
    }
    sleep(5);
    pool_destroy();
}