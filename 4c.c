#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<wait.h>
#include<fcntl.h>

int main(){
    int fd[2];
    pipe(fd);
    if(fork()==0){
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);

        execlp("ls","ls",(char*)NULL);
    }
    if(fork()==0){
        dup2(fd[0],STDIN_FILENO);
        close(fd[1]);
        close(fd[0]);
        
        execlp("wc","wc","-l",(char*)NULL);
    }
    close(fd[0]);
    close(fd[1]);

    while(wait(NULL)>0);
}