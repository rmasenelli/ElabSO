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
    int  id;
    char *message;
    char *id_sender;
    char *id_reciver;
    int del_S1;
    int del_S2;
    int del_S3;
    char *type;
}message_info_t;

typedef struct clocktime{
    int hour;
    int min;
    int sec;
}clocktime_t;

typedef struct sender_traffic_info{
    int id;
    char* message;
    int id_sender;
    int id_reciver;
    time_t time_arrival;
    time_t time_depart;
}sender_traffic_info_t;


void startprocess(int signal){
    printf("Start process ");
}

//FUNCTION

int lenghtLine(int fd);
int openFile(char pathFile[], mode_t mode);
char* readLine(int fd);
void writeLine(int fd, char string[]);
int lenghtString(char string[]);
void writeProcessInfo(int fd, char name[], int pid);
char* intToChar(int pid);
char* readInformation(int *index, int *lenght, char* info);
int charToInt(char* string);
sender_traffic_info_t getSenderInfoByMessageInfo(message_info_t messageinfo);
char* getStringBySenderInfo(sender_traffic_info_t info);
void concatStringInfo(char* string1, char* string2);
char* cutCharInInt(char * string);
void setPointerToEndFile(int fd);

message_info_t getInformationByLine(char* line);
int main(int argc, char * argv[]) {
    //VARIABLES TO OPEN FILE
    char pathFileI[]="./InputFiles/FX.csv";
    char pathFileO[]="./OutputFiles/FX.csv";
    mode_t modeO=O_RDWR; //OPEN
    mode_t modeC=S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH; //CREATE
    int fileDesc;
    

    //OPEN FILE F8.csv IF EXIST-OR CREATE THEN
    pathFileO[15]='8';
    fileDesc=openFile(pathFileO,modeO);
    if(fileDesc==-1){
        printf("File not Exist..\nCreate a new file F8.csv");
        fileDesc=creat(pathFileO,modeC);
        char head[]="Sender Id;PID\n";
        writeLine(fileDesc, head);
        close(fileDesc);
    }

    //CREATE CHILD PROCESS S1
    pid_t S1;
    if((S1=fork())==0){
        signal(SIGCONT,startprocess);
        pause();
        printf("S1...\n\n");
        pathFileO[15]='1';
        pathFileI[14]='0';
        int fileDescI;
        if(openFile(pathFileO,modeO)==-1){//IF FILE NOT EXIST
            char firstLine[]="Id;Message;Id Sender;Id Reciver;Time arrival;Time Depart.\n";
            fileDesc=creat(pathFileO,modeC);
            writeLine(fileDesc,firstLine);
            close(fileDesc);
        }
        message_info_t mesInfo;
        fileDesc=openFile(pathFileO,modeO);
        fileDescI=openFile(pathFileI, modeO);
        readLine(fileDescI);
        off_t end=lseek(fileDescI,0L,SEEK_END);
        off_t start=lseek(fileDesc,0L,SEEK_SET);
        while(start!=end){
            char* info=readLine(fileDescI);
            start=lseek(fileDesc,0L,SEEK_CUR);
            printf("Reading information...");
            mesInfo=getInformationByLine(info);
            sender_traffic_info_t send=getSenderInfoByMessageInfo(mesInfo);
            char *buffer=getStringBySenderInfo(send);
            printf("%s\n",buffer);
            printf("Start writing info message on %s\n",pathFileO);
            setPointerToEndFile(fileDesc);
            writeLine(fileDesc,buffer);
        }
        sleep(1);
        exit(0);
    }
    else{
        //WRITE PROCESS ID ON FILE BY PARENT (SENEDER MANAGER)
        if(S1==-1){
            printf("Error Create a process");
            _exit(1);
        }
        printf("Process ID Parents: %i\n", getpid());
        pathFileO[15]='8';
        fileDesc=openFile(pathFileO,modeO);
        lseek(fileDesc,0L,SEEK_END);
        char pname[]="S1";
        writeProcessInfo(fileDesc,pname,S1);
        close(fileDesc);
    }

    //CREATE CHILD PROCESS S2 
    pid_t S2;
    if((S2=fork())==0){
        signal(SIGCONT,startprocess);
        pause();
        printf("S2\n\n");
        pathFileO[15]='2';
        char firstLine[]="Id;Message;Id Sender;Id Reciver;Time arrival;Time Depart.\n";
        int create;
        int open;
        if(openFile(pathFileO,modeO)==-1){//IF FILE NOT EXIST, CREATE NEW FILE
            printf("\nCreate a new File F2.csv.\n");
            if(create=creat(pathFileO, modeC)==-1){
                printf("Error: Create a file");
                _exit(1);
            }
            open=openFile(pathFileO, modeO); 
            if(open==-1){
                _exit(1);
            }
            writeLine(open, firstLine);
            printf("Done!\n");
        }
        sleep(2);
        exit(0);
    }
    else{
        //WRITE PROCESS ID ON FILE BY PARENT (SENEDER MANAGER)
        if(S2==-1){
            printf("Error Create a process");
            _exit(1);
        }
        pathFileO[15]='8';
        fileDesc=openFile(pathFileO, modeO);
        lseek(fileDesc,0L,SEEK_END);
        char pname[]="S2";
        writeProcessInfo(fileDesc,pname,S2);
        close(fileDesc);
    }

    //CREATE CHILD PROCESS S3
    pid_t S3;
    if((S3=fork())==0){
        signal(SIGCONT, startprocess);
        pause();
        printf("S3\n\n");
        pathFileO[15]='3';
        char firstLine[]="Id;Message;Id Sender;Id Reciver;Time arrival;Time Depart.\n";
        int create;
        int open;
        if(openFile(pathFileO,modeO)==-1){//IF FILE NOT EXIST, CREATE NEW FILE
            printf("\nCreate a new File F3.csv.\n");
            if(create=creat(pathFileO, modeC)==-1){
                printf("Error: Create a file");
                _exit(1);
            }
            open=openFile(pathFileO, modeO); 
            if(open==-1){
                _exit(1);
            }
            writeLine(open, firstLine);
            printf("Done!\n");
        }
        sleep(3);
        exit(0);
    }
    else{
        if(S3==-1){
            printf("Error Create a process");
            _exit(1);
        }
        //WRITE PROCESS ID ON FILE BY PARENT (SENEDER MANAGER)
        pathFileO[15]='8';
        fileDesc=openFile(pathFileO, modeO);
        lseek(fileDesc,0L,SEEK_END);
        char pname[]="S3";
        writeProcessInfo(fileDesc,pname,S3);
        close(fileDesc);
    }
    printf("All child process ready!\n");
    sleep(1);
    kill(S1, SIGCONT);
    wait(0);
    kill(S2, SIGCONT);
    wait(0);
    kill(S3, SIGCONT);
    wait(0);
    return 0;
}

