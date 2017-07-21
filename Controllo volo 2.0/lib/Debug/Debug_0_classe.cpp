/** \file

Questo file contiene funzioni indispensabili al funzionamento della classe:
- il constructor
- la funzione di inizializzazione
- l'ISR che controlla il LED (chiamata su TIMER0_COMPA)


\date 20 luglio 2017
*/
#include "Debug_avvisi_compilatore.hpp"

#include "Debug.hpp"

/**constructor: assegna i valori di default, collega un'istanza di HardwareSerial
e abilita l'interrupt che controllerà il LED
*/
Debug::Debug (HardwareSerial& hwserial) :
//assegna i valori default alle impostazioni
_usaHardwareSerial          (DEBUG_DEFAULT_USA_SERIAL),
_usaLed                     (DEBUG_DEFAULT_USA_LED),
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
//collega HardwareSerial
_hardwareSerial             (hwserial)
{}


//crea un'istanza della classe
Debug debug(Serial);


//ISR della classe (per il controllo del LED)
#if defined DEBUG_ABILITA && defined DEBUG_ABILITA_INTERRUPT

 ISR(TIMER0_COMPA_vect) {
    debug.controllaLed();
}
//#error "ok!"


#endif


#ifdef DEBUG_ABILITA
/**
Questa funzione deve essere chiamata una volta all'inizio del programma.
Prima di essa si possono chiamare `usaSerial()` e `usaLed()` per disattivare una
di questefunzioni a livello globale. In ogni caso bisogna passare un valore per
entrambi i parametri, che sarà memorizzato in caso di una riattivazione successiva
della funzionalità disabilitata.

\param baud velocità della comunicazione seriale in baud
\param pinLed pin a cui è connesso il Led destinato al debug (numerazione dei pin
di Arduino). Ha un valore di default modificabile nelle impostazioni generali.
*/
void Debug::inizializza(long baud, byte pinLed)
{

    _pinLed = pinLed;
    _bitMaskPinLed = digitalPinToBitMask(_pinLed);
    _regPinLed = portOutputRegister(digitalPinToPort(_pinLed));

    _baudComunicazioneSeriale = baud;

    //prepara il LED
    if(_usaLed)  pinMode(_pinLed, OUTPUT);
    //prepara Serial
    if(_usaHardwareSerial) Debug::serialBegin(_baudComunicazioneSeriale);

    //prepara l'ISR
    #if defined DEBUG_ABILITA_INTERRUPT && defined DEBUG_ABILITA
    if (_usaLed) {
        Debug::abilitaInterrupt(true);
    }
    #endif
}




/**
Questa funzione è fondamentale per l'utilizzo del LED da parte della classe Debug.
Le altre funzioni possono accendere la spia luminosa ma non spegnerla; questa
funzione si occupa quindi di controllare se è "ora" di spegnere il LED e se è il
caso lo spegne.

\note Se si permette alla classe di usare l'interrupt TIMER0_COMPA per controllare
il LED questa funzione viene chiamata dall'ISR. Deve quindi esser eil più breve
possibile.

\note Questa funzione deve essere chiamata regolarmente e frequentemente per
poter avere dei segnali luminosi della giusta durata, altrimenti la luce resta
accesa per un tempo arbitrario che non dipende dal tempo richiesto dalla funzione
che l'ha accesa (ad es `messaggio()`) ma dalla posizione della chiamata di questa
funzione nel codice. L'ideale è consentire l'uso deell'ISR `ISR(TIMER0_COMPA_vect)`.
*/
void Debug::controllaLed() {
    if (_usaLed && _ledAcceso && _tempoSpegnimentoLed < millis()) {
        *_regPinLed &= ~_bitMaskPinLed;

        _ledAcceso = false;
    }
}



//########################### funzioni private #################################


/**
Abilita o disabilita l'interrupt su TIMERO0_COMPA, usato dalla classe per il
controllo del LED.

\param x true per abilitare, false per disabilitare.
*/
void Debug::abilitaInterrupt(bool x) {

    _usaInterrupt = x;

    uint8_t oldSREG = SREG;
    cli();
    //abilita
    if(_usaInterrupt) {
        TIMSK0 |= (1 << OCIE0A);
    }
    //disabilita
    else {
        TIMSK0 &= (0 << OCIE0A);
    }
    SREG = oldSREG;
}

/**
\param durata "Richiesta" di durata della luce. La funzione `controllaLed()`
leggerà questo valore e si occuperà di spegnere il LED dopo lo scadere del tempo
qui determinato.

\note `accendiLed(0)` accende il led per un tempo indeterminato, cioè fino a una
chiamata "manuale" (e non da parte di `controllaLed()`) di `spegniLed()`
*/
void Debug::accendiLed(int durata) {

    if(!_usaLed) return;

    digitalWrite(_pinLed, HIGH);
    _ledAcceso = true;
    _tempoSpegnimentoLed = millis() + durata;

}

/**
Questa funzione spegne il LED

\note La funzione eve essere il più breve possibile perché può essere
chiamata da un'ISR (a seconda delle impostazioni generali scelte dall'utente).

\warning Prima di chiamare questa funzione controllare che `_usaLed == true`,
altrimenti potrebbe essere spento un pin dedicato a qualcos'altro.
*/
void Debug::spegniLed() {

	*_regPinLed &= ~_bitMaskPinLed;

    _ledAcceso = false;

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




#endif //#ifdef DEBUG_ABILITA
