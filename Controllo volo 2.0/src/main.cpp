/**
\file

\date 19 luglio 2017

\brief Test della classe Debug.

Questo programma è inteso come test ragionevolmente completo della classe Debug.


Hardware necessario
-------------------
*  pin 13 - LED
*  interfaccia seriale - computer

*/

#include <Arduino.h>

#include "Debug.hpp"
//Esiste già un'istanza di Debug, chiamata debug


void setup () {
    //debug.begin(115200);
    debug.begin(115200, 13);
}

void loop () {
    //test serial
    /*
    debug.info(1);
    debug.info<long>(2, -1234567);
    debug.info("tre");
    debug.info<char>("_4_", 'Q');

    debug.warn(5);
    debug.warn<String>(6,"sei");
    debug.warn("sette");
    debug.warn<uint32_t>("otto",56787);

    debug.err(9);
    debug.err<char>(10,12);
    debug.err("11)");
    debug.err<char>("12)",'r');

    debug.breakpoint();
    debug.breakpoint(5000);

    float x;
    debug.assegnaValore<float>("x", &x);
    */

    //test led
    debug.abilitaComunicazione(false);
    debug.impostaDurataLuce(30, 300, 1000, 2000);
    debug.info(1);
    delay(2000);
    debug.warn(2);
    delay(2000);
    debug.err(3);
    delay(2000);
    debug.breakpoint(3000);
    delay(2000);
    debug.errFat();


}
