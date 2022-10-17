#include <stdio.h>
#include "PADRE_TRENI.h"
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define DEFAULT_PROTOCOL 0


int creaSegmento(char* nome){
  int fd;
  if((fd=open(nome,O_RDWR|O_CREAT|O_TRUNC,0666))<0){
      perror("errore durante l'apertura di un segmento");
      exit(EXIT_FAILURE);
   }
  return fd;
}

void inizializzaSegmento(int fd){
  if((fd=write(fd,"0",1))!=1){
     perror("errore durante la scrittura in un segmento");
     exit(EXIT_FAILURE);
    }
}


int creaTreno(){
  int pid=fork();
  if (pid<0){
    perror("errore durante le creazione di un processo treno");
    exit(EXIT_FAILURE);
  }
  return pid;
}
