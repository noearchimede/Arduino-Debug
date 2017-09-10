/**
\file

\brief Header della classe `Debug`
\date 5 - 17 luglio 2017, 12 - 16, 26 agosto 2017
*/

/**
\class Debug
\brief Classe utile per il debug di un prgramma qualsiasi.


Nel file header di questa classe viene dichiarate un'istanza `extern` disponibile
globalmente chiamate `debug` e collegata a `Serial`. Non è necessario crearne altre
istanze, e non ha senso farlo;  il comportamento di due istanze di questa classe
in uno stesso programma non è definito.
*/


#ifndef Debug_hpp
#define Debug_hpp

#include "Arduino.h"
#include "Led.hpp"
#include "Comunicazione.hpp"
#include "Assegna.hpp"

class Debug {

public:

    //tipi
    enum Livello {
        debug,
        informazione,
        avviso,
        errore,
        niente
    };


    Debug(HardwareSerial&);

    /**\name Gestione classe
    @{*/

    /**
    L'inizializzazione completa ha due parametri:
    \param serialBaud velocità della comunicazione seriale
    \param ledPin pin del led dedicato al debug

    L'inizializzazione con un solo parametro inizializza in modo che non sia
    usato il led:
    ```cpp
    debug.begin(115200);
    ```
    equivale a
    ```cpp
    debug.abilitaLed(false);
    debug.begin(115200, 13);
    ```
    */
    void begin(long baudSerial);
    void begin(long baudSerial, int pinLed);


    ///\breif controlla se il LED deve essere spento.
    ///\note Se l'ISR è abilitata sarà suo compito chiamare questa funzione.
    void controllaLed();

    /**@}*/


    /**\name Log
    @{*/

    /**
    Le funzioni `info`, `warn` e `err` stampano dei messaggi sul monitor seriale
    e fanno accendere un led per un tempo che dipende dal tipo di messaggio.
    L'unica differenza tra di esse è il livello di importanza: i messaggi info
    possono essere disabilitati lasciando warn ed err, e warn lasciando err.
    Inoltre i messaggi err sono visivamente in rilievo nell'output.

    Ogni messaggi ha un nome o un codice. Il nome è una stringa di testo, il codice
    è un numero che rappresenta univocamente il messaggio. Conviene usare il
    nome se si ha abbastanza memoria a disposizione e altrimenti il codie, che
    evidentemente rende il file di log meno leggibile.
    Inoltre a ogni messaggio è possibile associare un valore di qualsiasi tipo
    aritmetico (bool, intero con o senza segno o decimale), che sarà stampato
    accanto al nome.

    Linee guida per la scelta delle funzioni:
    - `info(...)`: informazione sul corretto svolgimento del programma, es.:
        svolto un certo calcolo, nuovo sensore connesso, ...
    - `warn(...)`: avviso su un potenziale problema che non dovrebbe accadere
        ma non compromette irreversibilmente lo svolgimento del programma, es.:
        tentativo di connessione a un sensore già connesso, temperatura: -400°C
    - `err(...)`: errore, cioè avvenimento indesiderato che compromette, o
        potrebbe verosimilmente compromettere il programma, es.: myFloat è nan,
        divisione per 0
    */
    void info(int codice)                {messaggio(informazione, codice, 0, false);}
    void info(const String& nome)        {messaggio(informazione, nome, 0, false);}

    template <typename T>
    void info(int codice, T val)         {messaggio(informazione, codice, val, true);}
    template <typename T>
    void info(const String& nome, T val) {messaggio(informazione, nome, val, true);}

    /**vedi \ref info*/
    void warn(int codice)                {messaggio(avviso, codice, 0, false);}
    void warn(const String& nome)        {messaggio(avviso, nome, 0, false);}

    template <typename T>
    void warn(int codice, T val)         {messaggio(avviso, codice, val, true);}
    template <typename T>
    void warn(const String& nome, T val) {messaggio(avviso, nome, val, true);}

