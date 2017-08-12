/**
\file

\brief Funzioni private e constructor

Il constructor non è privato, ma in teoria non dovrebbe mai essere usato nel resto
del programma (l'unica istanza che ha senso usare è quella creata in ´Debug.hpp´).

\date 5 - 17 luglio 2017
*/

#include "Debug.hpp"



#ifdef DEBUG_ABILITA



void Debug::usaSerial(bool x) {
    if(x == true) {
        _monitor.begin(_baudComunicazioneSeriale);
    }
    else {
        _monitor.end();
    }
    _usaHardwareSerial = x;
}
void Debug::usaLed(bool x) {
    _usaLed = x;
    if(_usaInterrupt) {
        if(x) abilitaInterrupt(true);
        else abilitaInterrupt(false);
    }
}
void Debug::impostaPinLed(int x) {
    _pinLed = x;
    _bitMaskPinLed = digitalPinToBitMask(_pinLed);
    _regPinLed = portOutputRegister(digitalPinToPort(_pinLed));
}
void Debug::usaInterrupt(bool x) {
    abilitaInterrupt(x);
}
void Debug::consentiBreakpoint(bool x) {
    _consentiBreakpoint = x;
}
void Debug::usaSempreAttesaMassimaBreak(bool x) {
    _usaSempreAttesaMassimaBreak = x;
}
void Debug::impostaAttesaMassimaBreak(unsigned int val) {
    _attesaMassimaBreakpoint = val;
}
void Debug::stampaMessaggi(bool x) {
    _stampaMessaggi = x;
}
void Debug::stampaMinimo(bool x) {
    _stampaMinimo = x;
}
void Debug::ignoraCodice(bool x) {
    _ignoraCodice = x;
}
void Debug::impostaAspettaFineNotifiche(bool x) {
    _aspettaFineNotifica = x;
}
void Debug::impostaDurataBuioDopoNotifica(int durata) {
    _durataBuioDopoNotifica = durata;
}
/**
\param mess durata della luce per i messaggi
\param err durata della luce per gli errori
\param errFat durata della luce nel lampeggiamento per gli errori fatali

Per lasciare invariato un valore, passare un numero negativo come argomento
corrispondente a quel valore; ad es.:
```cpp
impostaDurateLuci(-1, 350, -1);
```
modifica solo la durata della luce per gli errori.
*/
void Debug::impostaDurateLuci(int mess, int err, int errFat) {
    if (mess >= 0)
    _durataLuceMessaggio = mess;
    if (err >= 0)
    _durataLuceErrore = err;
    if (errFat >= 0)
    _durataLuceErroreFatale = errFat;
}
/**
\return `true` se sta usando la comunicazione seriale, `false` altrimenti
*/
bool Debug::staUsandoSerial() {
    return _usaHardwareSerial;
}




#endif //#ifdef DEBUG_ABILITA
