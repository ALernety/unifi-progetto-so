#include "../PADRE_TRENI/PADRE_TRENI.h"
#include "../PADRE_TRENI/TRENO.h"
#include "../REGISTRO/REGISTRO.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

enum Booleans { false, true };
typedef enum Booleans boolean;

int main(int argc, char const *argv[]) {
  char helpStr[450];
  sprintf(
      helpStr,
      "\033[31mNot enough arguments! Example of use:\033[0m\n"
      "\n"
      "Usage: %s MODE [RBC] MAP\n\n"
      "\033[36mMODE\033[0m possible values are:\n"
      "    \033[36mETCS1\033[0m    - program has to be used without server\n"
      "    \033[36mETCS2\033[0m    - program has to be used with server RBC\n"
      "\n"
      "\033[36mRBC\033[0m used to activate server\n"
      "\n"
      "\033[36mMAP\033[0m possible values are:\n"
      "    \033[36mMAPPA1\033[0m   - manage trains with map one \n"
      "    \033[36mMAPPA2\033[0m   - manage trains with map two\n",
      argv[0]);
  switch (argc) {
  case 3: {
    boolean isNotETCS = strcmp(argv[1], "ETCS1") && strcmp(argv[1], "ETCS2");
    boolean isNotMAP = strcmp(argv[2], "MAPPA1") && strcmp(argv[2], "MAPPA2");
    if (isNotETCS || isNotMAP) {
      printf("%s", helpStr);
      exit(EXIT_FAILURE);
    }
    break;
  }
  case 4: {
    boolean isNotMAP = strcmp(argv[3], "MAPPA1") && strcmp(argv[3], "MAPPA2");
    if (strcmp(argv[1], "ETCS2") || strcmp(argv[2], "RBC") || isNotMAP) {
      printf("%s", helpStr);
      exit(EXIT_FAILURE);
    }
    break;
  }
  default:
    printf("%s", helpStr);
    exit(EXIT_FAILURE);
    break;
  }

  int MA[16];
  int Train[5];
  // int T1, T2, T3, T4, T5;
  int registroPid;

  umask(000);
  for (int i = 0; i < 16; i++) {
    char file[5];
    sprintf(file, "MA%d", i + 1);
    MA[i] = creaSegmento(file);
    inizializzaSegmento(MA[i]);
    close(MA[i]);
  }

  registroPid = fork();
  if (registroPid < 0) {
    perror("errore durante le creazione del processo registro");
    exit(EXIT_FAILURE);
  }

  /*esecuzione processo registro*/
  if (registroPid == 0) {

    char *mappa1[100] = {"S1,MA1,MA2,MA3,MA8,S6", "S2,MA5,MA6,MA7,MA3,MA8,S6",
                         "S7,MA13,MA12,MA11,MA10,MA9,S3",
                         "S4,MA14,MA15,MA16,MA12,S8"};
    printf("%s\n", mappa1[0]);
    printf("%s\n", mappa1[1]);
    printf("%s\n", mappa1[2]);
    printf("%s\n", mappa1[3]);

    for (int i = 0; i < 4; i++) {
      char itineraryName[100];
      sprintf(itineraryName, "itinerarioT%i", i + 1);
      /*creazione pipe con nome*/
      int pipeTreno = creaPipeConNome(itineraryName);
      /*invio itinerario ai processi treni*/
      write(pipeTreno, mappa1[i], strlen(mappa1[i]) + 1);
      close(pipeTreno);
    }
    exit(EXIT_SUCCESS);
    // fine processo registro
  } else {
    for (int i = 0; i < 4; i++) {

      /*creazione treno*/
      Train[i] = creaTreno();

      /*esecuzione treno Train[i]*/
      if (Train[i] == 0) {
        char itinerario[100];
        char itineraryName[100];
        char logFile[100];
        char trainName[100];
        sprintf(itineraryName, "itinerarioT%i", i + 1);
        sprintf(logFile, "T%i.log", i + 1);
        sprintf(trainName, "T%i", i + 1);
        riceviItinerario(itineraryName, itinerario);
        printf("%s: %s\n", itineraryName, itinerario);
        Train[i] = open(logFile, O_CREAT | O_WRONLY | O_TRUNC, 0666);
        if (Train[i] == -1) {
          perror("errore durante la creazione di Train[i].log");
          exit(EXIT_FAILURE);
        }
        percorriItinerario(itinerario, trainName, Train[i]);
        // fine processo treno
        exit(EXIT_SUCCESS);
      }
    }
    for (int i = 0; i < 4; i++) {
      wait(NULL);
    }
  }
}