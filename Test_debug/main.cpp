/**
\file

\date 19 luglio, 7 ottobre 2017

\brief Test della classe Debug.

Questo programma è un test (ragionevolmente) completo della classe Debug.


Hardware necessario
-------------------
*  pin 13 - LED
*  [pin 12 - LED (per il test del cambiamento di pin)] -> non indispensabile
*  interfaccia seriale - computer

*/

#define TEST_SERIAL
#define TEST_LED

#include <Arduino.h>


#define DEBUG_DIASABILITA
#include "Debug.hpp"

//Esiste già un'istanza di Debug, chiamata debug

void setup () {

    //debug.begin(115200);
    const int pinLed = 13;
    debug.begin(115200, pinLed);


    #ifdef TEST_SERIAL

    debug.impostaLivello(Debug::debug);

    debug.lineeVuote(2);
    debug.info("test Serial");
    debug.lineeVuote(6);

    for(int i = 1; i <= 16; i++) {

        switch(i) {
            case 1:
            debug.info("default");
            debug.lineeVuote(2);
            break;
            case 2:
            debug.lineeVuote(4);
            debug.info("abilitaComunicazione: false");
            debug.abilitaComunicazione(false);
            debug.lineeVuote(2);
            break;
            case 3: case 9:
            debug.abilitaComunicazione(true);
            debug.lineeVuote(3);
            debug.info("livello: debug");
            debug.impostaLivello(Debug::debug);
            debug.lineeVuote(2);
            break;
            case 4: case 10:
            debug.lineeVuote(3);
            debug.info("livello: informazione");
            debug.impostaLivello(Debug::informazione);
            debug.lineeVuote(2);
            break;
            case 5: case 11:
            debug.lineeVuote(3);
            debug.info("livello: avviso");
            debug.impostaLivello(Debug::avviso);
            debug.lineeVuote(2);
            break;
            case 6: case 12:
            debug.lineeVuote(3);
            debug.impostaLivello(Debug::informazione);
            debug.info("livello: errore");
            debug.impostaLivello(Debug::errore);
            debug.lineeVuote(2);
            break;
            case 7: case 13:
            debug.lineeVuote(3);
            debug.impostaLivello(Debug::informazione);
            debug.info("livello: niente");
            debug.impostaLivello(Debug::niente);
            debug.lineeVuote(2);
            break;
            case 8:
            debug.lineeVuote(6);
            debug.impostaLivello(Debug::informazione);
            debug.info("NON VERBOSE\n");
            debug.verbose(false);
            debug.lineeVuote(2);
            continue;
            break;
            case 14:
            debug.lineeVuote(4);
            debug.impostaLivello(Debug::debug);
            debug.info("ignora val (liv.: debug)");
            debug.ignoraVal(true);
            debug.lineeVuote(2);
            break;
            case 15:
            debug.lineeVuote(4);
            debug.verbose(true);
            debug.info("aspetta fine notifica (liv.: debug)");
            debug.aspettaFineNotica(true);
            debug.lineeVuote(2);
            break;
            case 16:
            debug.lineeVuote(4);
            debug.aspettaFineNotica(false);
            debug.info("di nuovo default");
            debug.lineeVuote(2);
            break;

        }

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

        //(per un test completo delle funzioni "assegna" vedi il programma specifico)
        float x;
        debug.assegnaValore<float>("x", &x);

    }


    #endif

    #ifdef TEST_LED


    debug.impostaLivello(Debug::debug);

    debug.lineeVuote(16);
    debug.info("test LED");
    debug.lineeVuote(8);

    for(int i = 0; i <= 6;i++) {

        switch(i) {
            case 0:
            debug.lineeVuote(4);
            debug.info("default");
            debug.lineeVuote(2);
            break;
            case 1:
            debug.lineeVuote(4);
            debug.info("mod. durata luce info");
            debug.impostaDurataLuce(3000, false, false, false);
            debug.lineeVuote(2);
            break;
            case 2:
            debug.lineeVuote(4);
            debug.impostaDurataLuce(50, false, false, false);
            debug.info("mod. tutte durate");
            debug.impostaDurataLuce(1000, 2000, 3000, false);
            debug.lineeVuote(2);
            break;
            case 3:
            debug.lineeVuote(4);
            debug.info("aspetta fine notifica");
            debug.aspettaFineNotica(true);
            debug.lineeVuote(2);
            break;
            case 4:
            debug.lineeVuote(4);
            debug.aspettaFineNotica(false);
            debug.info("pin: 12");
            debug.impostaPinLed(12);
            debug.lineeVuote(2);
            break;
            case 5:
            debug.lineeVuote(4);
            debug.impostaPinLed(pinLed);
            debug.info("no interrupt");
            debug.abilitaInterrupt(false);
            debug.lineeVuote(2);
            break;
            case 6:
            debug.lineeVuote(4);
            debug.abilitaInterrupt(true);
            debug.info("interrupt");
            debug.aspettaFineNotica(true);
            debug.abilitaInterrupt(true);
            debug.lineeVuote(2);
            break;
        }

        debug.info("info");
        delay(1000);
        debug.warn("warn");
        delay(1000);
        debug.err("err");
        delay(1000);
        debug.breakpoint(2000);
    }

    #endif


    debug.impostaLivello(Debug::debug);
    debug.info("test ErrFat");

    debug.errFat();

}



void loop () {}
