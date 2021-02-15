/// @file receiver_manager.c
/// @brief Contiene l'implementazione del receiver_manager.

#include "err_exit.h"
#include "defines.h"
#include "shared_memory.h"
#include "semaphore.h"
#include "fifo.h"
#include "pipe.h"
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char * argv[]) {
    
    char pathFileS[]="./OutputFiles/FX.csv";
    mode_t modeO=O_RDWR; // open
    pid_t R1, R2, R3;
    int fileDesc;

    if((R1 = fork()) == 0){ // R1 in esecuzione

        //QUI DEVE SCRIVERE SU F6.CSV
        pathFileS[15]='6'; // opero sul file 6
        fileDesc=openFile(pathFileS, modeO);
        sleep(1);
        _exit(0);

    }else{
        printf("Errore generazione R1!\n");
    }

    if((R2 = fork()) == 0){ // R2 in esecuzione

        //QUI DEVE SCRIVERE SU F5.CSV
        sleep(2);
        _exit(0);
    }else{
        printf("Errore generazione R2!\n");
    }

    if((R3 = fork()) == 0){ // R3 in esecuzione
        
        //QUI DEVE SCRIVERE SU F4.CSV
        sleep(3);
        _exit(0);
    }else{
        printf("Errore generazione R3!\n");
    }


    
    return 0;

}
