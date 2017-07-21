/**
\file

\date 19 luglio 2017

\brief Test della classe Debug.

Questo programma dovrebbe essere un test completo della classe Debug.


Hardware necessario
-------------------
*  pin 13 - LED
*  interfaccia seriale - computer

*/

#include <Arduino.h>


#include "Debug.hpp"


void setup () {

    debug.begin(115200, 13);

}


void loop () {

    debug.controllaLed();

}

void provaFunzioni(int x) {
    switch (x) {
        case 1:
        debug.messaggio(1);
        break;
        case 2:
        debug.messaggio(2,0);
        break;
        case 3:
        dbug.messaggio(3, 6);
        
    }
}
