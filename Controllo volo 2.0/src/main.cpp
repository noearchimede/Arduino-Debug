/**
\file

\date 19 luglio 2017

\brief Test della classe Debug.

Questo programma è inteso come test completo della classe Debug.


Hardware necessario
-------------------
*  pin 13 - LED
*  interfaccia seriale - computer

*/

#include <Arduino.h>

#include "Debug.hpp"
//Esiste già un'istanza di Debug, chiamata debug

#define MESS debug.messaggio
#define ERR debug.errore


void setup () {
    debug.inizializza(115200, 13);
}

int x = 0;
void loop () {

/************************************************************************/x = 0;

    MESS(x);

    debug.breakpoint(x);/************************************************/x = 1;

    MESS(x, 1);
    MESS(x, 2);
    MESS(x, 3);
    MESS(x, 4);
    MESS(x, 5);

    debug.breakpoint(x);/************************************************/x = 2;

    MESS(x, 10, true);
    MESS(x, 20, true);
    MESS(x, 30, true);
    MESS(x, 40, true);
    MESS(x, 50, true);

    debug.breakpoint(x);/************************************************/x = 3;

    ERR(x);

    debug.breakpoint(x);/************************************************/x = 4;

    ERR(x, 1);
    ERR(x, 2);
    ERR(x, 3);
    ERR(x, 4);
    ERR(x, 5);

    debug.breakpoint(x);/************************************************/x = 5;

    ERR(x, 10, true);
    ERR(x, 20, true);
    ERR(x, 30, true);
    ERR(x, 40, true);
    ERR(x, 50, true);

    debug.breakpoint(x);/************************************************/x = 6;

    debug.breakpoint(x);
    debug.breakpoint(x, 1);
    debug.breakpoint(x, 2, 5000);
    debug.breakpoint(x, 3, 1);

    debug.breakpoint(x);/************************************************/x = 7;

    bool ba, bb;
    uint8_t u8; uint16_t u16; uint32_t u32;
    char i8; int16_t i16; int32_t i32;
    float fa, fb;

    debug.assegnaValore(&ba, x);
    debug.assegnaValore(&bb, x, 1);
    debug.assegnaValore(&u8, x, 2);
    debug.assegnaValore(&i8, x);
    debug.assegnaValore(&u16, x);
    debug.assegnaValore(&i16, x);
    debug.assegnaValore(&u32, x);
    debug.assegnaValore(&i32, x);
    debug.assegnaValore(&fa, x);
    debug.assegnaValore(&fb, x, 1);

    debug.breakpoint(x);/************************************************/x = 8;
    debug.messaggio(x, ba);
    debug.messaggio(x, ba);
    debug.breakpoint(x);/************************************************/x = 9;
    debug.messaggio(x, u8);
    debug.messaggio(x, u16);
    debug.messaggio(x, u32);
    delay(3000);/*******************************************************/x = 10;
    debug.messaggio(x, i8);
    debug.messaggio(x, i16);
    debug.messaggio(x, i32);
    delay(3000);/*******************************************************/x = 11;
    debug.messaggio(x, fa);
    debug.messaggio(x, fb);







}
