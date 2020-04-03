#include<stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>

long long arr[20];
int temp[20];
void *bumi(void *args){
    int tret = (int)args;
    int i;
    arr[tret]=0;
    for(i=1;i<=temp[tret];i++){
        arr[tret]+=i;
    }
}


int main(){
    pthread_t threads[20];
    key_t key = 1001;
    int *C;
    int i,j;
    int shmid = shmget(key,20*sizeof(int),IPC_CREAT | 0666);
    C = (int *)shmat(shmid,NULL,0);

    for(i=0;i<4;i++){
        for(j=0;j<5;j++){
            printf("%d ",C[i*5 + j]);
        }
        puts("");
    }

    for(i=0;i<20;i++){
        temp[i]=C[i];
        if(pthread_create(&(threads[i]),NULL,&bumi,(void*)i)==1){
            printf("Thread %d Error\n",i);
        }
    }
    for(i=0;i<20;i++){
        pthread_join(threads[i],NULL);
    }
    
    for(i=0;i<4;i++){
        for(j=0;j<5;j++){
            printf("%lld ",arr[i*5 + j]);
        }
        puts("");
    }
    shmdt((void *)C);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}