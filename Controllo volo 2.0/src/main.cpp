/**
\file

\date 17 luglio 2017

\brief Esempio di utilizzo della classe Debug.

Questo programma contiene il minimo necessario per poter usare correttamente la
classe. Oltre a quelle presenti qui, la classe diverse altre funzioni secondarie
(ad esempio quelle per modifcarne le  impostazioni).



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
    pinMode(13, OUTPUT);
    //La classe usa Serial, ma non lo inizializza al suo interno (HardwareSerial::begin
    // non funziona nel constructor, è chiamato troppo presto). Quindi è necessario
    // chimare una volta all'inizio del programma questa funzione.
    debug.begin(9600);

}


void loop () {

    // Questa funzione controlla il LED.
    // ATTENZIONE! Deve essere chiamata regolarmente e il più frequentemente possibile!
    //debug.controllaLed();
    debug.print("noepezzolisonoio!!!");debug.flush();
    debug.print("o");debug.flush();
    debug.print("e");debug.flush();


    debug.messaggio(1,2);
    //genera 100 messaggi casuali per dare un'idea realistica dell'output
    int x, n, c;


    //un errore fatale (senza "codice")
    //il programma resterà per sempre bloccato nella funzione seguente
    //debug.erroreFatale(ERRFAT_1);
}

/*
for(int i = 0; i < 100; i++) {

    x = 1;//micros()%10;
    n = 3;//(micros()%100)/10;
    c = 0;//(micros()%1000)/100;

    if(x < 5)
    //un messaggio con codice
    debug.messaggio(n, c);

    else if(x < 8)
    //un messaggio senza codice
    debug.messaggio(n);

    else if(x < 9)
    //un errore (senza "codice" (codice 0 = senza codice))
    debug.errore(n, 0);

    else
    debug.errore(n, c);


}*/
