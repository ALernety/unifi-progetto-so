# Progetto di Sistemi Operativi – Anno Accademico 2021/2022
| Nome         | Matricola | Mail                  |
| ------------ | --------- | --------------------- |
| Name Surname | 0000000   | name.surname@best.uni |
| Name Surname | 0000000   | name.surname@best.uni |

> Data di consegna: 00/00/0000


- [Progetto di Sistemi Operativi – Anno Accademico 2021/2022](#progetto-di-sistemi-operativi--anno-accademico-20212022)
  - [Compilazione ed esecuzione](#compilazione-ed-esecuzione)
          - [](#)

## Compilazione ed esecuzione
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

