/// @file sender_manager.c
/// @brief Contiene l'implementazione del sender_manager.

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
//DEFINE
#define ID 2
#define Q "Q"
#define SH "SH"
#define FIFO "FIFO"

//TYPEDEF

typedef struct message_info{
    int id;
    char *message;
    char id_sender[ID];
    int del_S1;
    int del_S2;
    int del_s2;
    char *type;
}message_info_t;

typedef struct clocktime{
    int hour;
    int min;
    int sec;
}clocktime_t;

typedef struct sender_traffic_info{
    int id;
    char *message;
    int id_sender;
    int id_reciver;
    time_t time_arrival;
    time_t time_depart;
}sender_traffic_info_t;

//FUNCTION

int lenghtLine(int fd);
int openFile(char pathFile[], mode_t mode);
void readLine(int fd);
void writeLine(int fd, char string[]);
int lenghtString(char string[]);
void writeProcessInfo(int fd, char name[], int pid);

int main(int argc, char * argv[]) {
    //VARIABLES TO OPEN FILE
    char path[]="./InputFiles/F0.csv";
    char pathFileS[]="./OutputFiles/FX.csv";
    mode_t modeO=O_RDWR;
    mode_t modeC=S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    int fileDesc;
    
    //READ FILE F8.csv IF EXIST OR CREATE THEN
    pathFileS[15]='7';
    fileDesc=openFile(pathFileS,modeO);
    if(fileDesc==-1){
        printf("File not Exist..\nCreate a new file F8.csv");
        fileDesc=creat(pathFileS,modeC);
        char head[]="Sender Id;PID\n";
        writeLine(fileDesc, head);
    }



    //OPEN INPUT FILE
    fileDesc=openFile(path,modeO);
    if(fileDesc==-1){
        _exit(1);//File not Found
    }

    //READ FIRST LINE (DESCRIPTION)
    readLine(fileDesc);
    
    int S1;
    
    //SUSPEND PARENT PROCESS
    wait(0);

    //CREATE A CHILD PROCESS S1
    if((S1=fork())==0){
        //ADD PROCESS IN F8.csv
        char pathFileS[15]='7';
        fileDesc=openFile(pathFileS, modeO);
        lseek(fileDesc,0L,SEEK_END);
        char pname[]="S1";
        writeProcessInfo(fileDesc,pname,S1);
        
        pathFileS[15]='1';
        char firstLine[]="Id;Message;Id Sender;Id Reciver;Time arrival;Time Depart.\n";
        int create;
        int open;
        if(openFile(pathFileS,modeO)==-1){//IF FILE NOT EXIST, CREATE NEW FILE
            printf("Create a new File F1.csv.\n");
            if(create=creat(pathFileS, modeC)==-1){
                printf("Error: Create a file");
                _exit(1);
            }
            open=openFile(pathFileS, modeO); 
            if(open==-1){
                _exit(1);
            }
            writeLine(open, firstLine);
            printf("Done!\n");
        }
        S1=getpid();
        kill(S1,SIGKILL);
        }
    
    int S2;
    wait(0);

    //CREATE A CHILD PROCESS S2
    if((S2=fork())==0){
        pathFileS[15]='2';
        char firstLine[]="Id;Message;Id Sender;Id Reciver;Time arrival;Time Depart.\n";
        int create;
        int open;
        mode_t modeO=O_RDWR;
        mode_t modeC=S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
        if(openFile(pathFileS,modeO)==-1){
            printf("Create a new File F2.csv.\n");
            if(create=creat(pathFileS, modeC)==-1){
                printf("Error: Create a file");
                _exit(1);
            }
            open=openFile(pathFileS, modeO); 
            if(open==-1){
                _exit(1);
            }
            writeLine(open, firstLine);
            printf("Done!\n");
        }
        S2=getpid();
        kill(S2,SIGKILL);
    }
    int S3;
    wait(0);
    //CREATE A PROCESS CHILD S3
    if((S3=fork())==0){
        pathFileS[15]='3';
        char firstLine[]="Id;Message;Id Sender;Id Reciver;Time arrival;Time Depart.\n";
        int create;
        int open;
        mode_t modeO=O_RDWR;
        mode_t modeC=S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
        if(openFile(pathFileS,modeO)==-1){
            printf("Create a new File F3.csv.\n");
            if(create=creat(pathFileS, modeC)==-1){
                printf("Error: Create a file");
                _exit(1);
            }
            open=openFile(pathFileS, modeO); 
            if(open==-1){
                _exit(1);
            }
            writeLine(open, firstLine);
            printf("Done!\n");
        }
        S3=getpid();
        kill(S3,SIGKILL);
    }
    
    return 0;
}


int openFile(char pathFile[], mode_t mode){
    int fd;
    fd=open(pathFile,mode);
    if(fd==-1){//Error, file not found
        printf("Error File not Found\n");
        return fd;
    }
    return fd;
}

void readLine (int fd){
    int l=lenghtLine(fd);
    char *s=(char *)malloc(sizeof(char)*l);
    if(read(fd,s,l)==-1){
        printf("Error read file");
        _exit(1);
    }
    printf("String read: %s",s);
    
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
    char tmp[20];
    int lenght;
    sprintf(tmp,"%d",pid);
    lenght=lenghtString(tmp)+3;
    buffer=(char*)malloc(sizeof(char)*lenght);
    for(int i=0; name[i]!='/n'; i++){
        if()
    }
}

int lenghtString(char s[]){
    int i=0;
    for(i; s[i]!='\n'; i++){
    }
    return i;
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

