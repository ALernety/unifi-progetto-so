
/* legge i dati contenuti nel buffer della pipe che ha il primo
   parametro come nome e li inserisce nella stringa passata come secondo
   parametro.
*/
void riceviItinerario(char*,char*);


/*Presa in input una stringa, la divide in tante sottostringhe
  usando il carattere ',' come separatore. Restituisce un array
  contenente tutte le stringhe derivanti da questa separazione.

  Ad esempio, se abbiamo una stringa di questo tipo:
        char* str={"S2,MA5,MA6,MA7,MA3,MA8,S6"};
        char** arrStr=riceviItinerario(str);
  arrStr avrà questa forma:
        arrStr={{"S2"},
                {"MA5"},
                {"MA6"},
                {"MA7"},
                {"MA3"},
                {"MA8"},
                {"S6"}};
*/
static char** dividiItinerario(char*);


/*
  Scrive sul file di log riferito dal parametro intero le
  informazioni riguardanti la stazione di partenza di un treno.
*/
static char* ottieniStazionePartenza(char**,char*,int);



/*
  Scrive sul file di log riferito dal parametro intero le
  informazioni relative alla stazione di arrivo e libera
  il segmento occupato in precedenza.
*/
static void gestisciarrivoInStazione(char*,char*,char*,int);


/*
 Scrive la data corrente nel file riferito dal parametro intero
 Il formato è formato dd-mm-yyyy hh:mm:ss.
*/
static void scriviDataSuFile(int );


/*
 Scrive sul file avente come file descriptor il primo parametro
 le informazioni sul segmento passato come secondo parametro.
 In particolare, se il flag è impostato a un valore diverso da 0
 il segmento è quello in cui il treno si trova attualmente, altrimenti
 è il prossimo segmento o stazione da attraversare.
*/
static void scriviSegmentoSuFile(int,char*,int);

/*
 Funzione principale che gestisce il passaggio di un treno
 dalla stazione di partenza a quella di arrivo. Tutte le
 informazioni sui segmenti attraversati vengono salvate nel
 file di log riferito dal parametro intero.
*/
void percorriItinerario(char*,char*,int );
