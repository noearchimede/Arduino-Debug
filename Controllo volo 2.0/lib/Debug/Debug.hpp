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

*/


#ifndef Debug_hpp
#define Debug_hpp

#include "Arduino.h"

#include "Debug_impostazioni.hpp"


class Debug {

public:


    ///Assegna tutti i valori di default e collega HardwareSerial
    Debug (HardwareSerial& hwserial);

    /// \name Funzioni principali
    /// @{

    ///Inizializza la classe Debug
    void inizializza(long, byte = DEBUG_DEFAULT_PIN_LED);

    ///Stampa un messaggio
    void messaggio(int, long = 0, bool = false);
    ///Stampa un errore
    void errore(int, long = 0, bool = false);
    ///Blocca il brogramma e segnala un errore fatale
    void erroreFatale(int, long = 0);

    ///inserisci un breakpoint
    void breakpoint(int, long = 0, unsigned long = 0);


    #ifdef DEBUG_ABILITA
    #ifdef DEBUG_ABILITA_ASSEGNA

    ///assegna un valore a una variabile di qualsiasi tipo
    void assegnaValore(bool*, int, long = 0);
    void assegnaValore(int8_t*, int, long = 0);
    void assegnaValore(uint8_t*, int, long = 0);
    void assegnaValore(int16_t*, int, long = 0);
    void assegnaValore(uint16_t* , int, long = 0);
    void assegnaValore(int32_t*, int, long = 0);
    void assegnaValore(uint32_t*, int, long = 0);
    void assegnaValore(float*, int, long = 0);

    #endif
    #endif

    ///versione il più breve possibile di controllaLed, chiamata esclusivamente dall'ISR
    void controllaLedInterrupt();
    ///Controlla il led; se è acceso ed è ora di spegnerlo lo spegne. Destinato all'uso "manuale".
    void controllaLed();

    /// @}


    /// \name Funzioni di modifica impostazioni
    /// @{

    ///cfr. `_usaHardwareSerial`
    void usaSerial(bool);
    ///cfr. `_usaLed`
    void usaLed(bool);
    ///cfr. `_pinLed`
    void impostaPinLed(int);
    ///cfr. `_usaInterrupt`
    void usaInterrupt(bool);
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


    enum Tipo { tipo_bo,
        tipo_u8, tipo_u16, tipo_u32,
        tipo_i8, tipo_i16, tipo_i32,
        tipo_f
    };
    union ValoreQualsiasi {
        bool b;
        uint8_t u8; uint16_t u16; uint32_t u32;
        int8_t i8; int16_t i16; int32_t i32;
        float fl;
    };
    struct NumeroQualsiasi {
        NumeroQualsiasi(Tipo t):tipo(t){};
        ValoreQualsiasi valore;
        const Tipo tipo;
    };


    ///\name Funzioni di stampa e ricezione dati
    ///@{

    void print(const char[]);
    void print(const String&);
    void print(uint8_t);
    void print(int8_t);
    void print(uint16_t);
    void print(int16_t);
    void print(uint32_t);
    void print(int32_t);
    void print(float);

    //0xFF è un valore non valido per `config`. Lascio la scelta del default a Serial.
    void serialBegin(long, byte = 0xFF);
    void serialEnd();

    int available();
    int read();

    ///@}


    ///\name Funzioni di gestione del LED
    ///@{

    ///abilita o disabilita l'interrupt usato per controllare il LED
    void abilitaInterrupt(bool);

    ///accende il led e imposta correttametne le variabili associate ad esso
    void accendiLed(int); //int: durata luce. Vedi anche commento sotto.
    ///spegne il led e imposta correttametne le variabili associate ad esso
    void spegniLed(); //il pin è sottinteso: questa classe ne usa solo uno, "_pinLed"

    ///Blocca il programma fino a quando il LED si spegne più qualche ms.
    void aspettaFineNotifica();

    ///@}

    ///\name funzioni usate da `assegnaValore(bool*, int, long)` e tutte le altre
    ///simili. @{

    ///Chiedi all'utente di inserire un numero naturale, un numero intero, un numero razionale o un valore bool
    void ottieniNumeroSerial(NumeroQualsiasi&);
    ///azioni da eseguire prima di `ottieniNumeroSerial`
    void azioniPrimaAssegnaValore(int, long);
    ///azioni da eseguire dopo `ottieniNumeroSerial`
    void azioniDopoAssegnaValore();

    /// @}


    //###### VARIABILI ######

    ///\name Impostazioni
    ///@{

    ///brief La classe può usare la porta seriale?
    bool _usaHardwareSerial;
    ///Velocità della comunicazione seriale.
    long _baudComunicazioneSeriale;

    ///la classe ha a disposizione un led?
    bool _usaLed;
    ///Pin del led
    int _pinLed;
    ///usa l'interrupt TIMER0_COMPA per controllare il LED
    bool _usaInterrupt;

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

    ///@}

    ///\name Variabili non modificabili dall'utente
    ///@{

    ///Riferimento a un'istanza di HardwareSerial (cioé a Serial)
    HardwareSerial& _hardwareSerial;


    ///Stato attuale del led
    bool _ledAcceso;
    ///Ora a cui dovrà essere spento il led. Non ha senso se `_ledAcceso == false`
    unsigned long _tempoSpegnimentoLed;


    ///\brief Bit corrispondente al pin nella porta definiita sopra
    ///\details cfr. implementazione di `digitalWrite(uint8_t, uint8_t)` di Arduino
    uint8_t _bitMaskPinLed;
    ///\brief pointer al regstro di output per una porta
    ///\details cfr. implementazione di `digitalWrite(uint8_t, uint8_t)` di Arduino
    volatile uint8_t *_regPinLed;

    ///@}

};


//dichiara l'esistenza di un'istanza della classe (dichiarata in `Debug_1_base.cpp`)
extern Debug debug;







#endif
