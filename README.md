**_Purposes_** : this project is a simplified representation inspired by interactions between sensors, ECU and HMI in an ADAS System made for the OS Lab of University of Florence Computer Science graduate course for year 2019/2020.

Feel free to contribute with ideas and opinions, it will be great!
And if this works helps someone I would be glad if you would share it with me.
Have fun!


# ADAS made trivial: rappresentazione ispirata alle interazioni in un sistema di guida autonoma

## 1. Obiettivo complessivo

Obiettivo del progetto è costruire un’architettura, estremamente stilizzata e rivistata, per sistemi ADAS,
descrivendo possibili interazioni e alcuni comportamenti tra componenti in scenari specifici.
Dove non specificato con il termine facoltativo, i requisiti definiti hanno carattere prescrittivo, con significato equivalente alla parola MUST secondo l’RFC 2119.

## 2. Descrizione del sistema e del suo comportamento

Il sistema comprende vari componenti, che mostrano una specifica funzionale ed interazioni secondo quanto descritto di seguito. 
I componenti si dividono in Human-Machine Interface, Sensori, Attuatori, Central ECU. La divisione è la seguente:

- **Componente Human-Machine Interface**
- **Attuatori**:
    - Componente steer-by-wire
    - Componente throttle control
    - Componente brake-by-wire
- **Sensori**
    - Componente front windshield camera
    - Componente forward facing radar(facoltativo)
    - Componente park assist
    - Componente surroundviewcameras(facoltativo) 
    - Componenteblindspot(facoltativo)
- **Componente Central ECU**


### _Componente Human-Machine Interface_ ###
Questo componente interagisce con la Central ECU, per mostrare a video, come output, quanto comunicato dalla Central ECU, e per inviare input ricevuti dall’utente tramite la tastiera.

Il componente è realizzato tramite due terminali. Un terminale è utilizzato solo per mostrare i valori di output (terminale di output), mentre l’altro terminale (terminale di input) è utilizzato solo per raccogliere e inviare input alla Central CPU.

Come output, il processo stampa a schermo sul terminale di output tutte le azioni comandate dalla Central ECU.

Gli input possibili, che l’utente può digitare sul terminale di input, sono:

- “INIZIO”: avvia il veicolo verso la sua destinazione
- “PARCHEGGIO”: si attiva la procedura di parcheggio, che conclude il percorso.
 
**Suggerimento.** Per semplicità, il processo di output può essere realizzato come un processo che legge continuamente dal file di log della Central ECU.

**Facoltativo.** E’ possibile arricchire l’output con ulteriori stampe a video, al fine di migliorare la leggibilità delle informazioni visualizzate.


### _Componente steer-by-wire_ ### 
Questo componente riceve dalla Central ECU il comando di sterzata DESTRA o SINISTRA, ed esegue la sterzata in 4 secondi (ovvero, l’azione di sterzare dura 4 secondi).

Ogni secondo, il componente stampa nel file di log steer.log: “NO ACTION”, “STO GIRANDO A DESTRA”, “STO GIRANDO A SINISTRA”, sulla base dell’azione in corso.


### _Componente throttle control_ ### 
Questo componente riceve il comando di accelerazione dalla Central ECU, nel formato “INCREMENTO X”, dove X indica l’aumento di velocità richiesto. La velocità è incrementata di 5 KM/H ogni secondo, fino a raggiungere il valore richiesto. 

Ogni secondo, il componente stampa nel file di log throttle.log: “NO ACTION”, oppure “AUMENTO 5”, sulla base dell’azione in corso.

**Facoltativo.** Per ogni secondo di accelerazione, c’è una probabilità di 10-5 che l’acceleratore fallisca. In tal caso, il componente throttle control invia un segnale alla Central ECU per evidenziare tale evento, e l’accelerazione in corso è annullata.


### _Componente brake-by-wire_ ###
Il componente riceve dalla Central ECU il comando di decelerazione o un segnale di pericolo. Il comando di decelerazione è ricevuto nel formato “FRENO X”, dove X indica la riduzione di velocità richiesta. La velocità è decrementata, ogni secondo, di 5 KM/H, fino a raggiungere il valore richiesto. Ogni secondo, il componente stampa nel file di log brake.log: “NO ACTION”, “DECREMENTO 5”, ”ARRESTO AUTO”, sulla base dell’azione in corso.


Se il componente riceve il segnale di pericolo dalla Central ECU, arresta l’auto (azione istantanea). Questo equivale a scrivere “ARRESTO AUTO” nel file di log brake.log.

**Nota.** Per facilitare la realizzazione, le caratteristiche dell’input sono tali che non possono arrivare comandi contrastanti. Ad esempio, mentre si sta effettuando una variazione di direzione, non arriveranno comandi di sterzo nella direzione opposta. Un simile ragionamento vale per accelerazione e decelerazione.


### _Componente front windshield camera_ ### 
Il componente iterativamente legge dati da una sorgente e li invia alla Central ECU. In dettaglio, ogni 10 secondi legge una riga dal file frontCamera.data, e trasmette il dato letto alla Central ECU. I dati inviati sono registrati nel file di log camera.log.


### _Componente forward facing radar (facoltativo)_ ### 
Il componente iterativamente legge i dati da una sorgente e li invia alla Central ECU. In dettaglio, ogni 2 secondi prova a leggere 24 byte da /dev/random. Se riesce a leggere 24 byte, questi sono trasmessi alla Central ECU. Altrimenti (se legge meno di 24 byte), non invia dati alla Central ECU. I dati inviati sono registrati nel file di log radar.log.


### _Componente park assist_ ### 
Quando il componente riceve un comando di attivazione da parte della Central ECU, per la durata di 30 secondi, legge iterativamente una volta al secondo 4 byte da /dev/urandom e li invia alla Central ECU. I dati inviati sono registrati nel file di log assist.log.