void setPointerToEndFile(int fd){
    lseek(fd, 0L, SEEK_END);
}

char* getStringBySenderInfo(sender_traffic_info_t info){
    char *string;
    char *tmp;
    char *in;
    string=intToChar(info.id);
    tmp=info.message;
    concatStringInfo(string,tmp);
    tmp=intToChar(info.id_sender);
    concatStringInfo(string,tmp);
    tmp=intToChar(info.id_reciver);
    concatStringInfo(string,tmp);
    
    concatStringInfo(string,"\n");
    return string;
}

void concatStringInfo(char *string1, char* string2){
    int i;
    int j;
    int max=lenghtString(string1)+lenghtString(string2);
    int fw=lenghtString(string1);
    string1=realloc(string1,sizeof(char)*(max+2));
    for(i=lenghtString(string1),j=0; i<=max; i++,j++){
        if(i==fw){
            string1[i]=';';//OVERWRITE END OF STRING
            j--;
        }
        else{
            string1[i]=string2[j];
        }
    }
    string1[i]='\0';

}

sender_traffic_info_t getSenderInfoByMessageInfo(message_info_t im){
    sender_traffic_info_t senderInfo;
    senderInfo.id=im.id;
    senderInfo.message=im.message;
    senderInfo.id_sender=charToInt(cutCharInInt(im.id_sender));
    senderInfo.id_reciver=charToInt(cutCharInInt(im.id_reciver));

    return senderInfo;
}

char* cutCharInInt(char * string){
    int l=lenghtString(string);
    int i;
    int newl=0;
    char *res=malloc(sizeof(char));
    for(i=0; i<l;i++){
        if(string[i]>=48 && string[i]<=57){
            ++newl;
            res=realloc(res,sizeof(char)*l);
            res[newl-1]=string[i];
        }
    }
    ++newl;
    res=realloc(res,sizeof(char)*newl);
    res[newl-1]='\0';
    return res;
}

message_info_t getInformationByLine(char* info){
    int index=0;
    int lenghtInf=0;
    message_info_t mes;
    mes.id=charToInt(readInformation(&index,&lenghtInf,info));
    mes.message=readInformation(&index,&lenghtInf,info);
    mes.id_sender=readInformation(&index,&lenghtInf,info);
    mes.id_reciver=readInformation(&index,&lenghtInf, info);
    mes.del_S1=charToInt(readInformation(&index,&lenghtInf,info));
    mes.del_S2=charToInt(readInformation(&index,&lenghtInf,info));
    mes.del_S3=charToInt(readInformation(&index,&lenghtInf,info));
    mes.type=readInformation(&index,&lenghtInf,info);
    printf("Read Complete!\n");
    printf("ID Message: %i\n",mes.id);
    printf("Message: %s\n",mes.message);
    printf("id Sender: %s\n",mes.id_sender);
    printf("id Reciver: %s\n",mes.id_reciver);
    printf("Del S1: %i\n",mes.del_S1);
    printf("Del S2: %i\n",mes.del_S2);
    printf("Del S3: %i\n",mes.del_S3);
    printf("Type send message: %s\n",mes.type);
    return mes;

}

char* readInformation(int *index, int *lenght, char* info){
    char *tmp;
    tmp=(char*)malloc(sizeof(char));

    for(*lenght=0,*index; info[*index]!=';' && info[*index]!='\n'; *index=*index+1,*lenght=*lenght+1){
        tmp=realloc(tmp,sizeof(char)*(*lenght+1));
        tmp[*lenght]=info[*index];
    }
    *index=*index+1;
    tmp=realloc(tmp,sizeof(char)*(*lenght+1));
    tmp[*lenght]='\0';
    return tmp;
}

int charToInt(char* string){
    int res=0;
    int lenght=lenghtString(string);
    for(int i=lenght-1; i>=0; i--){
        if(i==lenght-1){
            res=string[i];
            res=res-48;
        }
        else {
            int n=lenght-1;
            int molt=1;
            while(n!=i){
                molt=molt*10;
                --n;
            }
            res=res+((int)string[i]*molt);
        }
    }
    return res;
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
    int lenght,i=0;
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