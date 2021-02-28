/// @file client.c
/// @brief Contiene l'implementazione del client.

#include "defines.h"
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
// 21 CARATTERI DI INT
char* read_line(int fd, char* string);
int line_length(int fd);
void write_line(int fd, char string[]);
int string_length(char string[]);
char * getList(int fd, int space, char* string);


int main(int argc, char * argv[]) {
    char pathFileI[]="./InputFiles/F7.csv";
    char pathFileO[]="./OutputFiles/F7_out.csv";
    int fdI;
    int fdO;
    mode_t modeO=O_RDWR;
    mode_t modeC=S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    
    if((fdI=open(pathFileI,modeO))==-1){//END PROGRAM IF FILE NOT FOUND
        printf("Error - File Input Not Found!\n");
        _exit(0);
    }
    off_t end=lseek(fdI,0L,SEEK_END);
    off_t start=lseek(fdI,0L,SEEK_SET);
    read_line(fdI,"*");
    char *tmp;
    if((fdO=open(pathFileO,modeO))==-1){//CREATE NEW FILE 
        char firstLine[]="Id;Delay;Target;Action\n";
        fdO=creat(pathFileO,modeC);
        write_line(fdO,firstLine);
        close(fdO);
    }

    int spaceInt=line_length(fdO);

    while(start!=end){
        char* readL=read_line(fdI,readL);
        start=lseek(fdI,0L,SEEK_CUR);
        printf("Line read: %s\n",readL);
        char *oldList=getList(fdO,spaceInt,oldList);
        printf("OLD LIST: %s",oldList);
        lseek(fdO,(spaceInt+string_length(readL))*sizeof(char),SEEK_SET);
        write_line(fdO,oldList);
        lseek(fdO,spaceInt*sizeof(char),SEEK_SET);
        write_line(fdO,readL);
    }
    
    return 0;
}



char * getList(int fd, int space, char* string){ 
    off_t end=lseek(fd,0L,SEEK_END);
    off_t start=lseek(fd,space*sizeof(char),SEEK_SET);
    
    string=(char*)malloc(sizeof(char));
    for(int i=0; start!=end; i++){
        string=realloc(string,sizeof(char)*(i+1));
        char c;
        read(fd,&c,1);
        start=lseek(fd,0L,SEEK_CUR);
        string[i]=c;
    }
    
    printf("STRING: %s",string);
    return string;

}



void write_line(int fd, char string[]){ // scrive una linea sul file
    int lenght=string_length(string);
    if(write(fd,string,sizeof(char)*lenght)==-1){
        printf("Error write in file.");
        _exit(1);
    }
}



int string_length(char s[]){
    int i=0;
    for(i; s[i]!='\0'; i++){
    }
    return (i);
}
char* read_line (int fd, char* string){
    int l=line_length(fd);
    char *s=(char *)malloc(sizeof(char)*l);
    if(read(fd,s,l)==-1){
        printf("Error read file");
        _exit(1);
    }
    return s;   
}

int line_length(int fd){
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