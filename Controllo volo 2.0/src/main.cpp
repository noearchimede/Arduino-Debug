/**
\file

\date 5 - 9 luglio 2017

\brief Esempio di utilizzo della classe Debug.

Questo programma contiene il minimo necessario per poter usare correttamente la
classe. Oltre a quelle presenti qui, la classe diverse altre funzioni secondarie
(ad esempio quelle per modifcarne le  impostazioni).

Questo programma, compilato con la libreria Debug allo stato del 9 luglio 2017
e caricato su un Arduino UNO, ha funzionato.


Hardware necessario
-------------------
*  pin 13 - LED
*  interfaccia seriale - computer

*/

#include <Arduino.h>


#include <Debug.hpp>

//------------------------------------------------------------------------------
//Liste di #define per i messaggio, gli errori e gli errori fatali.

//messaggi
#define MESS_ESEMPIO_1  1
#define MESS_ESEMPIO_2  2

//errori
#define ERR_ESEMPIO     1

//errori fatali
#define ERRFAT_ESEMPIO  1
//------------------------------------------------------------------------------






void setup () {

    // La classe usa Serial, ma non lo inizializza al suo interno. Quindi è necessario
    // chimare una volta all'inizio del programma questa funzione. Ovviamente
    // prima di essa non potranno essere inviate notifiche (questo comunque non
    // causerebbe nessun errore, semplicemente non si vedrebbero sul monitor seriale).
    Serial.begin(115200);

}



void loop () {

    // Questa funzione controlla il LED.
    // ATTENZIONE! Deve essere chiamata regolarmente e il più frequentemente possibile!
    debug.controllaLed();



    //--------------------------------------------------------------------------
    // Funzioni di notifica, da usare in qualsiasi punto del programma

    //un messaggio (con "codice") (1)
    debug.messaggio(MESS_ESEMPIO_2, 4);

    //un errore (senza "codice" (codice 0 = senza codice))
    debug.errore(ERR_ESEMPIO, 0);

    //un errore fatale (senza "codice")
    //il programma resterà per sempre bloccato nella funzione seguente
    debug.erroreFatale(ERRFAT_ESEMPIO);

    //--------------------------------------------------------------------------

}
