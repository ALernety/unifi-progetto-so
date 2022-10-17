#include "TRENO.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

/*---------------------------riceviItinerario---------------------------------*/
void riceviItinerario(char *nome, char *itinerario) {
  int pipeFd;
  pipeFd = open(nome, O_RDONLY);
  read(pipeFd, itinerario, 100);
  close(pipeFd);
}
/*----------------------------------------------------------------------------*/

/*---------------------dividiItinerario---------------------------------------*/
static char **dividiItinerario(char *itinerario) {
  char **segmentArray = NULL;
  char *segment = strtok(itinerario, ",");
  int count = 0;
  while (segment) {
    segmentArray = realloc(segmentArray, sizeof(char *) * ++count);
    if (segmentArray == NULL) {
      exit(-1);
    }
    segmentArray[count - 1] = segment;
    segment = strtok(NULL, ",");
  }
  segmentArray = realloc(segmentArray, sizeof(char *) * (count + 1));
  segmentArray[count] = 0;
  return segmentArray;
}
/*----------------------------------------------------------------------------*/

/*-----------------------logCurrentDate-------------------------------------*/
static void logCurrentDate(int fd) {
  char buffer[30];
  size_t i;
  struct tm tim;
  time_t now;
  now = time(NULL);
  tim = *(localtime(&now));
  i = strftime(buffer, 30, "%d %b %Y; %H:%M:%S\n", &tim);
  int byteScritti = write(fd, buffer, i);
  if (byteScritti != i) {
    perror("errore durante la scrittura della data corrente");
  }
}
/*----------------------------------------------------------------------------*/

/*-------------------ottieniStazionePartenza---------------------------------*/
static char *ottieniStazionePartenza(char **arrItinerario, char *treno,
                                     int fd) {
  char *segmentoCorrente = arrItinerario[0];
  printf("stazione di partenza di %s: %s\n", treno, segmentoCorrente);
  char buffer[20];
  sprintf(buffer, "%s[Attuale: %s], ", buffer, segmentoCorrente);
  int writtenChars = write(fd, buffer, strlen(buffer));
  if (writtenChars != strlen(buffer)) {
    perror("errore durante la scrittura in un file di log");
  }
  return segmentoCorrente;
}
/*----------------------------------------------------------------------------*/

/*------------------------gestisciarrivoInStazione-----------------------*/
static void gestisciarrivoInStazione(char *treno, char *segmentoCorrente,
                                     char *segmentoSuccessivo, int fd) {
  printf("treno %s: arrivo in stazione: %s\n", treno, segmentoSuccessivo);
  FILE *fileSegmentoCorrente = fopen(segmentoCorrente, "r+");
  fwrite("0", 1, 1, fileSegmentoCorrente);
  fclose(fileSegmentoCorrente);
  scriviSegmentoSuFile(fd, segmentoSuccessivo, 1);
  scriviSegmentoSuFile(fd, "-- ", 0);
  logCurrentDate(fd);
}
/*------------------------------------------------------------------------*/

/*-------------scriviSegmentoSuFile-------------------------------------------*/
static void scriviSegmentoSuFile(int fd, char *segmento, int flag) {
  char buffer[30];
  if (flag) {
    strcat(buffer, "[Attuale: ");
  } else {
    strcat(buffer, "[Next: ");
  }

  strcat(buffer, segmento);
  strcat(buffer, "], ");

  int writtenChars = write(fd, buffer, strlen(buffer));
  if (writtenChars != strlen(buffer)) {
    perror("errore durante la scrittura in un file di log");
  }
}
/*----------------------------------------------------------------------------*/

/*---------------------------------percorriItinerario-------------------------*/
void percorriItinerario(char *itinerario, char *treno, int fd) {
  int contatore = 1;
  char permesso;
  char *segmentoSuccessivo;
  char **arrItinerario = dividiItinerario(itinerario);
  char *segmentoCorrente = arrItinerario[0];
  printf("stazione di partenza di %s: %s\n", treno, segmentoCorrente);
  scriviSegmentoSuFile(fd, segmentoCorrente, 1);

  while (1) {
    /*INIZIO ITERAZIONE WHILE*/
    segmentoSuccessivo = arrItinerario[contatore];
    printf("treno %s vuole accedere a %s\n", treno, segmentoSuccessivo);
    // provare a scrivere qui il segmento corrente
    scriviSegmentoSuFile(fd, segmentoSuccessivo, 0);
    logCurrentDate(fd);

    if (segmentoSuccessivo[0] == 'S') {
      gestisciarrivoInStazione(treno, segmentoCorrente, segmentoSuccessivo, fd);
      close(fd);
      exit(0);
    }

    FILE *fileSegmentoSuccessivo = fopen(segmentoSuccessivo, "r+");
    fread(&permesso, 1, 1, fileSegmentoSuccessivo);
    printf("permesso per %s di accesso a %s: %c\n", treno, segmentoSuccessivo,
           permesso);
    fseek(fileSegmentoSuccessivo, 0, SEEK_SET);

    if (permesso == '0') {
      printf("permesso accordato. %s accede a %s\n", treno, segmentoSuccessivo);
      fwrite("1", 1, 1, fileSegmentoSuccessivo);
      fseek(fileSegmentoSuccessivo, 0, SEEK_SET);
      if (segmentoCorrente[0] == 'S') {
        segmentoCorrente = segmentoSuccessivo;
        printf("il treno %s si trova in %s \n", treno, segmentoCorrente);
        scriviSegmentoSuFile(fd, segmentoCorrente, 1);
        fclose(fileSegmentoSuccessivo);
        contatore++;
      } else {
        FILE *fileSegmentoCorrente = fopen(segmentoCorrente, "r+");
        fwrite("0", 1, 1, fileSegmentoCorrente);
        fseek(fileSegmentoCorrente, 0, SEEK_SET);
        segmentoCorrente = segmentoSuccessivo;
        scriviSegmentoSuFile(fd, segmentoCorrente, 1);
        fclose(fileSegmentoSuccessivo);
        fclose(fileSegmentoCorrente);
        contatore++;
      }
    } else
      printf("il treno %s non può accedere a a %s \n", treno,
             segmentoSuccessivo);
    sleep(2);
  }
  printf("uscita funzione per %s\n", treno);
}
