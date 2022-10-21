
/* legge i dati contenuti nel buffer della pipe che ha il primo
   parametro come nome e li inserisce nella stringa passata come secondo
   parametro.
*/
void riceviItinerario(char *, char *);

/*
 Funzione principale che gestisce il passaggio di un treno
 dalla stazione di partenza a quella di arrivo. Tutte le
 informazioni sui segmenti attraversati vengono salvate nel
 file di log riferito dal parametro intero.
*/
void percorriItinerario(char *, char *, int);
