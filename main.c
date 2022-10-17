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
#include "REGISTRO.h"
#include "TRENO.h"


int main(int argc, char const *argv[]) {
    int MA1,MA2,MA3,MA4,MA5,MA6,MA7,MA8,MA9,MA10,MA11,MA12,MA13,MA14,MA15,MA16;
    int T1,T2,T3,T4,T5;
    int registroPid;


    umask(000);
    MA1=creaSegmento("MA1");
    MA2=creaSegmento("MA2");
    MA3=creaSegmento("MA3");
    MA4=creaSegmento("MA4");
    MA5=creaSegmento("MA5");
    MA6=creaSegmento("MA6");
    MA7=creaSegmento("MA7");
    MA8=creaSegmento("MA8");
    MA9=creaSegmento("MA9");
    MA10=creaSegmento("MA10");
    MA11=creaSegmento("MA11");
    MA12=creaSegmento("MA12");
    MA13=creaSegmento("MA13");
    MA14=creaSegmento("MA14");
    MA15=creaSegmento("MA15");
    MA16=creaSegmento("MA16");
    inizializzaSegmento(MA1);
    inizializzaSegmento(MA2);
    inizializzaSegmento(MA3);
    inizializzaSegmento(MA4);
    inizializzaSegmento(MA5);
    inizializzaSegmento(MA6);
    inizializzaSegmento(MA7);
    inizializzaSegmento(MA8);
    inizializzaSegmento(MA9);
    inizializzaSegmento(MA10);
    inizializzaSegmento(MA11);
    inizializzaSegmento(MA12);
    inizializzaSegmento(MA13);
    inizializzaSegmento(MA14);
    inizializzaSegmento(MA15);
    inizializzaSegmento(MA16);

    close(MA1);
    close(MA2);
    close(MA3);
    close(MA4);
    close(MA5);
    close(MA6);
    close(MA7);
    close(MA8);
    close(MA9);
    close(MA10);
    close(MA11);
    close(MA12);
    close(MA13);
    close(MA14);
    close(MA15);
    close(MA16);


    if( ( registroPid=fork() )<0 ){
    perror("errore durante le creazione del processo registro");
    exit(EXIT_FAILURE);
   }

   /*esecuzione processo registro*/
   if(registroPid==0){

     char* mappa1[100]={
       "S1,MA1,MA2,MA3,MA8,S6",
       "S2,MA5,MA6,MA7,MA3,MA8,S6",
       "S7,MA13,MA12,MA11,MA10,MA9,S3",
       "S4,MA14,MA15,MA16,MA12,S8"
     };


      /*creazione 5 pipe con nome*/
    int pipeT1=creaPipeConNome("itinerarioT1");
    int pipeT2=creaPipeConNome("itinerarioT2");
    int pipeT3=creaPipeConNome("itinerarioT3");
    int pipeT4=creaPipeConNome("itinerarioT4");

     /*invio itinerario ai processi treni*/
    write(pipeT1,mappa1[0],strlen(mappa1[0])+1);
    write(pipeT2,mappa1[1],strlen(mappa1[1])+1);
    write(pipeT3,mappa1[2],strlen(mappa1[2])+1);
    write(pipeT4,mappa1[3],strlen(mappa1[3])+1);

    close(pipeT1);
    close(pipeT2);
    close(pipeT3);
    close(pipeT4);
    exit(0);
  }//fine processo registro


    /*creazione treno T1*/
    if((T1=creaTreno())<0){
       perror("errore");
       exit(EXIT_FAILURE);
    }

    /*esecuzione treno T1*/
    if(T1==0){
       char itinerario[100];
       int T1;
       riceviItinerario("itinerarioT1",itinerario);
       printf("itinerarioT1: %s\n",itinerario );
      T1=open("T1.log",O_CREAT|O_WRONLY|O_TRUNC,0666);
        if(T1==-1)
          perror("errore durante la creazione di T1.log");
        percorriItinerario(itinerario,"T1",T1);

       }//fine processo treno1

/*creazione treno T2*/
 if((T2=creaTreno())<0){
   perror("errore");
   exit(EXIT_FAILURE);
 }
 /*esecuzione treno T2*/
  if(T2==0){
    char itinerario[100];
    riceviItinerario("itinerarioT2",itinerario);
    printf("itinerarioT2: %s\n",itinerario);

    int T2;
    T2=open("T2.log",O_CREAT|O_WRONLY|O_TRUNC,0666);
      if(T2==-1)
        perror("errore durante la creazione di T2.log");
      percorriItinerario(itinerario,"T2",T2);




  }//fine processo treno3







  if((T3=creaTreno())<0){
    perror("errore");
    exit(EXIT_FAILURE);
  }

  /*esecuzione treno T3*/
   if(T3==0){
     char itinerario[100];
     riceviItinerario("itinerarioT3",itinerario);
     printf("itinerarioT3: %s\n",itinerario );



     int T3;
     T3=open("T3.log",O_CREAT|O_WRONLY|O_TRUNC,0666);
       if(T3==-1)
         perror("errore durante la creazione di T3.log");
       percorriItinerario(itinerario,"T3",T3);

    }//fine processo treno3


    /*creazione processo treno T4*/
   if((T4=creaTreno())<0){
     perror("errore");
     exit(EXIT_FAILURE);
   }
   /*esecuzione treno T4*/
    if(T4==0){
      char itinerario[100];
      riceviItinerario("itinerarioT4",itinerario);
      printf("itinerarioT4: %s\n",itinerario );



      int T4;
      T4=open("T4.log",O_CREAT|O_WRONLY|O_TRUNC,0666);
        if(T4==-1)
          perror("errore durante la creazione di T4.log");
        percorriItinerario(itinerario,"T4",T4);


       }
       //fine processo treno4




/*creazione processo treno T4*/
    if((T5=creaTreno())<0){
      perror("errore");
      exit(EXIT_FAILURE);
    }
    /*esecuzione treno T5*/
     if(T5==0){
     //if(argv[2]=="MAPPA1")
         exit(0);
     }//fine processo treno5
return 0;


  }