    /**vedi \ref info*/
    void err(int codice)                 {messaggio(errore, codice, 0, false);}
    void err(const String& nome)         {messaggio(errore, nome, 0, false);}

    template <typename T>
    void err(int codice, T val)          {messaggio(errore, codice, val, true);}
    template <typename T>
    void err(const String& nome, T val)  {messaggio(errore, nome, val, true);}

    /**@}*/


    /**\name Debug
    @{*/

    /**
    Segnala un errore fatale e blocca il programma. Il nome/codice dell'errore
    e il valore associato, se presenti, devono essere segnalati con la funzione
    `err` subito prima di chiamare questa funzione.
    */
    void errFat();

    /**
    Blocca il programma fino a quando l'utente invia un carattere qualsiasi
    sul monitor seriale. Il nome/codice del breakpoint e il valore associato,
    se presenti, devono essere segnalati con la funzione info.
    Opzionalmente si può specificare come parametro un tempo massimo di attesa
    dopo il quale il programma riprenderà come se l'utente avesse mandato un
    carattere.
    */
    void breakpoint();
    void breakpoint(unsigned long attesaMassima);


    /**
    Assegna un valore numerico naturale, intero o decimale o un valore booleano
    a una variabile del programma. La funzione richiede che sia specificato il
    nome della variabile (o comunque una stringa di testo che faccia capire
    all'utente di ceh variabile si tratta).
    */
    template<typename T>
    void assegnaValore(const String& nome, T* pointer);


    /**@}*/


    /**\name Setters e getters
    @{*/

    void abilitaComunicazione(bool x);
    void abilitaLed(bool x);

    void impostaLivello(Livello x);
    void verbose(bool x);
    void ignoraVal(bool x);
    void aspettaFineNotica(bool x);

    void impostaPinLed(byte x);
    void abilitaInterrupt(bool x);
    ///\note passare 0 nei parametri che non si vogliono modificare
    void impostaDurataLuce(unsigned long informazione, unsigned long avviso, unsigned long errore, unsigned long erroreFatale);

    void impostaBaudComunicazione(long x);

    /**@}*/


private:

    /**\name Funzioni private
    @{*/

    /**
    Funzione generica di stampa dei messaggi (info, avvisi, errori).
    \param tipoMess il timpo di messaggio da stampare
    \param nome il nome, che può essere di qualsiasi tipo stampabile con
    `Comunicazione::stampa`. Si pensa in particolare a una `String&` o un `int`.
    \param val valore numerico associato al messaggio.
    \param haVal se `false`, `val` sarà ignorato in tutta la funzione
    */
    template <typename Nome, typename Val>
    void messaggio(Livello tipoMess, Nome nome, Val val, bool haVal);


    /**@}*/

    /**\name Messaggi
    @{*/
    Livello _livello;
    bool _verbose;
    bool _ignoraVal;
    bool _aspettaFineNotifica;

    /**@}*/

    /**\name Comunicazione
    @{*/
    Comunicazione _monitor;

    /**@}*/

    /**\name LED
    @{*/

    Led _led;
    bool _abilitaIsr;
    struct Durate {
        Durate(int i, int a, int e, int f):
        informazione(i), avviso(a), errore(e), erroreFatale(f) {};
        int informazione;
        int avviso;
        int errore;
        int erroreFatale;
    } _durataLuce;

    /**@}*/

};



#ifndef DEBUG_DISABILITA

//dichiara l'esistenza di un'istanza della classe (dichiarata in `Debug_1_base.cpp`)
extern Debug debug;

#endif


//------------------------ funzioni template -----------------------------------




#ifndef DEBUG_DISABILITA



//################################ MESSAGGIO #################################//

