/**
\file

\brief Funzioni secondarie

Il file contiene le funzioni meno importanti e usate più raramente, e in
particolare quelle per la modifica (o la consultazione) delle impostazioni

\date 5 - 17 luglio 2017

*/

#include "Debug.hpp"



/**
\param usaSerial la nuova impostazione
*/
void Debug::impostaUsaSerial(bool usaSerial) {
    _usaSerial = usaSerial;
}


void Debug::impostaStampaMessaggi(bool stampaMessaggi) {
    _stampaMessaggi = stampaMessaggi;
}


void Debug::impostaStampaMinimo(bool stampaMinimo) {
    _stampaMinimo = stampaMinimo;
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

void Debug::impostaDurataBuioDopoNotifica(int durata) {
    _durataBuioDopoNotifica = durata;
}


/**
\return `true` se sta usando la comunicazione seriale, `false` altrimenti
*/
bool Debug::staUsandoSerial() {
    return _usaSerial;
}
