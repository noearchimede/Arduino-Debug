/**
\file

\brief Funzioni in grado di bloccare il programma

Il file contiene le funzioni capaci di bloccare il programma in esecuzione.

\date 14 luglio 2017, 19 agosto 2017

*/

#include "Debug.hpp"


#ifndef DEBUG_DISABILITA


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



void Debug::breakpoint() {
    breakpoint(0);
}

void Debug::breakpoint(unsigned long attesaMassima) {

    //"impostazione privata" di questa funzione: nr. di puntini per rappresentare
    // il tempo di attesaMassima che scade. Serve anche per far lampeggiare il LED
    int nrPuntini = 15;

    //i breakpoint funzionano solo se il livello è `debug`
    if(_livello > debug) return;

    //salva l'ora dell'interruzione del programma
    unsigned long tempoInterruzione = millis();

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
