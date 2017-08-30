/** \file

\brief Implementazione della classe `Debug`
\date 16, 28 agosto 2017

*/

#include "Debug.hpp"

#ifndef DEBUG_DISABILITA




//############################### CONSTRUCTOR ################################//

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

//--------------------------------- ISTANZA ----------------------------------//
Debug debug(Serial);
//----------------------------------------------------------------------------//





//################################# SETTERS ##################################//

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



//################################### ISR ####################################//


ISR(TIMER0_COMPA_vect) {
    debug.controllaLed();
}







//################################ ERR_FAT ###################################//


void Debug::errFat() {

    //stampa l'ora a cui si è boccato il programma
    _monitor.print(millis());

    //segnala un errore fatale
    _monitor.print("\terr fatale\n");

    //stampa una riga di trattini
    for (int i = 0; i < 25; i++)
    _monitor.print('-');
    _monitor.print('\n');

    //loop infinito, nessuna uscita possibile.
    while (true) {
        _led.cambiaStato();
        delay(_durataLuce.erroreFatale);
    }

}




//############################### BREAKPOINT #################################//


void Debug::breakpoint() {
    breakpoint(0);
}

void Debug::breakpoint(unsigned long attesaMassima) {

    //salva l'ora dell'interruzione del programma
    unsigned long tempoInterruzione = millis();

    //"impostazione privata" di questa funzione: nr. di puntini per rappresentare
    // il tempo di attesaMassima che scade. Serve anche per far lampeggiare il LED
    int nrPuntini = 15;

    //i breakpoint funzionano solo se il livello è `debug`
    if(_livello > debug) return;

    //accende il led per un tempo indefinito
    _led.accendi();

    //separa con una riga vuota
    _monitor.print("\n");
    //stampa il tempo
    _monitor.print(tempoInterruzione);
    _monitor.print('\t');
    //scrivi che è un breakpoint
    _monitor.print("break\n");
    //stampa una riga di punti (.....)
    for(int i = 0; i < nrPuntini; i++) {
        _monitor.print('.');
    }


    //-------------------------- 2a parte: attesa ------------------------------

    //numero di puntini che rappresentano lo scorrere del tempo `attesaMassima`
    //già disegnati.
    int nrPuntiniDisegnati = 0;

    //va a capo se deve stampare i puntini che segnano il passare del tempo
    if (attesaMassima) {
        _monitor.print("\n");
    }

    //aspetta che l'utente o lo scadere del tempo massimo permettano di continuare
    while(true) {

        //se l'utente manda un carattere qualsiasi esci dal loop
        if(_monitor.available()) {
            while(_monitor.available()) {
                _monitor.read();
            }
            break;// da while(true)
        }

        //se è impostata un'attesa massima stampa i puntini che rappresentano
        //lo scorrere del tempo e qualdo scade esci dal loop
        if(attesaMassima) {

            //se è passato 1/nrPuntini del tempo disegna un nuovo puntino e fai lampeggiare il led
            if(tempoInterruzione + nrPuntiniDisegnati * (attesaMassima / nrPuntini) < millis()) {

                _monitor.print('\'');
                nrPuntiniDisegnati++;

                _led.cambiaStato();
            }

            if(tempoInterruzione + attesaMassima <= millis()) {
                break;
            }
        }

    }

    //scrivi l'ora della fine della pausa
    _monitor.print("\n");
    _monitor.print(millis());
    _monitor.print('\t');
    _monitor.print("fine break\n\n");

    _led.spegni();
}



#endif
