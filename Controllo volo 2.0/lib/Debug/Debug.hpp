/**
\file

\brief Header della classe `Debug`
\date 5 - 17 luglio 2017
*/

/**
\class Debug
\brief Classe utile per il debug di un prgramma qualsiasi.


Informazioni generali
---------------------

Questa classe può essere usata come strumento di debug per un programma basato su
Arduino.

Ha le seguenti funzionalità:
1. stampa di messaggi e messaggi di errore sul monitor seriale
2. segnalazione di errori fatali e conseguente blocco defnitivo del programma
3. sospensione temporanea del programma a determinati _breakpoints_ inseriti nel
codice
4. assegnazione di un valore a qualsiasi tipo di variabile del programa in
esecuzione tramite il monitor seriale

Ogni notifica sul monitor seriale è accompagnata da una segnalazione luminosa
tramite un LED, che deve essere interamente dedicato a questa classe (se non si
hanno pin disponibili questa funzionalità può essere disattivata).

Le notifiche non sono rappresentate da stringhe di testo (che occuperebbero
troppa memoria su ATmega328), ma da numeri. Occorre quindi creare un file di
`#define` per associare ad ogni errore o messaggio un numero.

I messaggi e gli errori sono molto simili tra loro, differiscono solo per la
visibilità sia sul monitor seriale sia tramite il LED (che si accende più a
lungo per segnalare gli errori).
Gli errori fatali sono gli errori che compromettono irrimediabilmente il proseguimento
del programma. La funzione `erroreFatale` dopo aver segnalato una prima volta l'errore
avvia un loop senza uscita in cui continuerà ad inviare sul monitor seriale il
codice dell'errore e l'ora a cui si è verificato.

\note Nel file header di questa classe viene creata un'istanza (`extern`) disponibile
globalmente. Non è necessario crearne altre istanze, e non ha senso farlo;  il
comportamento di due istanze di questa classe in uno stesso programma non è definito.


Linee guida per lo sviluppo
---------------------------

La classe `Debug` deve avere il monopolio della comunicazione con il computer.
I suoi messaggi possono comprendere:
- errori
- checkpoints
- calcoli svolti: risultati finali o parziali
- qualsiasi tipo di input esterno
- messaggi ricevuti da sensori, telecomando, ...

Oltre a servire per il riconoscimento degli errori "attuali", la classe deve
essere scritta e usata in modo da generare un _log_ di tutto ciò che accade nel
programma. Questo _log_ invece di essere inviato al computer (che per la maggior
parte del tempo si presume non sia connesso al sistema) potrebbe anche essere
salvato da qualche parte sul robot per una lettura successiva.

Un compito secondario ma comunque importante di questa classe è bloccare il
sistema in caso di errore fatale.


Istruzioni per l'utilizzo
-------------------------
Segue un esempio dell'utilizzo più semplice possibile della classe Debug.
\include /esempi/debug_esempio_minimo.cpp
*/


#ifndef Debug_hpp
#define Debug_hpp

#include "Arduino.h"

#include "debug_impostazioni.hpp"



class Debug: public HardwareSerial {

public:

    ///Assegna tutti i valori di default
    Debug (void);

    /// \name Funzioni principali
    /// @{

    ///HardwareSerial::begin con una piccola aggiunta
    void begin(unsigned long baud, byte config = SERIAL_8N1);

    ///Stampa un messaggio
    void messaggio(int, long = 0, bool = false);
    ///Stampa un errore
    void errore(int, long = 0, bool = false);
    ///Blocca il brogramma e segnala un errore fatale
    void erroreFatale(int, long = 0);

    ///inserisci un breakpoint
    void breakpoint(int, long = 0, long = -1);


    #ifdef DEBUG_ABILITA
    #ifdef DEBUG_USA_SERIAL
    #ifdef DEBUG_USA_ASSEGNA

    ///assegna un valore a una variabile di qualsiasi tipo
    void assegnaValore(bool*, int, long = 0);
    void assegnaValore(char*, int, long = 0);
    void assegnaValore(byte*, int, long = 0);
    void assegnaValore(int*, int, long = 0);
    void assegnaValore(unsigned int* , int, long = 0);
    void assegnaValore(long*, int, long = 0);
    void assegnaValore(unsigned long*, int, long = 0);
    void assegnaValore(float*, int, long = 0);

    #endif
    #endif
    #endif


    ///Controlla il led; se è acceso ed è ora di spegnerlo lo spegne
    void controllaLed();

    /// @}


    /// \name Funzioni di modifica impostazioni
    /// @{

