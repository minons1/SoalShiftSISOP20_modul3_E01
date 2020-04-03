#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>
#include <ctype.h>
#include <wait.h>
#include <termios.h>
#include <fcntl.h>
#define PORT 8080

int my_health;
int enemy_health;
int before;

int getch(void){
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}
int kbhit(void){
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF)
    {
    ungetc(ch, stdin);
    return 1;
    }

    return 0;
}

void *langit(void *args){
    double time=1/(double)((random()%10)+5);
    clock_t t=clock();
    before=my_health+1;
    while(my_health>0&&enemy_health>0){
        if(((double)(clock()-t)/CLOCKS_PER_SEC)>=time){
            my_health-=1;
            t=clock();
        }
    }
}

void *gurun(void *arg){
    int flag=1,i;
    char command[64];
    
    while(flag){
        puts("1. Find Match\n2. Logout");
        printf("%s","Choices : ");
        gets(command);
        if(tolower(command[0])=='f'){
            int time=random()%10;
            while(time>0){
                puts("Waiting For Player ....");
                sleep(1);
                time--;
            }
            my_health=100;
            enemy_health=100;
            int status;
            if(fork()==0){
                execlp("clear","clear",(char *)NULL);
            }
            while((wait(&status)>0));
            puts("game dimulai silahkan taptap secepat mungkin !!!!");
            pthread_t trd2;
            if(pthread_create(&(trd2),NULL,&langit,NULL)==1){
                printf("Thread %d Error\n",i);
            }
            while(my_health>0&&enemy_health>0){
                if(!kbhit()){
                    if(before-my_health==2){
                        before-=1;
                        printf("\t\t\t\t\t\t\t%d\n",my_health);
                    }
                }
                else if(getch()==32){
                    if(before-my_health==2){
                        before-=1;
                    }
                    printf("HIT!!\t\t\t\t\t\t\t%d\n",my_health);
                    enemy_health--;
                }
            }
            printf("%d\n",my_health);
            if(my_health>0){
                puts("Game berakhir kamu menang");
            }
            else{
                puts("Game berakhir kamu kalah");
            }
            pthread_join(trd2,NULL);
            sleep(4);
        }
        else{
            flag=0;
        }
        int status;
        if(fork()==0){
            execlp("clear","clear",(char *)NULL);
        }
        while((wait(&status)>0));
    }
    
}

int main() {
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    
    int status;
    if(fork()==0){
        execlp("clear","clear",(char *)NULL);
    }
    while((wait(&status)>0));

    char command[64];
    char username[64];
    char password[64];
    char message[64] = {0};
    srandom(time(NULL));

    while(1){
        puts("1. Login\n2. Register");
        printf("%s","Choices : ");

        gets(command);

        send(sock,command,strlen(command),0);
        if(command[0]=='q')break;

        gets(username);
        send(sock,username,strlen(username),0);

        gets(password);
        send(sock,password,strlen(password),0);

        read(sock,message,64);

        int status;
        if(fork()==0){
            execlp("clear","clear",(char *)NULL);
        }
        while((wait(&status)>0));
        puts(message);

        if(strcmp(message,"login success")==0){
            pthread_t trd1;
            if(pthread_create(&trd1,NULL,&gurun,NULL)==1){
                printf("Thread Error\n");
            }
            pthread_join(trd1,NULL);
        }
        
        memset(message,0,64);
    }
    return 0;    
}