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


int main(int argc, char * argv[]) {
    char pathFileI[]="./InputFiles/F7.csv";
    char pathFileO[]="./OutputFiles/F7_out.csv";
    int fd;
    mode_t modeO=O_RDWR;
    mode_t modeC=S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    fd=open(pathFileI,modeO);
    
    if(fd==-1){//END PROGRAM IF FILE NOT FOUND
        printf("Error - File Input Not Found!\n");
        _exit(0);
    }
    off_t end=lseek(fd,0L,SEEK_END);
    off_t start=lseek(fd,0L,SEEK_SET);
    read_line(fd,"c");
    char *tmp;
    int fdO=open(pathFileO,modeO);
    if(fdO==-1){
        char firstLine[]="Id;Delay;Target;Action\n";
        fdO=creat(pathFileO,modeC);
        write_line(fdO,firstLine);
        close(fdO);
    }
    fdO=open(pathFileO,modeO);
    printf("OFFSET: %d", line_length(fdO));
    lseek(fdO,(off_t)((line_length(fdO))*sizeof(char)),SEEK_SET);
    tmp=read_line(fd,tmp);
    write_line(fdO,tmp);
    printf("OFFSET: %d", line_length(fdO));
    lseek(fdO,0L,SEEK_SET);
    printf("OFFSET: %d", line_length(fdO));
    lseek(fdO,(off_t)((line_length(fdO))*sizeof(char)),SEEK_SET);
    tmp=read_line(fd,tmp);
    write_line(fdO,tmp);

    
    /*tmp=read_line(fd,tmp); // legge la prima riga
    printf("%s\n",tmp); //manda a capo
    start=lseek(fd,0L,SEEK_CUR);
    printf("TEST %d",line_length(fdO)); // scrive l'intestazione
    lseek(fdO,(off_t)((line_length(fdO))+1*sizeof(char)),SEEK_SET);
    write_line(fdO,tmp);
    write_line(fdO,"\n");
    tmp=read_line(fd,tmp);
    lseek(fdO,0L,SEEK_SET);
    lseek(fdO,(off_t)((line_length(fdO)+1)*sizeof(char)),SEEK_SET);
    printf("%s",tmp);
    write_line(fdO,tmp);
    write_line(fdO,"\n");*/
    
    return 0;
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