    /** \brief Cambia l'impostazione `usaSerial`
    Usare questa impostazione solo se si decide di sospendere l'utilizzo del serial
    a un certo punto del programma; se non lo si usa mai usare l'impostazione globale
    `DEBUG_USA_SERIAL` in modo che il codice relativo a Serial non sia nemmeno compilato.
    Non ha tuttavia molto senso usare questa classe senza serial.
    */
    void usaSerial(bool);
    ///cfr. `_consentiBreakpoint`
    void consentiBreakpoint(bool);
    ///cfr. `_usaSempreAttesaMassimaBreak`
    void usaSempreAttesaMassimaBreak(bool);
    ///cfr. `_attesaMassimaBreakpoint`
    void impostaAttesaMassimaBreak(unsigned int);
    ///cfr. `_stampaMessaggi`
    void stampaMessaggi(bool);
    ///cfr. `_stampaMinimo`
    void stampaMinimo(bool);
    ///cfr. `_ignoraCodice`
    void ignoraCodice(bool);
    ///cfr. `_aspettaFineNotifica`
    void impostaAspettaFineNotifiche(bool);
    ///Cambia il tempo minimo di attesa tra due segnali luminosi se DEBUG_ASPETTA_FINE_NOTIFICA
    void impostaDurataBuioDopoNotifica(int);
    ///\brief Cambia i tempi di accensione del led per i vari tipi di notifica
    ///parametri nell'ordine: mess-err-errFat
    void impostaDurateLuci(int, int, int);

    bool staUsandoSerial();

    /// @}


private:

    ///crea il tipo "super", per comodità
    typedef HardwareSerial super;

    ///accende il led e imposta correttametne le variabili associate ad esso
    void accendiLed(int); //int: durata luce. Vedi anche commento sotto.
    ///spegne il led e imposta correttametne le variabili associate ad esso
    void spegniLed(); //il pin è sottinteso: questa classe ne usa solo uno, "_pinLed"

    ///Blocca il programma fino a quando il LED si spegne più qualche ms.
    void aspettaFineNotifica();

    ///\name funzioni usate da `assegnaValore(bool*, int, long)` e tutte le altre
    ///simili. @{

    ///Chiedi all'utente di inserire un numero naturale, un numero intero, un numero razionale o un valore bool
    void ottieniNumeroSerial(unsigned long* , long* , float*, bool*);
    ///azioni da eseguire prima di `ottieniNumeroSerial`
    void azioniPrimaAssegnaValore(int, long);
    ///controlla che i valori siano nei limiti consentiti
    bool controllaLimiti(long, long, long);
    ///chiede all'utente di confermare il numero immesso
    bool confermaAssegnaValore();
    ///azioni da eseguire dopo `ottieniNumeroSerial`
    void azioniDopoAssegnaValore();

    /// @}


    //###### VARIABILI ######

    ///la classe ha a disposizione un led?
    const int _usaLed;
    ///Pin del led
    const int _pinLed;

    ///Consenti alla funzione `breakpoint`di interrompere il programma?
    bool _consentiBreakpoint;
    //ogni breakpoint "scade" dopo un certo tempo, anche se nn specificato esplicitamente
    // nella chiamate della funzione
    bool _usaSempreAttesaMassimaBreak;
    //tempo massimo di attesa in ms (max. 1 minuto)
    unsigned int _attesaMassimaBreakpoint;

    ///Stampa anche i messaggi (true) o solo le altre notifiche (false)?
    bool _stampaMessaggi;
    ///Stampa solo il minimo indispensabille (true)
    bool _stampaMinimo;
    ///Ignora il codice associato ai messaggi. Ha effetto solo se `_stampaMinimo == true`
    bool _ignoraCodice;

    ///Dopo ogni notifica blocca il programma fino a quando il LED si spegne
    bool _aspettaFineNotifica;
    ///Attesa (opzionale) dopo lo spegnimento del LED
    int _durataBuioDopoNotifica;

    ///Durata della luce per i messaggi
    int _durataLuceMessaggio;
    ///Durata della luce per gli errori
    int _durataLuceErrore;
    ///Durata del lempeggiamento per gli errori fatali
    int _durataLuceErroreFatale;

    ///\brief La classe può usare la porta seriale? Da usare solo per singole parti
    ///del codice; se si vuole disattivare Serial globalmente commentare
    /// DEBUG_USA_SERIAL nelle impostazioni.
    bool _usaSerial;



    ///Stato attuale del led
    bool _ledAcceso;
    ///Ora di accensione del led. Non ha senso se `_ledAcceso == false`
    unsigned long _tempoAccensioneLed;
    ///Tempo per cui deve stare attualmente acceso il led. Non ha senso se `_ledAcceso == false`
    int _durataLuceLed;

    ///Velocità della comunicazione seriale. Impostato da Debug::begin() con il valore
    //scelto dall'utente.
    long _baudComunicazioneSeriale;


};

//crea un'istanza della classe, esattamente come il framework Arduino fa per Serial
extern Debug debug;


#endif
