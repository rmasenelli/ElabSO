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

char* readLine(int fd);
int lenghtLine(int fd);

int main(int argc, char * argv[]) {
    char pathFileI[]="./InputFiles/F7.csv";
    char pathFileO[]="./OutputFiles/F7.csv";
    int fd;
    mode_t modeO=O_RDWR;
    fd=open(pathFileI,modeO);
    
    if(fd==-1){//END PROGRAM IF FILE NOT FOUND
        printf("Error - File Input Not Found!\n");
        _exit(0);
    }
    off_t end=lseek(fd,0L,SEEK_END);
    off_t start=lseek(fd,0L,SEEK_SET);
    readLine(fd);
    char *tmp;
    while(start!=end){
        tmp=readLine(fd);
        printf("%s\n",tmp);
        start=lseek(fd,0L,SEEK_CUR);   
    }
    return 0;
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