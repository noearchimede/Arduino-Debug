/**
\file

\brief Header della classe `Debug`
\date 5 - 9 luglio 2017
*/

/**
\class Debug
\brief Classe utile per il debug di un prgramma qualsiasi.

Informazioni generali
---------------------

Questa classe permette di stampare sul monitor seriale tre tipi di notifica:
- messaggi
- errori
- errori fatali (vedi sotto)
Inoltre permette di inserire nel programma dei _breakpoints_ a cui il programma si
arresta e, dopo averlo segnalati, attende il comando dell'utente per continuare.

Le notifiche non sono rappresentate da stringhe di testo (che occuperebbero
troppa memoria su ATmega328), ma da numeri. Occorre quindi creare un file di
`#define` per associare ad ogni errore o messaggio un numero.
Inoltre per ogni notifica inviata sul monitor seriale si accende un LED, che
rimane accesso per un tempo differente a seconda del tipo di notifica.

I messaggi e gli errori sono molto simili tra loro, differiscono solo per la
visibilità sia sul monitor seriale sia tramite il LED (che si accende più a
lungo per segnalare glie errori).
Gli errori fatali invece sono gestiti in modo molto diverso: quando il programma
chiama la funzione `erroreFatale` di questa classe si blocca per sempre nel loop
contenuto in essa.

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

/// \name Impostazioni di default della classe `Debug`
/// @{

///Pin del LED
#define DEBUG_DEFAULT_PIN_LED                   13
///Attiva la comunicazione seriale (`true`)?
#define DEBUG_DEFAULT_USA_SERIAL                true
///Abilita i breakpoints?
#define DEBUG_DEFAULT_CONSENTI_BREAKPOINT       true
///Stampa anche i messaggi (`true`) o solo gli errori?
#define DEBUG_DEFAULT_STAMPA_MESS               true
///Stampa solo il minimo indispensabille (`true`) o produci un output più leggibile?
#define DEBUG_DEFAULT_STAMPA_MINIMO             false
///\brief Ignora il codice eventualmente associato alle notifiche
///\warning ha effetto solo se anche `DEBUG_DEFAULT_STAMPA_MINIMO` è `true`
#define DEBUG_DEFAULT_IGNORA_CODICE             false
///Dopo ogni notifica aspetta che il led sia spento prima di proseguire
#define DEBUG_DEFAULT_ASPETTA_FINE_NOTIFICA     false

///Tempo per cui il LED resta acceso per ogni messaggio (in millisecondi)
#define DEBUG_DEFAULT_LUCE_MESS                 50
///Tempo per cui il LED resta acceso per ogni errore (in millisecondi)
#define DEBUG_DEFAULT_LUCE_ERR                  300
///Durata della luce (e del "buio") nel lampeggiamento in caso di errore fatale
#define DEBUG_DEFAULT_LUCE_ERRFAT               1500
///Se `DEBUG_ASPETTA_FINE_NOTIFICA`è `true`, questo è il tempo minimo tra due segnali LED
#define DEBUG_DEFAULT_DURATA_BUIO_DOPO_NOTIFICA 30

///Tempo di attesa prima che il monitor seriale sia resettato in caso di errore fatale
#define DEBUG_DEFAULT_TEMPO_RESET_SERIAL        5000
///\brief Velocità della comunicazione seriale con il computer a partire da
///`DEBUG_DEFAULT_TEMPO_RESET_SERIAL` ms dopo un errore fatale, in Baud
#define DEBUG_DEFAULT_BAUD_SERIAL               115200

/// @}


class Debug: public HardwareSerial {

  public:

    ///Assegna tutti i valori di default
    Debug (void);

    /// \name Funzioni principali
    /// @{

    ///Stampa un messaggio
    void messaggio(int, long = 0, bool = false);
    ///Stampa un errore
    void errore(int, long = 0, bool = false);
    ///Blocca il brogramma e segnala un errore fatale
    void erroreFatale(int, long = 0);

    ///inserisci un breakpoint
    void breakpoint(int, long = 0, unsigned long = 0);

    ///Controlla il led; se è acceso ed è ora di spegnerlo lo spegne
    void controllaLed();

    /// @}


    /// \name Funzioni di modifica impostazioni
    /// @{

    ///Cambia l'impostazione `usaSerial`
    void impostaUsaSerial(bool);
    ///Cambia l'impostazione `stampaMessaggi`
    void impostaStampaMessaggi(bool);
    ///Cambia l'impostazione `stampaMinimo`
    void impostaStampaMinimo(bool);
    ///Cambia i tempi di accensione del led per i vari tipi di notifica
    void impostaDurateLuci(int, int, int); //parametri nell'ordine: mess-err-errFat
    ///Cambia il tempo minimo di attesa tra due segnali luminosi se DEBUG_ASPETTA_FINE_NOTIFICA
    void impostaDurataBuioDopoNotifica(int);
    ///Chiedi alla classe se sta usando la comunicazione seriale
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


    //###### VARIABILI ######

    /// \name Controllo del LED
    /// @{

    ///Pin del led
    int _pinLed;
    ///Durata della luce per i messaggi
    int _durataLuceMessaggio;
    ///Durata della luce per gli errori
    int _durataLuceErrore;
    ///Durata del lempeggiamento per gli errori fatali
    int _durataLuceErroreFatale;
    ///Attesa (opzionale) dopo lo spegnimento del LED
    int _durataBuioDopoNotifica;

    ///Stato attuale del led
    bool _ledAcceso;
    ///Ora di accensione del led. Non ha senso se `_ledAcceso == false`
    unsigned long _tempoAccensioneLed;
    ///Tempo per cui deve stare attualmente acceso il led. Non ha senso se `_ledAcceso == false`
    int _durataLuceLed;

    /// @}


    /// \name Controllo di Serial
    /// @{


    ///La classe può usare la porta seriale?
    bool _usaSerial;
    ///Velocità della comunicazione seriale in caso di errore fatale (in Baud)
    long _baudComunicazioneSeriale;
    ///Stampa anche i messaggi (true) solo gli errori (false)?
    bool _stampaMessaggi;
    ///Stampa solo il minimo indispensabille (true)
    bool _stampaMinimo;
    ///Ignora il codice associato ai messaggi. Ha effetto solo se `_stampaMinimo == true`
    bool _ignoraCodice;

    /// @}


    /// \name Altre impostazioni
    /// @{

    ///Dopo ogni notifica blocca il programma fino a quando il LED si spegne
    bool _aspettaFineNotifica;
    ///Consenti alla funzione `breakpoint`di interrompere il programma?
    bool _consentiBreakpoint;


    /// @}

};

//crea un'istanza della classe, esattamente come il framework Arduino fa per Serial
extern Debug debug;


#endif
