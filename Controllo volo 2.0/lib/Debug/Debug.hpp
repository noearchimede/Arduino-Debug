/**
\file

\brief Header della classe `Debug`
\date 5 - 17 luglio 2017, 12 - 16 agosto 2017
*/

/**
\class Debug
\brief Classe utile per il debug di un prgramma qualsiasi.

Informazioni generali
---------------------

\note Nel file header di questa classe viene creata un'istanza (`extern`) disponibile
globalmente. Non è necessario crearne altre istanze, e non ha senso farlo;  il
comportamento di due istanze di questa classe in uno stesso programma non è definito.
*/


#ifndef Debug_hpp
#define Debug_hpp

#include "Arduino.h"
#include "Led.hpp"
#include "Comunicazione.hpp"

class Debug {

public:

    //tipi
    enum Livello {
        debug,
        inform,
        avviso,
        errore,
        niente
    };


    Debug(HardwareSerial&);

    /**\name Gestione classe @{*/

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


    /**\name Log @{*/

    /**
    Le funzioni `info`, `warn` e `err` stampano dei messaggi sul monitor seriale
    e fanno accendere un led per un tempo che dipende dal tipo di messaggio.
    L'unica differenza tra di esse è il livello di importanza: i messaggi info
    possono essere disabilitati lasciando warn ed err, e warn lasciando err.
    Inoltre i messaggi err sono visivamente in rilievo nell'output.

    Ogni messaggi ha un nome o un codice. Il nome è una stringa di testo, il codice
    è un numero che rappresenta univocamente il messaggio. Conviene usare il
    nome se si ha abbastanza memoria a disposizione e altrimenti il codie, che
    evidentemente rende più impegnativo leggere il file di log.
    Inoltre a ogni messaggio è possibile associare un valore di qualsiasi tipo
    aritmetico (bool, intero con o senza segno o decimale), che sarà stampato
    accanto al nome.

    Scelta delle funzioni:
    - `info(...)`: informazione sul corretto svolgimento del programma, es.:
        svolto un certo calcolo, nuovo sensore connesso, ...
    - `warn(...)`: avviso su un potenziale problema che non dovrebbe accadere
        ma non compromette irreversibilmente lo svolgimento del programma, es.:
        tentativo di connessione a un sensore già connesso, temperatura: -400°C
    - `err(...)`: errore, cioè avvenimento indesiderato che compromette, o
        potrebbe verosimilmente compromettere il programma, es.: myFloat è nan,
        divisione per 0

    */
    void info(int codice);
    template <typename T> void info(int codice, T val);
    void info(String& nome);
    template <typename T> void info(String& nome, T val);

    /**vedi sopra*/
    void warn(int codice);
    template <typename T> void warn(int codice, T val);
    void warn(String& nome);
    template <typename T> void warn(String& nome, T val);

    /**vedi sopra*/
    void err(int codice);
    template <typename T> void err(int codice, T val);
    void err(String& nome);
    template <typename T> void err(String& nome, T val);

    /**@}*/


    /**\name Debug @{*/

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
    void assegnaValore(String& nome, T* pointer);


    /**@}*/


    /**\name Setters e getters @{*/

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

    /**\name Funzioni private @{*/

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

    /**\name Messaggi @{*/
    Livello _livello;
    bool _verbose;
    bool _ignoraVal;
    bool _aspettaFineNotifica;

    /**@}*/

    /**\name Comunicazione @{*/
    Comunicazione _monitor;

    /**@}*/

    /**\name LED @{*/

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

//dichiara l'esistenza di un'istanza della classe (dichiarata in `Debug_1_base.cpp`)
extern Debug debug;

#endif
