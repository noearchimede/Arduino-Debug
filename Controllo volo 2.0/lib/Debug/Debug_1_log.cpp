/** \file

\brief Implementazione delle funzioni di comunicazione della classe `Debug`
\date 18 agosto 2017

*/

#include "Debug.hpp"

#ifndef DEBUG_DISABILITA


template <typename Nome, typename Val>
void Debug::messaggio(Debug::Livello tipoMess, Nome nome, Val val, bool haVal) {


    //non stampare ad es. un'info se il livello è err
    if(_livello > tipoMess) return;


    //accende il led in modo che si spenga dopo il tempo corrispondente a tipoMess
    unsigned long tempo;
    switch (tipoMess) {
        case inform: tempo = _durataLuce.informazione; break;
        case avviso: tempo = _durataLuce.avviso; break;
        case errore: tempo = _durataLuce.errore; break;
        case niente: break; //non può succedere
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
            case inform: _monitor.print("info"); break;
            case avviso: _monitor.print("warn"); break;
            case errore: _monitor.print("err"); break;
            case niente: break; //non può succedere
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
            case inform: break;
            case avviso: _monitor.print("w"); break;
            case errore: _monitor.print("e"); break;
            case niente: break; //non può succedere
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




//################################### INFO ###################################//
void Debug::info(int codice) {
    messaggio(inform, codice, 0, false);
}
template <typename T>
void Debug::info(int codice, T val) {
    val++; //se `val` non è un numero o un bool non compila
    messaggio(inform, codice, val, true);
}
void Debug::info(String& nome) {
    messaggio(inform, nome, 0, false);
}
template <typename T>
void Debug::info(String& nome, T val) {
    val++; //se `val` non è un numero o un bool non compila
    messaggio(inform, nome, val, true);
}

//################################### WARN ###################################//

void Debug::warn(int codice) {
    messaggio(avviso, codice, 0, false);
}
template <typename T>
void Debug::warn(int codice, T val) {
    val++; //se `val` non è un numero o un bool non compila
    messaggio(avviso, codice, val, true);
}
void Debug::warn(String& nome) {
    messaggio(avviso, nome, 0, false);
}
template <typename T>
void Debug::warn(String& nome, T val) {
    val++; //se `val` non è un numero o un bool non compila
    messaggio(avviso, nome, val, true);
}

//################################### ERR ####################################//

void Debug::err(int codice) {
    messaggio(errore, codice, 0, false);
}
template <typename T>
void Debug::err(int codice, T val) {
    val++; //se `val` non è un numero o un bool non compila
    messaggio(errore, codice, val, true);
}
void Debug::err(String& nome) {
    messaggio(errore, nome, 0, false);
}
template <typename T>
void Debug::err(String& nome, T val) {
    val++; //se `val` non è un numero o un bool non compila
    messaggio(errore, nome, val, true);
}


#endif
