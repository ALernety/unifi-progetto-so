#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

char** dividiItinerario(char*);
int main(int argc, char const *argv[]) {

  char itinerario2[]="S1,MA1,MA2,MA3,MA8,S6";
  char itinerario3[]="S2, MA5, MA6, MA7, MA3, MA8, S6";
  char itinerario4[]="S7, MA13, MA12, MA11, MA10, MA9, S3";
  char itinerario5[]="S4, MA14, MA15, MA16, MA12, S8";
  char *treno="T1";
  int T1=open("T1.log",O_CREAT|O_WRONLY|O_APPEND|O_TRUNC,0666);
  char** res1 =dividiItinerario(itinerario2);


char *segmentoCorrente=res1[0];
printf("stazione di partenza di %s: %s\n",treno,segmentoCorrente );
char buffer[50];
strcat(buffer,"[Attuale: ");
strcat(buffer,segmentoCorrente);
strcat(buffer,"], ");
printf("ok\n" );

int byte=write(T1,buffer,strlen(buffer));
if(byte!=strlen(buffer))
perror("errore durante la scrittura in un file di log");

char buffer2[30];
size_t i;
struct tm tim;
time_t now;
now=time(NULL);
tim=*(localtime(&now));
i=strftime(buffer2,30,"%d %b %Y; %H:%M:%S\n",&tim);
int byteScritti=write(T1,buffer2,i);
if(byteScritti!=i)
perror("errore durante la scrittura della data");

write(T1,"scrittA",8);





printf("stampa itinerario2: \n");
for (int i = 0; i < 7; i++) {
  printf("res[%d]=%s\n",i,res1[i] );
}

return 0;
}

char** dividiItinerario(char*itinerario){
  char** res=NULL;
  char *tok=strtok(itinerario,",");
  int count=0;
  while(tok){
    res=realloc(res,sizeof(char*)* ++count);
    if(res==NULL)
    exit(-1);
  res[count-1]=tok;
  tok=strtok(NULL,",");
}
res=realloc(res,sizeof(char*)* (count+1));
res[count]=0;
return res;
}
