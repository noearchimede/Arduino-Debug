#ifndef comunicazione_hpp
#define comunicazione_hpp

#include "Arduino.h"

class Comunicazione {

public:

    Comunicazione(HardwareSerial& hwserial);

    void abilita(bool);
    void impostaVelocita(long);
    long ottieniVelocita();

    //0xFF è un valore non valido per `config`. Lascio la scelta del default a Serial.
    void begin(long, byte = 0xFF);
    void end();

    int available();
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

    HardwareSerial& _hwserial;

    long _baud;
    bool _attivo;

};



#endif
