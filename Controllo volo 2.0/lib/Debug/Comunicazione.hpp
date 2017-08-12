/** \file

\brief Header della classe `Comunicazione`, una classe di supporto per Debug
\date 12 agosto 2017

La classe `Comunicazione` si occupa di tutta la comunicazione tra la classe
Debug e l'utente, esclusi i segnali luminosi del LED.
È sostanzialmente un alias di HardwareSerial con qualche funzione aggiuntiva. Il
vantaggio di usare questa invece di HardwareSerial è che così è molto più facile
sostituire l'interfaccia seriale con un'altra o aggiungerne una nuova.

*/

#ifndef comunicazione_hpp
#define comunicazione_hpp

#include "Arduino.h"

class Comunicazione {

public:

    Comunicazione(HardwareSerial& hwserial);

    ///\brief abilita la comunicazione
    ///\details Se la comunicazione è disabilitata le funzioni di comunicazioni
    ///termineranno appena chiamate.
    void abilita();
    ///cfr. `abilita()`
    void disabilita();

    ///imposta la velocità della comunicazione seriale
    void impostaVelocita(long);
    ///restituisce la velocità della comunicazione seriale attuale
    long ottieniVelocita();

    ///\brief inizializza la comunicazione seriale.
    ///\param baud velocità della comunicazione
    ///\param config configurazione di `HardwareSerial`
    ///\note 0xFF è un valore non valido per `config`. Lascio la scelta del default a Serial.
    void begin(long, byte = 0xFF);
    ///come `begin(long, byte = 0xFF);`; usa il valore di `_baud` come velocità
    void begin();
    ///termina la comunicazione seriale
    void end();

    ///nuovi dati disponibili?
    int available();
    ///preleva un carattere dal buffer di caratteri in arrivo
    int read();

    void print(const char[]);
    void print(const String&);
    void print(uint8_t);
    void print(int8_t);
    void print(uint16_t);
    void print(int16_t);
    void print(uint32_t);
    void print(int32_t);
    void print(float);


private:

    ///collegamento a un'istanza di HardwareSerial (tipicamente `Serial`)
    HardwareSerial& _hwserial;

    ///velocità attuale della comunicazione seriale
    long _baud;
    ///Se la comunicazione è disabilitata le funzioni di comunicazione escono
    ///subito grazie a un singolo if
    bool _comunicazioneAbilitata;

};



#endif
