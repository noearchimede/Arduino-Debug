/** \file

\brief Header della classe `Comunicazione`, una classe di supporto per Debug
\date 12 agosto 2017

*/

/**
\class Comunicazione
\brief Interfaccia sovrapposta a Serial di Arduino, con funzioni in parte diverse

La classe `Comunicazione` si occupa di tutta la comunicazione tra la classe
Debug e l'utente, esclusi i segnali luminosi del LED.

La presentza di un'interfaccia che fa da tramite tra la classe Debug e Serial
ha due vantaggi:

- permette di cambiare facilmente il tipo di comunicazione (ad es. da Serial a
    un sistema di comunicazione radio)
- permette di adattare le funzioni di Serial alle esigenze di Debug (ad es. si
    può facilmente cambiare il modo di visualizzazione dei valori bool da 0/1 a
    true/false)

*/
#ifndef Debug_Comunicazione_hpp
#define Debug_Comunicazione_hpp

#include "Arduino.h"

class Comunicazione {

public:

    Comunicazione(HardwareSerial&);

    ///ottieni una reference all'oggetto HardwareSerial usato dalla libreria
    HardwareSerial& ottieniOggettoComunicazione () {return _hwserial;}


    ///inizializza la comunicazione seriale.
    /**
    \param baud velocità della comunicazione
    \param config configurazione di `HardwareSerial`
    \note 0xFF è un valore non valido per `config`. Lascio la scelta del default a Serial.
    */
    void begin(long, byte = 0xFF);
    ///come `begin(long, byte = 0xFF);`; usa il valore di `_baud` come velocità
    void begin();
    ///termina la comunicazione seriale
    void end();

    ///abilita la comunicazione
    /**
    Se la comunicazione è disabilitata le funzioni di comunicazioni
    termineranno appena chiamate.
    */
    void abilita();
    ///cfr. `abilita()`
    void disabilita();
    ///è abilitato o no?
    bool abilitato();


    ///imposta la velocità della comunicazione seriale
    void impostaVelocita(long);
    ///restituisce la velocità della comunicazione seriale attuale
    long ottieniVelocita();


    ///nuovi dati disponibili?
    int available();
    ///preleva un carattere dal buffer di caratteri in arrivo
    int read();

    ///stampa l'argomento passato alla funzione
    void print(bool);
    void print(uint8_t, int = 10);
    void print(uint16_t, int = 10);
    void print(uint32_t, int = 10);
    void print(int8_t, int = 10);
    void print(int16_t, int = 10);
    void print(int32_t, int = 10);
    void print(double, int = 2);

    void print(char);
    void print(const char[]);
    void print(const __FlashStringHelper *);


private:

    ///Stato del sistema di comunicazione
    /*
    Se la comunicazione è disabilitata le funzioni di comunicazione escono
    subito grazie a un singolo if
    */
    bool _abilita;

    ///collegamento a un'istanza di HardwareSerial (tipicamente `Serial`)
    HardwareSerial& _hwserial;

    ///velocità attuale della comunicazione seriale
    long _baud;

};


#endif
