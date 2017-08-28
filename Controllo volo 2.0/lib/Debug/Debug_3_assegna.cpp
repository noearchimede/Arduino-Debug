/**
\file

\brief Funzioni "assegna valore"

Il file contiene le funzioni che permettono di assegnare un valore a una qualsiasi
variabile di tipo aritmetico.

\date 28 agosto 2017

*/

#include "Debug.hpp"
#include "Assegna.hpp"


template<typename T>
void Debug::assegnaValore(String& nome, T* pointer) {

    //salva l'ora dell'interruzione del programma
    unsigned long tempoInterruzione = millis();

    //accende il led per un tempo indefinito
    _led.accendi();

    //separa con una riga vuota
    _monitor.print("\n");
    //stampa il tempo
    _monitor.print(tempoInterruzione);
    _monitor.print('\t');
    //scrivi che è un'assegnazione di valore
    _monitor.print("assegn\n");


    //esegui l'assegnazioone vera e propria
    Assegna assegna(_monitor);
    assegna(pointer);

    //sfine dell'interruzione del programma
    _monitor.print("\n");
    _monitor.print(millis());
    _monitor.print('\t');
    _monitor.print("fine assegn\n\n");
    _led.spegni();
};
