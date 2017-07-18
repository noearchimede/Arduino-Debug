/**
\file

\brief Funzioni in grado di bloccare il programma

Il file contiene le funzioni capaci di bloccare il programma in esecuzione, e
cioè `erroreFatale` (blocco definitivo) e `breakpoint` (sospensione temporanea)

\date 9 e 14 luglio 2017

*/

#include "Debug.hpp"

#ifdef DEBUG_ABILITA

#include "Debug_string.hpp"


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


    #ifdef DEBUG_USA_SERIAL


    if(_usaSerial)
    super::print("\n");       //salta una riga (se sta usando serial)


    for (int i = 0; i < S_NR_PUNTI_LINEA_ERRFAT; i++)
    if(_usaSerial)
    super::print(S_CHAR_LINEA_ERRFAT);      //stampa una riga di ----- (se sta usando serial)

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

    #endif //#ifdef DEBUG_USA_SERIAL


    while (true) {

        Debug::accendiLed(_durataLuceErroreFatale);


        #ifdef DEBUG_USA_SERIAL

        if (resetSerial)
        super::begin(_baudComunicazioneSeriale);

        //questa condizione sarà certamente vera a partire da quando il monitor
        // inizierà ad essere resettato
        if(_usaSerial) {

            super::print(S_ERRFAT_PRIMA_DI_TEMPO);  //stampa "Errore fatale a 876578 ms:"
            super::print(tempoBlocco);
            super::print(S_ERRFAT_DOPO_TEMPO);   //va a capo

            super::print(millis());   //stampa il tempo attuale
            super::print(S_SEP_T_NR);

            super::print(S_ERRFAT);
            super::print(numero);     //stampa il nr. che rappresenta il messaggio

            if (codice) {
                super::print(S_SEP_NR_COD);
                super::print(codice);   //ev. stampa il codice
            }

            super::print("\n\n");     //va a capo e salta una riga

        }

        #endif//#ifdef DEBUG_USA_SERIAL

        //aspetta circa il doppio della durata della luce (circa perché le operazioni
        // Serial prendono del tempo, quindi la luce durerà un po' più del buio)
        unsigned long t = millis();
        while (t + 2 * _durataLuceErroreFatale > millis())
        Debug::controllaLed();

        #ifdef DEBUG_USA_SERIAL
        //se sono passati 15 secondi inizia a resettare il monitor a ogni lampeggiamento del LED
        if (tempoBlocco + _tempoResetSerial < millis() && !resetSerial) {
            resetSerial = true;
            _usaSerial = true;
        }

        #endif//#ifdef DEBUG_USA_SERIAL
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
automaticamente. Se è negativo (default) il programma riprende esclusivamente su
richiesta dell'utente, se vale 0 userà il tempo di timeout di default.
*/
void Debug::breakpoint(int numero, long codice, long attesaMassima) {

    #ifdef DEBUG_ABILITA_BREAKPOINT

    //esci dalla la funzione se i breakpoint sono disabilitati
    if (!_consentiBreakpoint) {
        return;
    }

    //accende il led per un tempo indefinito
    Debug::accendiLed(0);


    //se attesaMassima è 0 (= usa default) oppure se l'utente ha deciso di usarla sempre
    // e non l'ha definita nei parametri della funzione usa il tempo di attesa di default
    // Lo stesso vale se non si usa il serial (in tal caso il tempo è l'unico modo per
    // sbloccare il breakpoint)
    if(attesaMassima == 0 || ((_usaSempreAttesaMassimaBreak || !_usaSerial) && attesaMassima < 0)) {
        attesaMassima = _attesaMassimaBreakpoint;
    }


    //"impostazione privata" di questa funzione: nr. di puntini per rappresentare
    // il tempo di attesaMassima che scade. Serve anche per far lampeggiare il LED
    int nrPuntini = S_NR_PUNTI_LINEA_TIMEOUT;


    #ifdef DEBUG_USA_SERIAL

    //salva l'ora dell'interruzione del programma
    unsigned long tempoInterruzione = millis();


    if(_usaSerial) {

        super::print("\n"); //salta una riga

        //già che è definito sopra, usa `tempoInterruzione` invece di millis()
        super::print(tempoInterruzione);   //stampa il tempo
        super::print(S_SEP_T_NR);

        super::print(S_BREAK);   //scrivi che è un breakpoint
        super::print(numero);     //stampa il nr. che rappresenta il breakpoint

        if (codice) {
            super::print(S_SEP_NR_COD);
            super::print(codice);  //ev. stampa il codice
        }

        super::print("\n");      //vai a capo


        for(int i = 0; i < nrPuntini; i++) {
            super::print(S_CHAR_MODELLO_TIMEOUT);          //stampa una riga di punti (.....)
        }

    }


    //--------------------------------------------------------------------------

    //numero di puntini che rappresentano lo scorrere del tempo `attesaMassima`
    //già disegnati
    int nrPuntiniDisegnati;

    #endif


    //aspetta che l'utente o lo scadere del tempo massimo permettano di continuare
    while(true) {

        #ifdef DEBUG_USA_SERIAL

        if(_usaSerial) {

            if(super::available() > 0) {

                if(C_IN_SBLOCCA_BREAKPOINT != '\0') {

                    char c = super::read();
                    if (c == C_IN_SBLOCCA_BREAKPOINT) {
                        break;// da while(true)
                    }
                }
                //se il carattere C_IN_SBLOCCA_BREAKPOINT è '\0'
                else break;// da while(true)

            }//if(super::available() > 0) {
        }

        #endif

        if(attesaMassima > 0) {

            //se è passato 1/nrPuntini del tempo disegna un nuovo puntino e fai lampeggiare il led
            if(tempoInterruzione + nrPuntiniDisegnati * (attesaMassima / nrPuntini) <= millis()) {

                #ifdef DEBUG_USA_SERIAL
                if(_usaSerial) super::print(S_CHAR_LINEA_TIMEOUT);
                #endif

                if(_ledAcceso) spegniLed();
                else accendiLed(0);
            }

            if(tempoInterruzione + attesaMassima <= millis()) {
                break;
            }
        }
    }

    #ifdef DEBUG_USA_SERIAL

    //scrivi l'ora della fine della pausa
    super::print(millis());
    super::print(S_SEP_T_NR);
    super::print(S_FINE_BREAK);
    super::print("\n");

    #endif

    Debug::spegniLed();

    #endif //DEBUG_ABILITA_BREAKPOINT
}


#endif //#ifdef DEBUG_ABILITA
