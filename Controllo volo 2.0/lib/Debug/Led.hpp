/** \file

\brief Header della classe `Led`, che controlla il LED associato alla classe Debug
\date 12 agosto 2017

La classe `Led` si occupa della gestione del LED associato alla classe Debug.
*/

#ifndef Debug_Led_hpp
#define Debug_Led_hpp

#include "Arduino.h"

class Led {

public:

    Led();

    ///Inizializza.
    void begin(int pin = 0xFF);

    ///cambia pin
    void cambiaPin(int pin);

    ///\brief consenti l'utilizzo del LED
    void abilita();
    ///cfr. `abilita()`
    void disabilita();
    ///il LED (o meglio questa classe) è abilitato o no?
    bool abilitato();


    ///accende il led e imposta correttametne le variabili associate ad esso
    void accendi(int durataLuce);
    void accendi();
    ///spegne il led e imposta correttametne le variabili associate ad esso
    void spegni();
    ///Se il led è acceso lo spegne e viceversa
    void cambiaStato();

    ///\brief Controlla il led; se è acceso ed è ora di spegnerlo lo spegne. La funzione è
    /// scritta per poter essere chiamata da un'ISR.
    void controlla();

    ///Blocca il programma fino a quando il LED si spegne.
    void aspettaSpegnimento();



private:

    ///\brief posto `false` dal costructor, diventa `true` dopo `begin()` e non
    ///cambia mai più. Se è `false` la classe non toccherà nessun pin
    bool _init;

    ///se disabilitato questa classe non modifica lo stato di nessun pin
    bool _abilita;
    ///Pin del led
    int _pin;


    //### variabili di stato ###

    ///Stato attuale del led
    bool _acceso;
    ///se false il led dovrà essere spento manualmente
    bool _spegniAutomaticamente;
    ///Ora a cui dovrà essere spento il led. Non ha senso se `_acceso == false`
    unsigned long _tempoSpegnimento;

    ///\brief Bit corrispondente al pin nella porta definiita sopra
    ///\details cfr. implementazione di `digitalWrite(uint8_t, uint8_t)` di Arduino
    uint8_t _bitMaskPin;
    ///\brief pointer al regstro di output per una porta
    ///\details cfr. implementazione di `digitalWrite(uint8_t, uint8_t)` di Arduino
    volatile uint8_t *_regPin;

};



#endif
