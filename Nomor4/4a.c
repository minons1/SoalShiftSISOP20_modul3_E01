#include<stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include<stdlib.h>
#include<pthread.h>


int main(){
    key_t key=1001;

    int i,j,k;
    int A[4][2]={1,2,2,1,2,3,1,2};
    int B[2][5]={1,1,2,2,3,1,2,3,1,1};
    int *C;

    //menjadikan array C dengan ukuran 20 menjadi shared variable
    int shmid;
    if((shmid= shmget(key,20*sizeof(int),IPC_CREAT | 0666 ))<0)
        puts("do something");
    C = (int *)shmat(shmid,NULL,0);
    
    //mengalikan matriks A dan B dan disimpan di matriks C
    for(i =0;i<4;i++){
        for(j=0;j<5;j++){
            for(k=0;k<2;k++){
                
                C[i*5 + j]+=A[i][k]*B[k][j];
            }
        }
    }
    
    //mencetak matriks C
    for (i=0;i<4;i++){
        for(j=0;j<5;j++){
            printf("%d ",C[i*5 + j]);
        }
        puts("");
    }
    sleep(3);
    shmdt((void *)C);
    return 0;
}
