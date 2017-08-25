/** \file

\brief Implementazione della classe `Debug`
\date 16 agosto 2017

*/

#include "Debug.hpp"

#ifndef DEBUG_DISABILITA


Debug::Debug(HardwareSerial& hwserial) :
//messaggi
_livello(debug),
_verbose(true),
_ignoraVal(false),
_aspettaFineNotifica(false),
//comunicazione
_monitor(hwserial),
//led
_abilitaIsr(true),
_durataLuce(50,100,300,1000)
{
}


//------------------
Debug debug(Serial);
//------------------


void Debug::begin(long baudSerial) {
    _led.disabilita();
    //il pin 13 non sarà modificato
    begin(baudSerial, 13);
}

void Debug::begin(long baudSerial, int pinLed) {

    //prepara il LED
    _led.begin(pinLed);
    //prepara Serial
    _monitor.begin(baudSerial);

    //prepara l'ISR
    if (_abilitaIsr) {
        abilitaInterrupt(true);
    }
}



void Debug::controllaLed() {
    _led.controlla();
}



//------------------------ Setters ----------------------------


void Debug::abilitaComunicazione(bool x) {
    if(x) {
        _monitor.abilita();
        _monitor.begin();
    }
    else {
        _monitor.end();
        _monitor.disabilita();
    }

}
void Debug::abilitaLed(bool x) {
    if(x) {
        _led.abilita();
        if(_abilitaIsr) abilitaInterrupt(true);
    }
    else {
        _led.disabilita();
        abilitaInterrupt(false);
    }
}


void Debug::impostaLivello(Debug::Livello x) {
    _livello = x;
}

void Debug::verbose(bool x) {
    _verbose = x;
    //ignoraVal ha senso solo se l'output non è verbose
    if(_verbose) _ignoraVal = false;
}

void Debug::ignoraVal(bool x) {
    _ignoraVal = x;
    //verbose deve essere disattivato per ignorare il valore
    //associato ai messaggi
    if(_ignoraVal) _verbose = false;
}

void Debug::aspettaFineNotica(bool x) {
    _aspettaFineNotifica = x;
}


void Debug::impostaPinLed(byte x) {
    _led.cambiaPin(x);
}

void Debug::abilitaInterrupt(bool x) {

    _abilitaIsr = x;

    uint8_t prevSREG = SREG;
    cli();
    //abilita
    if(_abilitaIsr) {
        TIMSK0 |= (1 << OCIE0A);
    }
    //disabilita
    else {
        TIMSK0 &= (0 << OCIE0A);
    }

    SREG = prevSREG;
}

void Debug::impostaDurataLuce(unsigned long informazione, unsigned long avviso, unsigned long errore, unsigned long erroreFatale) {
    if(informazione) _durataLuce.informazione = informazione;
    if(avviso) _durataLuce.avviso = avviso;
    if(errore) _durataLuce.errore = errore;
    if(erroreFatale) _durataLuce.erroreFatale = erroreFatale;
}

void Debug::impostaBaudComunicazione(long x) {
    _monitor.impostaVelocita(x);
}



//-------------------------- ISR -------------------------------


ISR(TIMER0_COMPA_vect) {
    debug.controllaLed();
}


#endif