**Facoltativo.** Quando si attiva l’interazione con park assist, la Central ECU sospende (o rimuove) tutti I sensori e attuatori, tranne park assist e surround view cameras.
   
**Facoltativo.** 
Il componente Park assist non è generato all’avvio del Sistema, ma creato dalla Central ECU al bisogno.


### _Componente surround view cameras (facoltativo)_ ### 
Il componente agisce solo quando park assist è attivo. Finchè park assist è attivo, 1 volta al secondo, legge 16 byte da /dev/urandom e li invia alla Central ECU. I dati inviati sono registrati nel file di log cameras.log.


### _Componente blind spot (facoltativo)_ ###
Il componente agisce solo su richiesta della Central ECU. Quando si effettua una curva a destra o sinistra, per l’intera durata della sterzata, iterativamente legge ogni 0.5 secondi 8 bytes da /dev/urandom e li invia alla Central ECU. dati inviati sono registrati nel file di log spot.log.

**Facoltativo.**. Blind Spot cameras può essere costamente in attesa di un segnale direttamente da steer-by- wire, invece che di una richiesta dalla Central ECU.


### _Componente Central ECU_ ### 
La Central ECU effettua le seguenti azioni. All’avvio del sistema, imposta la velocità attuale a 0, e ignora qualsiasi informazione ricevuta dai sensori, finchè riceve dall’HMI il comando “INIZIO”. Questo indica l’avvio del percorso.

Dopo aver ricevuto il comando INIZIO, durante la sua esecuzione, la Central ECU riceve continuamente dati dai sensori.

Per ciascuna informazione ricevuta da front windshield camera, la Central ECU effettua le seguenti azioni:

– Se l’informazione è un numero, questo valore indica la velocità desiderata del veicolo. Se necessario, la Central ECU interagisce con i componenti throttle control o brake-by-wire per impostare la velocità richiesta.

– Se l’informazione è l’istruzione DESTRA o SINISTRA, la Central ECU invia il comando corrispondente a steer-by-wire, per comandare la sterzata.

– Se l’informazione è PERICOLO, la Central ECU invia un segnale di pericolo al componente brake-by-wire. 

In seguito all’esecuzione di un segnale di pericolo, la Central ECU imposta la velocità a zero. E’ quindi
necessario che l’utente digiti nuovamente INIZIO, per proseguire il percorso.

**Facoltativo.** I dati ricevuti da Forward facing radar sono continuamente analizzati e raccolti: se i valori raccolti contengono almeno uno tra i valori i) 0xA00F; ii) 0xB072; iii) 0x2FA8; iv) 0x8359; v) 0xCE23, allora la Central ECU invia un segnale di pericolo a brake-by-wire.


Se il comando ricevuto dalla HMI è invece “PARCHEGGIO”, la Central ECU comunica l’arresto del veicolo a brake-by-wire.


Quando la velocità raggiunge 0, Central ECU attiva Park assist ultrasonic sensors e Surround view cameras. Se la Central ECU non riceve da nessuno dei due, per 30 secondi, uno dei valori: i) 0x172A, ii) 0xD693, iii) 0x, iv) 0xBDD8, v) 0xFAEE, vi) 0x4300, l’auto è parcheggiata e la missione termina. Altrimenti, la Central ECU ri- avvia la procedura di PARCHEGGIO, attivando attiva Park assist ultrasonic sensors e Surround view cameras.
  
**Nota.** Surround view cameras è facoltativo, come già specificato sopra.

**Facoltativo.** Il comando di PARCHEGGIO potrebbe arrivare mentre i vari attuatori stanno eseguendo ulteriori comandi (accelerare o sterzare). I vari attuatori interrompono le loro azioni, per avviare le procedure di parcheggio.

**Facoltativo.** Durante una sterzata, la Central ECU legge i valori di Blind spot rear radars. Se per la durata della sterzata la Central ECU non riceve nessuno dei valori i) 0x414E, ii) 0x4452, iii) 0x4C41, iv) 0x424F, v) 0x5241, vi) 0x544F, vii) 0x8359, viii) 0xA01F, ix) 0x5249, x) 0x5100, allora la sterzata termina correttamente, altrimenti si invia un segnale di pericolo a brake-by-wire.

**Facoltativo.** Se la Central ECU riceve il segnale di fallimento accelerazione da throttle control, imposta la velocità a 0 e invia all’output della HMI un messaggio di totale terminazione dell’esecuzione.
Tutti i comandi inviati dalla Central ECU a qualunque componente sono inseriti in un file di log ECU.log e stampati a video tramite la HMI.

## 3. Note implementative del programma richiesto.

Ciascun componente deve essere rappresentato da almeno un processo. Non sono ammessi thread. E’
ammessa l’esecuzione come root, se necessaria (da motivare nella relazione).

## 4. Modalità di avvio ed esecuzione

Il programma è eseguibile in due modalità, NORMALE ed ARTIFICIALE. Le due modalità sono selezionate tramite un parametro di input, da impostare opportunamente a NORMALE o ARTIFICIALE, passato come argomento al main (ovvero, tramite argv[1]).

Nel caso della modalità di avvio NORMALE: i componenti forward facing radar, park assist, surround view cameras, blind spot, leggono i dati da /dev/random e /dev/urandom come specificato nel testo del progetto.

Nel caso della modalità di avvio ARTIFICIALE: i componenti forward facing radar, park assist, surround view cameras, blind spot leggono i dati:

    - Invece che da /dev/urandom, dal file urandomARTIFICIALE.binary 
    - Invece che da /dev/random, dal file randomARTIFICIALE.binary
    
**Facoltativo.** Utilizzare un’unica shell per lanciare l’intero progetto.
