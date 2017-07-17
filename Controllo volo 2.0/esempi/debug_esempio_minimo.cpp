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


#include "Debug.h"

//------------------------------------------------------------------------------
//Liste di #efine per i messaggio, gli errori e gli errori fatali.

//messaggi
#define MESS_ESEMPIO_1  1
#define MESS_ESEMPIO_2  2

//errori
#define ERR_ESEMPIO     1

//errori fatali
#define ERRFAT_ESEMPIO  1
//------------------------------------------------------------------------------


// Crea un'istanza della classe, collegandovi un'interfaccia seriale (l'unica
// disponibile, nel caso di ATmega328P) e un LED (in questo caso collegato al pin 13).
// Si presume che Serial e del LED siano a completa disposizione della classe, e
// cioè che non siano mai usati altrove nel programma.
// La classe stessa non impedisce che essi siano comunque usati, ma il comportamento
// in caso di "interferenze" non è definito.
Debug debug(&Serial, 13);



void setup () {

    // La classe usa Serial, ma non lo inizializza al suo interno. Quindi è necessario
    // chimare una volta all'inizio del programma questa funzione. ovviamente
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
