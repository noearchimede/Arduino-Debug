/**
\file

\date 19 luglio 2017

\brief Esempio di utilizzo della classe Debug.

Questo programma cerca di riprodurre quello che potrebbe essere un utput reslistico
della classe Debug. Utile per controllare eventuali modifiche al file Debug_string.hpp.



Hardware necessario
-------------------
*  pin 13 - LED
*  interfaccia seriale - computer

*/

#include <Arduino.h>


#include "Debug.hpp"

//------------------------------------------------------------------------------
//Liste di #define

//messaggi
#define MESS_1      1
#define MESS_2      2

//errori
#define ERR_1       1

//errori fatali
#define ERRFAT_1    1

//breakpoints
#define BREAK_1     1

//assegnazioni
#define ASSEGN_1   1
#define ASSEGN_2   2
#define ASSEGN_3   3
//------------------------------------------------------------------------------



void setup () {
    //La classe usa Serial, ma non lo inizializza al suo interno (HardwareSerial::begin
    // non funziona nel constructor, è chiamato troppo presto). Quindi è necessario
    // chimare una volta all'inizio del programma questa funzione.
    debug.inizializza(115200, 13);

}


void loop () {

    // Questa funzione controlla il LED.
    // ATTENZIONE! Deve essere chiamata regolarmente e il più frequentemente possibile!
    //debug.controllaLed();



    debug.messaggio(1,2);
    //genera tanti messaggi casuali per dare un'idea realistica dell'output
    int x, n, c;
    for(int i = 0; i < 50; i++) {

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
