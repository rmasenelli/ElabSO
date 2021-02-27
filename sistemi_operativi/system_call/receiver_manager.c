/// @file receiver_manager.c
/// @brief Contiene l'implementazione del receiver_manager.

#include "err_exit.h"
#include "defines.h"
#include "shared_memory.h"
#include "semaphore.h"
#include "fifo.h"
#include "pipe.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

typedef struct message_info{
    int  id;
    char *message;
    char *id_sender;
    char *id_reciver;
    int del_S1;
    int del_S2;
    int del_S3;
    char *type;
}message_info_t;

void startprocess(int signal){
    printf("Start process ");
}
char* intToChar(int pid);
int lenghtLine(int fd);
int lenghtString(char string[]);
int openFile(char pathFile[], mode_t mode);
char* readLine(int fd);
void writeLine(int fd, char string[]);
int lenghtString(char string[]);
void writeProcessInfo(int fd, char name[], int pid);

int main(int argc, char * argv[]) {
    
    char pathFileO[]="./OutputFiles/FX.csv";
    mode_t modeO=O_RDWR; // open
        
    mode_t modeC=S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    pid_t R1, R2, R3;
    int fileDesc;

    if((R1 = fork()) == 0){ // R1 in esecuzione

        signal(SIGCONT,startprocess);
        pause();
        printf("R1...\n");
        pathFileO[15]='6'; // opero sul file 6
        if(openFile(pathFileO,modeO)==-1){//IF FILE NOT EXIST
            char firstLine[]="Id;Message;Id Sender;Id Reciver;Time arrival;Time Depart.\n";
            fileDesc=creat(pathFileO,modeC);
            writeLine(fileDesc,firstLine);
            close(fileDesc);
        }
        sleep(1);
        _exit(0);
    }else{
        if(R1==-1){
            printf("Error creating process R1");
            _exit(1);
        }
        printf("Process ID Parents: %i\n", getpid());
        pathFileO[15]='9';
        if(openFile(pathFileO,modeO)==-1){//IF FILE NOT EXIST
            char firstLine[]="Id;PID\n";
            fileDesc=creat(pathFileO,modeC);
            writeLine(fileDesc,firstLine);
        }
        lseek(fileDesc,0L,SEEK_END);
        char pname[]="R1";
        writeProcessInfo(fileDesc,pname,R1);
        close(fileDesc);
    }

    if((R2 = fork()) == 0){ // R2 in esecuzione

        //QUI DEVE SCRIVERE SU F5.CSV
        signal(SIGCONT,startprocess);
        pause();
        printf("R2...\n");
        pathFileO[15]='5'; // opero sul file 6
        if(openFile(pathFileO,modeO)==-1){//IF FILE NOT EXIST
            char firstLine[]="Id;Message;Id Sender;Id Reciver;Time arrival;Time Depart.\n";
            fileDesc=creat(pathFileO,modeC);
            writeLine(fileDesc,firstLine);
        }
        sleep(2);
        _exit(0);
    }else{
        if(R2==-1){
            printf("Error creating process R2");
            _exit(1);
        }
        printf("Process ID Parents: %i\n", getpid());
        pathFileO[15]='9';
        if(openFile(pathFileO,modeO)==-1){//IF FILE NOT EXIST
            char firstLine[]="Id;PID\n";
            fileDesc=creat(pathFileO,modeC);
            writeLine(fileDesc,firstLine);
        }
        lseek(fileDesc,0L,SEEK_END);
        char pname[]="R2";
        writeProcessInfo(fileDesc,pname,R2);
        close(fileDesc);
    }

    if((R3 = fork()) == 0){ // R3 in esecuzione
        
        //QUI DEVE SCRIVERE SU F4.CSV
        signal(SIGCONT,startprocess);
        pause();
        printf("R3...\n");
        pathFileO[15]='4'; // opero sul file 6
        if(openFile(pathFileO,modeO)==-1){//IF FILE NOT EXIST
            char firstLine[]="Id;Message;Id Sender;Id Reciver;Time arrival;Time Depart.\n";
            fileDesc=creat(pathFileO,modeC);
            writeLine(fileDesc,firstLine);
            close(fileDesc);
        }
        sleep(3);
        _exit(0);
    }else{
        if(R3==-1){
            printf("Error creating process R3");
            _exit(1);
        }
        printf("Process ID Parents: %i\n", getpid());
        pathFileO[15]='9';
        if(openFile(pathFileO,modeO)==-1){//IF FILE NOT EXIST
            char firstLine[]="Id;PID\n";
            fileDesc=creat(pathFileO,modeC);
            writeLine(fileDesc,firstLine);
            close(fileDesc);
        }
        lseek(fileDesc,0L,SEEK_END);
        char pname[]="R3";
        writeProcessInfo(fileDesc,pname,R3);
        close(fileDesc);
    }
    sleep(1);
    kill(R1, SIGCONT);
    wait(0);
    kill(R2, SIGCONT);
    wait(0);
    kill(R3, SIGCONT);
    wait(0);
    return 0;

}

int openFile(char pathFile[], mode_t mode){
    int fd;
    fd=open(pathFile,mode);
    if(fd==-1){//Error, file not found
        printf("Error File not Found. Creating file...\n");
        return fd;
    }
    return fd;
}

char* readLine (int fd){
    int l=lenghtLine(fd);
    char *s=(char *)malloc(sizeof(char)*l);
    if(read(fd,s,l)==-1){
        printf("Error read file");
        _exit(1);
    }
    return s;   
}

void writeLine(int fd, char string[]){ 
    int lenght=lenghtString(string);
    if(write(fd,string,sizeof(char)*lenght)==-1){
        printf("Error write in file.");
        _exit(1);
    }
}

void writeProcessInfo(int fd,char name[], int pid){
    char *buffer;
    char *tmp=intToChar(pid);
    int lenght, i=0;
    lenght=lenghtString(tmp)+4;
    buffer=(char*)malloc(sizeof(char)*lenght);
    for(i; i<(lenght-1); i++){
        if(i<=1){
            buffer[i]=name[i];
        }
        else if(i==2){
            buffer[i]=';';
        }
        else {
            buffer[i]=tmp[i-3];
        }
    }
    buffer[i]='\n';
    writeLine(fd,buffer);
}

int lenghtString(char s[]){
    int i=0;
    for(i; s[i]!='\0'; i++){
    }
    return (i);
}

int lenghtLine(int fd){
    int lenght=1;
    char n;
    while(read(fd,&n,1)!=0 && n!='\n'){
        lenght++;
    }

    if(lseek(fd,-(lenght),SEEK_CUR)==-1){
        printf("Error set lseek");
        _exit(1);
    }
    return lenght;
}

char* intToChar(int pid){
    char *aint;
    char *res;
    aint=(char *)malloc(sizeof(char));
    int tmp=pid;
    int i;
    
    for(i=0; tmp!=0; i++){
        aint=realloc(aint,(i+1)*sizeof(char));
        aint[i]=((tmp%10)+48);
        tmp=tmp/10;
    }
    
    res=(char*)malloc(sizeof(char)*(i+1));
    int j=0;
    i=i-1;
    for(i;i>=0;i--){
        res[j]=aint[i];
        j++;
    }
    return res;
}