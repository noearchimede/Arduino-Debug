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

    _monitor.print("\n");       //salta una riga

    for (int i = 0; i < S_NR_PUNTI_LINEA_ERRFAT; i++)

    _monitor.print(S_CHAR_LINEA_ERRFAT);      //stampa una riga di -----

    _monitor.print("\n");       //vai a capo

    //loop infinito, nessuna uscita possibile.
    while (true) {

        accendiLed(0);
        delay(_durataLuceErroreFatale);
        spegniLed();
        delay(_durataLuceErroreFatale);


        _monitor.print(S_ERRFAT_PRIMA_DI_TEMPO);  //stampa "Errore fatale a 876578 ms:"
        _monitor.print(tempoBlocco);
        _monitor.print(S_ERRFAT_DOPO_TEMPO);   //va a capo

        _monitor.print(millis());   //stampa il tempo attuale
        _monitor.print(S_SEP_T_NR);

        _monitor.print(S_ERRFAT);
        _monitor.print(numero);     //stampa il nr. che rappresenta il messaggio

        if (codice) {
            _monitor.print(S_SEP_NR_COD);
            _monitor.print(codice);   //ev. stampa il codice
        }

        _monitor.print("\n\n");     //va a capo e salta una riga



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
automaticamente. Se è 0 (default) il programma riprende esclusivamente su
richiesta dell'utente, se vale 1 userà il tempo di timeout di default.
*/
void Debug::breakpoint(int numero, long codice, unsigned long attesaMassima) {

    #ifdef DEBUG_ABILITA_BREAKPOINT

    //esci dalla la funzione se i breakpoint sono disabilitati
    if (!_consentiBreakpoint) {
        return;
    }

    //salva l'ora dell'interruzione del programma
    unsigned long tempoInterruzione = millis();


    //accende il led per un tempo indefinito
    accendiLed(0);


    //se attesaMassima è 1 (= usa default) oppure se l'utente ha deciso di usarla sempre
    // e non l'ha definita nei parametri della funzione usa il tempo di attesa di default
    // Lo stesso vale se non si usa il serial (in tal caso il tempo è l'unico modo per
    // sbloccare il breakpoint)
    if(attesaMassima == 1 || ((_usaSempreAttesaMassimaBreak || !_usaHardwareSerial) && !attesaMassima)) {
        attesaMassima = _attesaMassimaBreakpoint;
    }


    //"impostazione privata" di questa funzione: nr. di puntini per rappresentare
    // il tempo di attesaMassima che scade. Serve anche per far lampeggiare il LED
    int nrPuntini = S_NR_PUNTI_LINEA_TIMEOUT;



    _monitor.print("\n"); //salta una riga

    //già che è definito sopra, usa `tempoInterruzione` invece di millis()
    _monitor.print(tempoInterruzione);   //stampa il tempo
    _monitor.print(S_SEP_T_NR);

    _monitor.print(S_BREAK);   //scrivi che è un breakpoint
    _monitor.print(numero);     //stampa il nr. che rappresenta il breakpoint

    if (codice) {
        _monitor.print(S_SEP_NR_COD);
        _monitor.print(codice);  //ev. stampa il codice
    }

    _monitor.print("\n");      //vai a capo


    for(int i = 0; i < nrPuntini; i++) {
        _monitor.print(S_CHAR_MODELLO_TIMEOUT);          //stampa una riga di punti (.....)
    }


    //--------------------------------------------------------------------------

    //numero di puntini che rappresentano lo scorrere del tempo `attesaMassima`
    //già disegnati.
    int nrPuntiniDisegnati = 0;

    //va a capo se deve stampare i puntini che segnano il passare del tempo
    if (attesaMassima > 0) {
        _monitor.print("\n");
    }

    //aspetta che l'utente o lo scadere del tempo massimo permettano di continuare
    while(true) {


        if(_usaHardwareSerial && _monitor.available()) {

            if(C_IN_SBLOCCA_BREAKPOINT != '\0') {

                char c = _monitor.read();
                if (c == C_IN_SBLOCCA_BREAKPOINT) {
                    while(_monitor.available()) {
                        _monitor.read();
                    }
                    break;// da while(true)
                }
            }
            //se il carattere C_IN_SBLOCCA_BREAKPOINT è '\0'
            else {
                while(_monitor.available()) {
                    _monitor.read();
                }
                break;// da while(true)
            }

        }//if(_usaHardwareSerial && _monitor.available() > 0)



        if(attesaMassima > 0) {

            //se è passato 1/nrPuntini del tempo disegna un nuovo puntino e fai lampeggiare il led
            if(tempoInterruzione + nrPuntiniDisegnati * (attesaMassima / nrPuntini) < millis()) {

                _monitor.print(S_CHAR_LINEA_TIMEOUT);
                nrPuntiniDisegnati++;

                if(_ledAcceso) spegniLed();
                else accendiLed(0);
            }

            if(tempoInterruzione + attesaMassima <= millis()) {
                break;
            }
        }
    }

    //scrivi l'ora della fine della pausa
    _monitor.print("\n");
    _monitor.print(millis());
    _monitor.print(S_SEP_T_NR);
    _monitor.print(S_FINE_BREAK);
    _monitor.print("\n\n");           //salta una riga

    spegniLed();

    #endif //DEBUG_ABILITA_BREAKPOINT
}


#endif //#ifdef DEBUG_ABILITA
