

#include "Comunicazione.hpp"

Comunicazione::Comunicazione(HardwareSerial& hwserial) :
_hwserial(hwserial),
_attivo(false)
{}


void Comunicazione::abilita(bool attivo) {
    _attivo = attivo;
}
void Comunicazione::impostaVelocita(long baud) {
    _baud = baud;
}
long Comunicazione::ottieniVelocita() {
    return _baud;
}


void Comunicazione::begin(long baud, byte config){
    if(_attivo) {
        if(config == 0xFF)
        _hwserial.begin(baud);
        else
        _hwserial.begin(config);
    }
    _baud = baud;
}
void Comunicazione::end(){
    if(_attivo)
    _hwserial.end();
    _attivo = false;
}




int Comunicazione::available(){
    if(_attivo)
    return _hwserial.available();
    else return 0;
}


int Comunicazione::read(){
    if(_attivo)
    return _hwserial.read();
    else return 0;
}




void Comunicazione::print(const char val[]){
    if(_attivo)
    _hwserial.print(val);
}
void Comunicazione::print(const String& val){
    if(_attivo)
    _hwserial.print(val);
}
void Comunicazione::print(uint8_t val){
    if(_attivo)
    _hwserial.print(val);
}
void Comunicazione::print(int8_t val){
    if(_attivo)
    _hwserial.print(val);
}
void Comunicazione::print(uint16_t val){
    if(_attivo)
    _hwserial.print(val);
}
void Comunicazione::print(int16_t val){
    if(_attivo)
    _hwserial.print(val);
}
void Comunicazione::print(uint32_t val){
    if(_attivo)
    _hwserial.print(val);
}
void Comunicazione::print(int32_t val){
    if(_attivo)
    _hwserial.print(val);
}
void Comunicazione::print(float val){
    if(_attivo)
    _hwserial.print(val);
}
