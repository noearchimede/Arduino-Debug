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

#include <Arduino.h>


//#define DEBUG_DIASABILITA
#include "Debug.hpp"



//Esiste già un'istanza di Debug, chiamata debug

void setup () {

    //debug.begin(115200);
    const int pinLed = 13;
    debug.begin(115200, pinLed);


    debug.impostaLivello(Debug::debug);
    debug.info("\n\n\n\n\ntest Serial\n\n\n");


    for(int i = 1; i < 16; i++) {

        switch(i) {
            case 1:
            debug.info("default");
            break;
            case 2:
            debug.info("\n\nabilitaComunicazione: false");
            debug.abilitaComunicazione(false);
            break;
            case 3: case 9:
            debug.abilitaLed(true)
            debug.info("\n\nlivello: debug");
            debug.impostaLivello(Debug::debug);
            break;
            case 4: case 10:
            debug.info("\n\nlivello: informazione");
            debug.impostaLivello(Debug::informazione);
            break;
            case 5: case 11:
            debug.info("\n\nlivello: avviso");
            debug.impostaLivello(Debug::avviso);
            break;
            case 6: case 12:
            debug.impostaLivello(Debug::informazione);
            debug.info("\n\nlivello: errore");
            debug.impostaLivello(Debug::errore);
            break;
            case 7: case 13:
            debug.impostaLivello(Debug::informazione);
            debug.info("\n\nlivello: niente");
            debug.impostaLivello(Debug::niente);
            break;
            case 8:
            debug.impostaLivello(Debug::informazione);
            debug.info("\n\n\n\nNON VERBOSE\n");
            debug.verbose(false);
            break;
            case 14:
            debug.impostaLivello(Debug::debug);
            debug.info("\n\nignora val (liv.: debug)");
            debug.ignoraVal(true);
            break;
            case 15:
            debug.verbose(true);
            debug.info("\n\naspetta fine notifica (liv.: debug)");
            debug.aspettaFineNotica(true);
            break;
            case 16:
            debug.aspettaFineNotica(false);
            debug.info("\n\ndi nuovo default");
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





    debug.impostaLivello(Debug::debug);
    debug.info("\n\n\n\n\ntest LED\n\n\n");


    for(int i = 0; i < ;i++) {

        switch(i) {
            case 0:
            debug.info("default");
            break;
            case 1:
            debug.info("mod. durata luce info");
            debug.impostaDurataLuce(3000, false, false, false);
            break;
            case 2:
            debug.impostaDurataLuce(50, false, false, false);
            debug.info("mod. tutte durate");
            debug.impostaDurataLuce(1000, 2000, 3000, false);
            break;
            case 3:
            debug.info("aspetta fine notifica");
            debug.aspettaFineNotica(true);
            break;
            case 4:
            debug.aspettaFineNotica(false);
            debug.info("pin: 12");
            debug.impostaPinLed(12);
            break;
            case 5:
            debug.impostaPinLed(pinLed);
            debug.info("no interrupt");
            debug.abilitaInterrupt(false);
            break;
            case 6:
            debug.abilitaInterrupt(true);
            debug.info("interrupt");
            debug.abilitaInterrupt(true);
            break;
        }

        debug.info("info");
        delay(2000);
        debug.warn("warn");
        delay(2000);
        debug.err("err");
        delay(2000);
        debug.breakpoint(3000);
        delay(2000);
    }




    debug.impostaLivello(Debug::debug);
    debug.info("\n\n\n\n\ntest ErrFat\n\n\n");

    debug.errFat();

}



void loop () {}
