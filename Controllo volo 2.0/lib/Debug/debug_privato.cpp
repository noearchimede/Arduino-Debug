/**
\file

\brief Funzioni private e constructor

Il constructor non è privato, ma in teoria non dovrebbe mai essere usato nel resto
del programma (l'unica istanza che ha senso usare è quella creata in ´Debug.hpp´).

\date 5 - 17 luglio 2017
*/

#include "Debug.hpp"



///Il constructor di HardwareSerial si trova in questo file
#include "HardwareSerial_private.h"

//variabili da passare al costructor di HardwareSerial, del tutto inutili nel
// resto del file
static uint8_t ubrrh, ubrrl, ucsra, ucsrb, ucsrc, udr;

/**
Assegna tutti i valori di default e collega la classe super::

\warning `Serial` deve essere inizializzato nel codice (tipicamente in `void setup()`).
Per inizializzarlo non chiamare `Serial.begin(<baud>)`, ma `Debug::inizializzaSerial`
(`Serial.begin()` sarà poi chiamata all'interno di quella funzione).

\warning si deve passare alla funzione e inizializzare Serial anche se non si
intende usarlo, perché in caso di errore fatale sarà comunque attivato.

\param serial Un'istanza della classe HardwareSerial. Sarà presumibilmente sempre
`Serial` (`Debug(Serial, <pin>)`)

\param pinLed Pin del led a disposizione della classe (che dovrà detenere il monopolio
sul suo utilizzo). Normalmente è il pin 13, visto che sulla scheda Arduino UNO
c'è già un LED collegato ad essa.


*/
Debug::Debug() : HardwareSerial(&ubrrh, &ubrrl, &ucsra, &ucsrb, &ucsrc, &udr) {

    //assegna i valori di default
    _pinLed                     = DEBUG_DEFAULT_PIN_LED;
    _usaSerial                  = DEBUG_DEFAULT_USA_SERIAL;
    _baudComunicazioneSeriale   = DEBUG_DEFAULT_BAUD_SERIAL;
    _consentiBreakpoint         = DEBUG_DEFAULT_CONSENTI_BREAKPOINT;
    _stampaMessaggi             = DEBUG_DEFAULT_STAMPA_MESS;
    _stampaMinimo               = DEBUG_DEFAULT_STAMPA_MINIMO;
    _ignoraCodice               = DEBUG_DEFAULT_IGNORA_CODICE;
    _durataLuceMessaggio        = DEBUG_DEFAULT_LUCE_MESS;
    _durataLuceErrore           = DEBUG_DEFAULT_LUCE_ERR;
    _durataLuceErroreFatale     = DEBUG_DEFAULT_LUCE_ERRFAT;
    _durataBuioDopoNotifica     = DEBUG_DEFAULT_DURATA_BUIO_DOPO_NOTIFICA;
    _aspettaFineNotifica        = DEBUG_DEFAULT_ASPETTA_FINE_NOTIFICA;

    //prepara il LED
    pinMode(_pinLed, OUTPUT);

}


/**
\param durata "Richiesta" di durata della luce. La funzione `controllaLed()`
leggerà questo valore e si occuperà di spegnere il LED dopo lo scadere del tempo
qui determinato.

\note `accendiLed(0)` accende il led per un tempo indeterminato, cioè fino a una
chiamata "manuale" (e non da parte di `controllaLed()`) di `spegniLed()`
*/
void Debug::accendiLed(int durata) {
    digitalWrite(_pinLed, HIGH);
    _ledAcceso = true;
    _tempoAccensioneLed = millis();
    _durataLuceLed = durata;

}

/**
cfr. commento per `accendiLed(int durata)`
*/
void Debug::spegniLed() {
    digitalWrite(_pinLed, LOW);
    _ledAcceso = false;
    _durataLuceLed = 0;
}


/**
Il tempo di attesa supplementare dopo lo spegnimento del LED è necessario per vedere
lo "stacco" fra due segnali luminosi se due notifiche sono molto vicine traloro.
*/
void Debug::aspettaFineNotifica() {

    while(_ledAcceso)
    Debug::controllaLed();

    delay(_durataBuioDopoNotifica);
}
