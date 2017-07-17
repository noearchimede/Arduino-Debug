/**
\file

\brief Funzioni in grado di bloccare il programma

Il file contiene le funzioni capaci di bloccare il programma in esecuzione, e
cioè `erroreFatale` (blocco definitivo) e `breakpoint` (sospensione temporanea)

\date 9 e 14 luglio 2017

*/

#include "Debug.hpp"


/**
Questa funzione segnala un errore fatale e blocca il programma.
La segnalazione avviente, come sempre, in due modi: tramite il LED e tramite il
monitor seriale.
-   Il LED lampeggia incessantemente, restando acceso per `_durataLuceErroreFatale`
ms e spento per lo stesso tempo (i tempi non sono precisi).
-   Il monitor seriale manda a ogni lampeggiamento un messaggio di errore con il
tempo attuale, il tempo a cui si è verificato l'errore e il numero/ev. codice
dell'errore.

Dopo `DEBUG_TEMPO_RESET_SERIAL` ms il programma inizia a reinizializzare il
monitor seriale prima di ogni messaggio di errore. In questo modo anche se si
connette il computer al robot solo dopo che si è verificato l'errore si può
sapere che errore c'è stato.

I numeri che rappresentano gli errori fatali sono distinti sia da quelli dei messaggi
sia da quelli degli errori, perciò avranno una lista di `#define` indipendete.
Per più informazioni sul significato di `numero` e `codice` cfr. la documentazione
della funzione `messaggio(int, long, bool)`.

\param numero Il numero che rappresenta l'errore
\param codice [facoltativo] Il codice/numero/... associato a quell'errore
*/
void Debug::erroreFatale(int numero, long codice) {

    //salva l'ora a cui si è boccato il programma
    unsigned long tempoBlocco = millis();

    if(_usaSerial)
    super::print("\n");       //salta una riga (se sta usando serial)


    for (int i = 0; i < 20; i++)
    if(_usaSerial)
    super::print("-");      //stampa una riga di ----- (se sta usando serial)

    if(_usaSerial)
    super::print("\n");       //vai a capo (se sta usando serial)



    //loop infinito, nessuna uscita possibile.
    //a ogni loop:
    //  - a partire da DEBUG_TEMPO_RESET_SERIAL ms dopo l'erore, resetta la connessione
    //    seriale (così si avranno informazioni anche nel caso che il computer
    //    venga connesso solo dopo che l'errore si è verificato)
    //  - stampa un messaggio di errore
    //  - fa lampeggiare il led

    bool resetSerial = false;

    while (true) {

        Debug::accendiLed(_durataLuceErroreFatale);

        if (resetSerial)
        super::begin(_baudComunicazioneSeriale);

        //questa condizione sarà certamente vera a partire da quando il monitor
        // inizierà ad essere resettato
        if(_usaSerial) {

            super::print("Errore fatale a ");  //stampa "Errore fatale a 876578 ms:"
            super::print(tempoBlocco);
            super::print(" ms:\n");   //va a capo

            super::print(millis());   //stampa il tempo attuale
            super::print("\t");

            super::print("err. ");
            super::print(numero);     //stampa il nr. che rappresenta il messaggio

            if (codice) {
                super::print(":");
                super::print(codice);   //ev. stampa il codice
            }

            super::print("\n\n");     //va a capo e salta una riga

        }

        //aspetta circa il doppio della durata della luce (circa perché le operazioni
        // Serial prendono del tempo, quindi la luce durerà un po' più del buio)
        unsigned long t = millis();
        while (t + 2 * _durataLuceErroreFatale > millis())
        Debug::controllaLed();


        //se sono passati 15 secondi inizia a resettare il monitor a ogni lampeggiamento del LED
        if (tempoBlocco + DEBUG_DEFAULT_TEMPO_RESET_SERIAL < millis() && !resetSerial) {
            resetSerial = true;
            _usaSerial = true;
        }

    }

}

/**
Un breakpoint è un punto di interruzione del programma: quando il programma in
esecuzione arriva a un breakpoint si interrompe.
L'interruzine non è però definitiva come nel caso si un errore fatale. Il programma
può quindi riprendere, in due modi:
- input dell'utente tramite il monitor seriale
- (se definito) scadenza del tempo impostato come terzo parametro

I numeri che rappresentano i breakpoint sono distinti sia da quelli dei messaggi
sia da quelli degli errori (fatali e non), perciò avranno una lista di `#define`
indipendete.
Per più informazioni sul significato di `numero` e `codice` cfr. la documentazione
della funzione `messaggio(int, long, bool)`.

\param numero Il numero che rappresenta il messaggio
\param codice [opzionale] Il codice/numero/... associato a quel messaggio
\param attesaMassima [opzionale] Tempo in ms dopo il quale il programma riprende
automaticamente
*/
void Debug::breakpoint(int numero, long codice, unsigned long attesaMassima) {

    //esci dalla la funzione se i breakpoint sono disabilitati
    if (!_consentiBreakpoint) {
        return;
    }


    //"impostazione privata" di questa funzione: nr. di puntini per rappresentare
    // il tempo di attesaMassima che scade
    int __nrPuntini = 20;

    //salva l'ora dell'interruzione del programma
    unsigned long tempoInterruzione = millis();

    //accedne il led per un tempo indefinito
    Debug::accendiLed(0);

    if(_usaSerial) {

        super::print("\n"); //salta una riga

        //già che è definito sopra, usa `tempoInterruzione` invece di millis()
        super::print(tempoInterruzione);   //stampa il tempo
        super::print("\t");

        super::print("breakpoint ");   //scrivi che è un breakpoint
        super::print(numero);     //stampa il nr. che rappresenta il breakpoint

        if (codice) {
            super::print(":");
            super::print(codice);  //ev. stampa il codice
        }

        super::print("\n");      //vai a capo


        for(int i = 0; i < __nrPuntini; i++) {
            super::print(".");          //stampa una riga di punti (.....)
        }

    }


    //--------------------------------------------------------------------------

    //numero di puntini che rappresentano lo scorrere del tempo `attesaMassima`
    //già disegnati
    int __nrPuntiniDisegnati;

    //aspetta che l'utente o lo scadere del tempo massimo permettano di continuare
    while(true) {

        if(_usaSerial) {
            if(super::available() > 0) {
                break;

                //qui sarebbe possibile inserire condizioni sul carattere ricevuto
                //(ad esempio richiederne uno specifico):
                //char carattereRicevuto = Serial.read();
                // if (carattereRicevuto == <carattere/i accettato/i>) { break; }
            }
        }

        if(attesaMassima) {
            //se è passato 1/__nrPuntini del tempo disegna un nuovo puntino
            if(_usaSerial) {
                if(tempoInterruzione + __nrPuntiniDisegnati * (attesaMassima / __nrPuntini) <= millis()) {
                    super::print("'");
                }
            }

            if(tempoInterruzione + attesaMassima <= millis()) {
                break;
            }
        }
    }

    //scrivi l'ora della fine della pausa
    super::print(millis());
    super::print("\t");
    super::print("fine break");
    super::print("\n");


    Debug::spegniLed();
}
