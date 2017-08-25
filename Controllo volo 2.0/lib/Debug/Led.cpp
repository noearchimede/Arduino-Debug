/**\file
\brief implementazione della classe `Led`
\date 12 agosto 2017
*/


#include "Led.hpp"

Led::Led() : _init(false), _abilita(true)
{
}

/**/
void Led::begin(int pin) {
    cambiaPin(pin);
    if(!_abilita) return;
    pinMode(_pin, OUTPUT);

    //la classe è stata inizializzata
    _init = true;
}

void Led::cambiaPin(int pin) {
    _pin = pin;
    _bitMaskPin = digitalPinToBitMask(_pin);
    _regPin = portOutputRegister(digitalPinToPort(_pin));
}
/*
Se disabilitata questa classe non modifica lo stato di nessun pin.
*/
void Led::abilita() {
    _abilita = true;
}
/**/
void Led::disabilita() {
    _abilita = false;
}
/**/
bool Led::abilitato() {
    if(!_init) return false;
    return _abilita;
}


/**
\param durata "Richiesta" di durata della luce. La funzione `controllaLed()`
leggerà questo valore e si occuperà di spegnere il LED dopo lo scadere del tempo
qui determinato.

*/
void Led::accendi(int durataLuce) {
    if(!_init) return;
    if(!_abilita) return;
    digitalWrite(_pin, HIGH);
    _acceso = true;
    _tempoSpegnimento = millis() + durataLuce;
    _spegniAutomaticamente = true;
}

/**
Accendi "per sempre", cioè senza definire un tempo di spegnimento
*/
void Led::accendi() {
    if(!_init) return;
    if(!_abilita) return;
    digitalWrite(_pin, HIGH);
    _acceso = true;
    _spegniAutomaticamente = false;
}

/**/
void Led::spegni() {
    if(!_init) return;
    if(!_abilita) return;
    *_regPin &= ~_bitMaskPin;
    _acceso = false;
}

/**/
void Led::cambiaStato() {
    if(_acceso) spegni();
    else accendi();
}


/**
Questa funzione è fondamentale per l'utilizzo del LED da parte della classe Debug.
Le funzioni di quest'ultima classe possono accendere la spia luminosa ma non spegnerla; questa
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
void Led::controlla() {
    if(!_init) return;
    if(!_acceso) return;//il più probabile fra i primi tre `if`
    if(!_abilita) return;

    if(_tempoSpegnimento < millis()) {
        *_regPin &= ~_bitMaskPin;
        _acceso = false;
    }
}



/**
Il tempo di attesa supplementare dopo lo spegnimento del LED è necessario per vedere
lo "stacco" fra due segnali luminosi se due notifiche sono molto vicine traloro.
*/
void Led::aspettaSpegnimento() {
    if(!_init) return;
    if(!_abilita) return;
    while(_acceso) controlla();
}
