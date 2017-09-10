/**
\file

\date 19 luglio 2017

\brief Esempio dell'output della classe Debug.

Questo programma cerca di riprodurre quello che potrebbe essere un utput reslistico
della classe Debug. Utile per controllare eventuali modifiche al file Debug_string.hpp.



Hardware necessario
-------------------
*  pin 13 - LED
*  interfaccia seriale - computer

*/

#include <Arduino.h>


#include "Debug.hpp"


void setup () {
    debug.inizializza(115200, 13);
}


void loop () {

    debug.messaggio(1,2);

    int x, n, c;
    for(int i = 0; i < 100; i++) {

        debug.controllaLed();

        x = micros()%10;
        n = (micros()%100)/10;
        c = (micros()%1000)/100;

        switch (x) {
            case 0:
            case 1:
            case 2:
            case 3:
            //un messaggio con codice
            debug.messaggio(n, c);
            break;
            case 4:
            case 5:
            //un messaggio senza codice
            debug.messaggio(n);
            break;
            case 6:
            //un errore (senza "codice" (codice 0 = senza codice))
            debug.errore(n, 0);
            break;
            case 7:
            debug.errore(n, c);
            break;
            case 8:
            debug.breakpoint(n);
            break;
            case 9:
            int v;
            debug.assegnaValore(&v, n);
            debug.messaggio(0,v);
            break;
            default:
            debug.erroreFatale(ERRFAT_1);
        }


        //un errore fatale (senza "codice")
        //il programma resterà per sempre bloccato nella funzione seguente

    }



}
