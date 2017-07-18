/**
\file

\brief Funzioni private e constructor

Il constructor non è privato, ma in teoria non dovrebbe mai essere usato nel resto
del programma (l'unica istanza che ha senso usare è quella creata in ´Debug.hpp´).

\date 5 - 17 luglio 2017
*/

#include "Debug.hpp"
#include "Debug_impostazioni.hpp"


///Il constructor di HardwareSerial si trova in questo file
#include "HardwareSerial_private.h"

/**
Assegna tutti i valori di default a questa classe e passa gli indirizzi dei registri
alla classe HardwareSerial. I nomi dei registri "arrivano" tramite `Arduino.h`.
I nomi possono essere definiti con o senza `0` (ad es. `UBRRH` o `UBRR0H`), quindi
prevedo le due possibilità (ho copiato questo procedimento da `HardwareSerial0.cpp`,
linee 67 - 71).

*/

Debug::Debug() :

//chiama il costructor della classe ereditata (HardwareSerial)
#if defined(UBRRH) && defined(UBRRL)
HardwareSerial(&UBRRH, &UBRRL, &UCSRA, &UCSRB, &UCSRC, &UDR),
#else
HardwareSerial(&UBRR0H, &UBRR0L, &UCSR0A, &UCSR0B, &UCSR0C, &UDR0),
#endif

//assegna i valori alle costanti
_usaLed                     (DEBUG_USA_LED),
_pinLed                     (DEBUG_PIN_LED),
_tempoResetSerial           (DEBUG_TEMPO_RESET_SERIAL),

//assegna i valori default alle impostazioni
_consentiBreakpoint         (DEBUG_DEFAULT_CONSENTI_BREAKPOINT),
_usaSempreAttesaMassimaBreak(DEBUG_DEFAULTA_USA_TIMEOUT_BREAK),
_attesaMassimaBreakpoint    (DEBUG_DEFAULT_TIMEOUT_BREKPOINT),
_stampaMessaggi             (DEBUG_DEFAULT_STAMPA_MESS),
_stampaMinimo               (DEBUG_DEFAULT_STAMPA_MINIMO),
_ignoraCodice               (DEBUG_DEFAULT_IGNORA_CODICE),
_aspettaFineNotifica        (DEBUG_DEFAULT_ASPETTA_FINE_NOTIFICA),
_durataBuioDopoNotifica     (DEBUG_DEFAULT_DURATA_BUIO_DOPO_NOTIFICA),
_durataLuceMessaggio        (DEBUG_DEFAULT_LUCE_MESS),
_durataLuceErrore           (DEBUG_DEFAULT_LUCE_ERR),
_durataLuceErroreFatale     (DEBUG_DEFAULT_LUCE_ERRFAT),
_usaSerial                  (true)
{

    //prepara il LED
    if(DEBUG_USA_LED) {
        pinMode(_pinLed, OUTPUT);
    }

}



#ifdef DEBUG_ABILITA


/**
\param durata "Richiesta" di durata della luce. La funzione `controllaLed()`
leggerà questo valore e si occuperà di spegnere il LED dopo lo scadere del tempo
qui determinato.

\note `accendiLed(0)` accende il led per un tempo indeterminato, cioè fino a una
chiamata "manuale" (e non da parte di `controllaLed()`) di `spegniLed()`
*/
void Debug::accendiLed(int durata) {

    if(DEBUG_USA_LED) {
        digitalWrite(_pinLed, HIGH);
    }
    _ledAcceso = true;
    _tempoAccensioneLed = millis();
    _durataLuceLed = durata;

}

/**
cfr. commento per `accendiLed(int durata)`
*/
void Debug::spegniLed() {

    if(DEBUG_USA_LED) {
        digitalWrite(_pinLed, LOW);
    }
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





//######## modifica impostazioni ########


void Debug::usaSerial(bool x) {
    if(x == true) {
        super::begin(_baudComunicazioneSeriale);
    }
    else {
        super::end();
    }
    _usaSerial = x;
}
void Debug::consentiBreakpoint(bool x) {
    _consentiBreakpoint = x;
}
void Debug::usaSempreAttesaMassimaBreak(bool x) {
    _usaSempreAttesaMassimaBreak = x;
}
void Debug::impostaAttesaMassimaBreak(unsigned int val) {
    _attesaMassimaBreakpoint = val;
}
void Debug::stampaMessaggi(bool x) {
    _stampaMessaggi = x;
}
void Debug::stampaMinimo(bool x) {
    _stampaMinimo = x;
}
void Debug::ignoraCodice(bool x) {
    _ignoraCodice = x;
}
void Debug::impostaAspettaFineNotifiche(bool x) {
    _aspettaFineNotifica = x;
}
void Debug::impostaDurataBuioDopoNotifica(int durata) {
    _durataBuioDopoNotifica = durata;
}
/**
\param mess durata della luce per i messaggi
\param err durata della luce per gli errori
\param errFat durata della luce nel lampeggiamento per gli errori fatali

Per lasciare invariato un valore, passare un numero negativo come argomento
corrispondente a quel valore; ad es.:
```cpp
impostaDurateLuci(-1, 350, -1);
```
modifica solo la durata della luce per gli errori.
*/
void Debug::impostaDurateLuci(int mess, int err, int errFat) {
    if (mess >= 0)
    _durataLuceMessaggio = mess;
    if (err >= 0)
    _durataLuceErrore = err;
    if (errFat >= 0)
    _durataLuceErroreFatale = errFat;
}
/**
\return `true` se sta usando la comunicazione seriale, `false` altrimenti
*/
bool Debug::staUsandoSerial() {
    return _usaSerial;
}




#endif //#ifdef DEBUG_ABILITA
