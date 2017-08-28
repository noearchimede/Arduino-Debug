

#include "Comunicazione.hpp"

Comunicazione::Comunicazione(HardwareSerial& hwserial) :
_abilita(true),
_hwserial(hwserial),
_baud(9600)
{
}

void Comunicazione::begin(long baud, byte config) {
    _baud = baud;
    if(_abilita) {
        if(config == 0xFF)
        _hwserial.begin(baud);
        else
        _hwserial.begin(baud, config);
    }

}
void Comunicazione::begin() {
    begin(_baud);
}

void Comunicazione::end() {
    if(_abilita)
    _hwserial.end();
}



void Comunicazione::abilita() {
    _abilita = true;
}
void Comunicazione::disabilita() {
    _abilita = false;
}
bool Comunicazione::abilitato() {
    return _abilita;
}

void Comunicazione::impostaVelocita(long baud) {
    _baud = baud;
}
long Comunicazione::ottieniVelocita() {
    return _baud;
}



int Comunicazione::available() {
    if(_abilita)
    return _hwserial.available();
    else return 0;
}


int Comunicazione::read() {
    if(_abilita)
    return _hwserial.read();
    else return 0;
}


void Comunicazione::print(bool val) {
    if(!_abilita) return;
    if(val == true) _hwserial.print("true");
    if(val == false) _hwserial.print("false");
}

void Comunicazione::print(uint8_t val, int base) {
    if(_abilita) {
        //stampa prefisso base per i numeri esadecimali e binari
        switch(base) {
            case 16: _hwserial.print("0x"); break;
            case 2: _hwserial.print("B");  break;
        }
        _hwserial.print(val, base);
    }
}
void Comunicazione::print(uint16_t val, int base) {
    if(_abilita) {
        //stampa prefisso base per i numeri esadecimali e binari
        switch(base) {
            case 16: _hwserial.print("0x"); break;
            case 2: _hwserial.print("B");  break;
        }
        _hwserial.print(val, base);
    }
}
void Comunicazione::print(uint32_t val, int base) {
    if(_abilita) {
        //stampa prefisso base per i numeri esadecimali e binari
        switch(base) {
            case 16: _hwserial.print("0x"); break;
            case 2: _hwserial.print("B");  break;
        }
        _hwserial.print(val, base);
    }
}
void Comunicazione::print(int8_t val, int base) {
    if(_abilita) {
        //stampa prefisso base per i numeri esadecimali e binari
        switch(base) {
            case 16: _hwserial.print("0x"); break;
            case 2: _hwserial.print("B");  break;
        }
        _hwserial.print(val, base);
    }
}
void Comunicazione::print(int16_t val, int base) {
    if(_abilita) {
        //stampa prefisso base per i numeri esadecimali e binari
        switch(base) {
            case 16: _hwserial.print("0x"); break;
            case 2: _hwserial.print("B");  break;
        }
        _hwserial.print(val, base);
    }
}
void Comunicazione::print(int32_t val, int base) {
    if(_abilita) {
        //stampa prefisso base per i numeri esadecimali e binari
        switch(base) {
            case 16: _hwserial.print("0x"); break;
            case 2: _hwserial.print("B");  break;
        }
        _hwserial.print(val, base);
    }
}

void Comunicazione::print(double val, int decimali) {
    if(_abilita)
    _hwserial.print(val, decimali);
}

void Comunicazione::print(char val) {
    if(_abilita)
    _hwserial.print(val);
}
void Comunicazione::print(const char val[]) {
    if(_abilita)
    _hwserial.print(val);
}
void Comunicazione::print(const String& val) {
    if(_abilita)
    _hwserial.print(val);
}

