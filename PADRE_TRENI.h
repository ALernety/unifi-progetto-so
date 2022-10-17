
/* Semplice funzione che esegue una fork. Serve per creare i processi treni*/
int creaTreno();

/* Crea un file che rappresenta un segmento dell'itinerario */
int creaSegmento(char *);

/* scrive il carattere '0' nel file riferito dal file descriptor passato come
 * parametro*/
void inizializzaSegmento(int);
