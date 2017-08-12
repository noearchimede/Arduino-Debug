
#include "Comunicazione.hpp"

Comunicazione::Comunicazione(HardwareSerial& hwserial) :
_hwserial(hwserial),
_baud(9600),
_comunicazioneAbilitata(true)
{}


void Comunicazione::abilita() {
    _comunicazioneAbilitata = true;
}
void Comunicazione::disabilita() {
    _comunicazioneAbilitata = false;
}

void Comunicazione::impostaVelocita(long baud) {
    _baud = baud;
}
long Comunicazione::ottieniVelocita() {
    return _baud;
}


void Comunicazione::begin(long baud, byte config){
    _baud = baud;
    if(_comunicazioneAbilitata) {
        if(config == 0xFF)
        _hwserial.begin(baud);
        else
        _hwserial.begin(config);
    }

}
void Comunicazione::begin() {
    begin(_baud);
}

void Comunicazione::end(){
    if(_comunicazioneAbilitata)
    _hwserial.end();
}




int Comunicazione::available(){
    if(_comunicazioneAbilitata)
    return _hwserial.available();
    else return 0;
}


int Comunicazione::read(){
    if(_comunicazioneAbilitata)
    return _hwserial.read();
    else return 0;
}




void Comunicazione::print(const char val[]){
    if(_comunicazioneAbilitata)
    _hwserial.print(val);
}
void Comunicazione::print(const String& val){
    if(_comunicazioneAbilitata)
    _hwserial.print(val);
}
void Comunicazione::print(uint8_t val){
    if(_comunicazioneAbilitata)
    _hwserial.print(val);
}
void Comunicazione::print(int8_t val){
    if(_comunicazioneAbilitata)
    _hwserial.print(val);
}
void Comunicazione::print(uint16_t val){
    if(_comunicazioneAbilitata)
    _hwserial.print(val);
}
void Comunicazione::print(int16_t val){
    if(_comunicazioneAbilitata)
    _hwserial.print(val);
}
void Comunicazione::print(uint32_t val){
    if(_comunicazioneAbilitata)
    _hwserial.print(val);
}
void Comunicazione::print(int32_t val){
    if(_comunicazioneAbilitata)
    _hwserial.print(val);
}
void Comunicazione::print(float val){
    if(_comunicazioneAbilitata)
    _hwserial.print(val);
}
