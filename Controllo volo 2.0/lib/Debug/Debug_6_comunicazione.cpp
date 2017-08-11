
#include "Debug.hpp"
#include "Arduino.h"


void Debug::serialBegin(long baud, byte config){
    if(_usaHardwareSerial) {
        if(config == 0xFF)
        _hardwareSerial.begin(baud);
        else
        _hardwareSerial.begin(config);
    }
    _baudComunicazioneSeriale = baud;
}
void Debug::serialEnd(){
    if(_usaHardwareSerial)
    _hardwareSerial.end();
}

void Debug::print(const char val[]){
    if(_usaHardwareSerial)
    _hardwareSerial.print(val);
}
void Debug::print(const String& val){
    if(_usaHardwareSerial)
    _hardwareSerial.print(val);
}
void Debug::print(uint8_t val){
    if(_usaHardwareSerial)
    _hardwareSerial.print(val);
}
void Debug::print(int8_t val){
    if(_usaHardwareSerial)
    _hardwareSerial.print(val);
}
void Debug::print(uint16_t val){
    if(_usaHardwareSerial)
    _hardwareSerial.print(val);
}
void Debug::print(int16_t val){
    if(_usaHardwareSerial)
    _hardwareSerial.print(val);
}
void Debug::print(uint32_t val){
    if(_usaHardwareSerial)
    _hardwareSerial.print(val);
}
void Debug::print(int32_t val){
    if(_usaHardwareSerial)
    _hardwareSerial.print(val);
}
void Debug::print(float val){
    if(_usaHardwareSerial)
    _hardwareSerial.print(val);
}


int Debug::available(){
    if(_usaHardwareSerial)
    return _hardwareSerial.available();
    else return 0;
}
int Debug::read(){
    if(_usaHardwareSerial)
    return _hardwareSerial.read();
    else return 0;
}
