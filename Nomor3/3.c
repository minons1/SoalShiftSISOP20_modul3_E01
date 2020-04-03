#include<stdio.h>
#include<dirent.h>
#include<string.h>
#include<pthread.h>
#include<ctype.h>
#include <sys/types.h>
#include<sys/stat.h>

void *hutan(void *filename){
    char* file=(char*)filename;
    char* split= strrchr(filename,'.');
    char dirname[200];
    if(split){
        strcpy(dirname,split+1);
        int i;
        for(i=0;i<strlen(dirname);i++){
            dirname[i]=tolower(dirname[i]);
        }
    }
    else{
        strcpy(dirname,"Unknown");
    }
    struct stat sb;
    if(!(stat(dirname, &sb)==0 &&S_ISDIR(sb.st_mode))){
        mkdir(dirname,0755);
    }
    char* only_file= strrchr(filename,'/');
    char new[200]="/home/salim/praktikum/Modul3/";
    strcat(new,dirname);
    strcat(new,only_file);
    if(rename(filename,new)==0);
}
int main(int argc,char *argv[]){
    int i;
    DIR *dir;
    char dirname[64];
    if(strcmp(argv[1],"-f")==0){
        for(i=2;i<argc;i++){
            pthread_t thread;
            if(pthread_create(&thread,NULL,hutan,(void*)argv[i])==1){
                printf("Thread Error\n");
            }
            pthread_join(thread,NULL);
        }
    }
    else{
        if(argv[1][0]=='*'){
            dir = opendir("/home/salim/praktikum/Modul3");
            strcpy(dirname,"/home/salim/praktikum/Modul3");
        }
        else if(strcmp(argv[1],"-d")==0){
            dir=opendir(argv[2]);
            strcpy(dirname,argv[2]);
        }
        struct dirent *drnt;
        if(dir){
            char check[200];
            while(drnt = readdir(dir)){
                strcpy(check,dirname);
                strcat(check,"/");
                strcat(check,drnt->d_name);

                struct stat sb;
                if(stat(check, &sb)==0 &&S_ISREG(sb.st_mode)){
                    pthread_t thread;
                    if(pthread_create(&thread,NULL,hutan,(void*)check)==1){
                        printf("Thread Error\n");
                    }
                    pthread_join(thread,NULL);
                }

            }
            closedir(dir);
        }
        else{
            puts("cant open directory");
        }
    }
}