template <typename Nome, typename Val>
void Debug::messaggio(Debug::Livello tipoMess, Nome nome, Val val, bool haVal) {

    if(haVal) val += 0; //se `val` non è un numero o un bool non compila

    //non stampare ad es. un'info se il livello è err
    if(_livello > tipoMess) return;


    //accende il led in modo che si spenga dopo il tempo corrispondente a tipoMess
    unsigned long tempo;
    switch (tipoMess) {
        case informazione: tempo = _durataLuce.informazione; break;
        case avviso: tempo = _durataLuce.avviso; break;
        case errore: tempo = _durataLuce.errore; break;
        default: break; //non può succedere
    }
    _led.accendi(tempo);


    //se la comunicazione è disabilitata la funzione termina qui
    if(!_monitor.abilitato()) return;


    //stampa informazioni dettagliate
    if(_verbose) {

        //gli errori saranno isolati dal resto dell'output da una riga vuota
        if(tipoMess == errore) _monitor.print('\n');

        //stampa il tempo
        _monitor.print(millis());
        _monitor.print('\t');

        //segnala il tipo di messaggio
        switch(tipoMess) {
            case informazione: _monitor.print("info"); break;
            case avviso: _monitor.print("warn"); break;
            case errore: _monitor.print("err"); break;
            default: break; //non può succedere
        }
        _monitor.print('\t');

        //stampa il nome o il codice del messaggio (un testo o un int)
        _monitor.print(nome);

        //ev. stampa il valore associato al messaggio
        if (haVal) {
            _monitor.print(':');
            _monitor.print(val);
        }

        //vai a capo e lascia una riga vuota
        _monitor.print('\n');
        //gli errori saranno isolati dal resto dell'output da una riga vuota
        if(tipoMess == errore) _monitor.print('\n');

    }//if(_verbose)

    //stampa informazioni minime
    else {

        switch(tipoMess) {
            case informazione: break;
            case avviso: _monitor.print("w"); break;
            case errore: _monitor.print("e"); break;
            default: break; //non può succedere
        }
        _monitor.print('\t');

        //stampa il nome o il codice del messaggio (un testo o un int)
        _monitor.print(nome);

        //ev. stampa il valore associato al messaggio
        if (haVal && !_ignoraVal) {
            _monitor.print(':');
            _monitor.print(val);
        }

        _monitor.print("\n");        //vai a capo
    }

    if(_aspettaFineNotifica) {
        _led.aspettaSpegnimento();
        delay(50);
    }

}

#endif //#ifndef DEBUG_DISABILITA


//versione vuota della funzione `messaggio`: le funzioni qui sotto chiameranno
// qesta se la classe è disabilitata
#ifdef DEBUG_DISABILITA

template <typename Nome, typename Val>
void Debug::messaggio(Debug::Livello tipoMess, Nome nome, Val val, bool haVal) {}

#endif





#ifndef DEBUG_DISABILITA

//############################## ASSEGNA VALORE ##############################//

template<typename T>
void Debug::assegnaValore(const String& nome, T* pointer) {

    //salva l'ora dell'interruzione del programma
    unsigned long tempoInterruzione = millis();

    //accende il led per un tempo indefinito
    _led.accendi();

    //separa con una riga vuota
    _monitor.print("\n");
    //stampa il tempo
    _monitor.print(tempoInterruzione);
    _monitor.print('\t');
    //scrivi che è un'assegnazione di valore
    _monitor.print("assegn\n");


    //esegui l'assegnazioone vera e propria
    // Assegna assegna(_monitor.ottieniOggettoComunicazione());
    assegna(pointer);

    //sfine dell'interruzione del programma
    _monitor.print("\n");
    _monitor.print(millis());
    _monitor.print('\t');
    _monitor.print("fine assegn\n\n");
    _led.spegni();
};


#endif //#ifndef DEBUG_ABILITA (*)

#endif //#ifndef Debug_hpp


/* (*)
Le funzioni assegnaValore se sono chiamate sono indispensabili per il programma
(il valore della variabile che dovrebbero modificare potrebbe essere assurdo prima
della chiamata). Quindi se si disabilita la classe il compilatore deve impedirne
la chiamata; il modo più semplice per fare questo è eliminarle del tutto. Per
questo non ne esiste una verisone vuota. */
