# Progetto di Sistemi Operativi – Anno Accademico 2021/2022
| Nome         | Matricola | Mail                  |
| ------------ | --------- | --------------------- |
| Name Surname | 0000000   | name.surname@best.uni |
| Name Surname | 0000000   | name.surname@best.uni |

> Data di consegna: 00/00/0000


- [Progetto di Sistemi Operativi – Anno Accademico 2021/2022](#progetto-di-sistemi-operativi--anno-accademico-20212022)
    - [Compilazione ed esecuzione](#compilazione-ed-esecuzione)
          - [](#)
    - [Sistema obiettivo](#sistema-obiettivo)
      - [Hardware](#hardware)
        - [Architettura](#architettura)
        - [CPU e memoria](#cpu-e-memoria)
      - [Software](#software)

### Compilazione ed esecuzione
Per compilare tutto il codice sorgente è necessario digitare il comando make all da shell. Durante la compilazione saranno generate le seguenti cartelle: 
+ **bin** File binari necessari per l'esecuzione.  
+ **log** File di log generati dai processi treni e da RBC.  
+ **obj** File oggetto risultanti dalla compilazione dei sorgenti.
+ **tmp** File creati dai processi durante l'esecuzione, necessari per il loro funzionamento.  
  
Digitando soltanto make o make help,  si ottiene un messaggio di aiuto che mostra tutti i possibili comandi.  

![makeHelp](./doc_imgs/make_help.png)

Il programma è eseguibile in modalità ETCS1 ed ETCS2. In entrambi i casi è necessario avviare tramite shell il file railway_manager situato nella cartella bin.   
Nella figura sotto è mostrato un esempio di avvio in modalità ETCS1. Eventualmente si può sostituire MAPPA1 con MAPPA2:

![ETCS1Start](./doc_imgs/ETCS1_start.png)

L'esecuzione in modalità ETCS2 richiede l'utilizzo di due shell. Si riporta sotto un possibile avvio:  

Shell 1:
###### ![ETCS2Start](./doc_imgs/ETCS2_start.png)

Shell 2:
![ETCS2RBCStart](./doc_imgs/ETCS2_RBC_start.png)
### Sistema obiettivo
#### Hardware
| Architettura | RAM          | CPU                     |
| ------------ | ------------ | ----------------------- |
| x86_64       | 8 GB         | Intel® Core™ i5-8250U   |
| x86_64       | Ubuntu 22.04 | Linux 5.15.0-56-generic |

##### Architettura
Nonostante il codice sia stato sviluppato su computer montanti architetture **x86_64**, non sono state utilizzate funzionalità specifiche di quest'ultima. Pertanto, è possibile compilare i sorgenti anche su computer che montano architetture diverse, come **arm**, **arm64** o **x86**. 
##### CPU e memoria

 Caratteristiche computazionali dell'esecuzione di un processo TRENO:

![Performance graphic]()

Dati di proc:
```
proc
```

La dimensione dei file di log, potrebbe rappresentare un limite per la memoria secondaria. Effettuando un semplice  [profiling](https://github.com/nmaggioni/pmon) di diversi componenti, i risultati ottenuti sono approssimativamente gli stessi e tutti molto bassi. Questo ci permette di non avere vincoli prestazionali, computazionali e di memoria primaria.

Il programma richiede poca memoria (primaria e secondaria) e CPU per essere eseguito. Le prestazioni necessarie sono quelle minime del sistema operativo più qualche MB di memoria primaria. Questo ci permette di usare il programma anche su sistemi embedded.


#### Software
| Sistema Operativo | Distribuzione | Kernel                  | File system |
| ----------------- | ------------- | ----------------------- | ----------- |
| GNU/Linux         | Ubuntu 22.04  | Linux 5.15.0-56-generic | EXT4        |
| GNU/Linux         | Ubuntu 22.04  | Linux 5.15.0-56-generic | EXT4        |

Il codice è stato sviluppato aderendo allo standard POSIX e utilizzando
le utility GNU presenti nei sistemi operativi della famiglia GNU/Linux.
Essendo lo standard POSIX e le librerie GNU molto usati in diversi sistemi
operativi, è probabile che sistemi operativi (BSD, MacOS, Solaris etc.) che
rispettano le caratteristiche descritte prima possano soddisfare le
necessità del programma. Cioé, sarà possibile compilare ed eseguire il codice
correttamente anche su suddetti sistemi.  

I test sono stati eseguiti nei sistemi operativi presenti nella tabella sopra.              


Non sono state usate funzionalità specifiche di un particolare kernel. Di conseguenza, l'utilizzo di un kernel diverso da quelli elencati non dovrebbe influenzare il corretto funzionamento del programma. Similmente, dovrebbe essere indifferente anche la scelta
della distribuzione Linux.  

Non avendo usato funzionalità proprie di un file system, non è necessario usarne uno in particolare. Se le richieste del programma in fatto di efficienza aumenteranno, quanto appena detto potrebbe non essere più vero.   

