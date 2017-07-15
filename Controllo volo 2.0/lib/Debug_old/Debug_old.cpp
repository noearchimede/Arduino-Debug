/**
\file

\brief sorgente della classe `Debug`
\date 5 - 9 luglio 2017
*/

#include "Debug_old.hpp"


/** Assegna tutti i valori di default e collega la classe Serial.

\warning `Serial` deve essere inizializzato nel codice (tipicamente in `void setup()`).
Per inizializzarlo non chiamare `Serial.begin(<baud>)`, ma `Debug::inizializzaSerial`
(`Serial.begin()` sarà poi chiamata all'interno di quella funzione).

\warning si deve passare alla funzione e inizializzare Serial anche se non si
intende usarlo, perché in caso di errore fatale sarà comunque attivato.

\param serial Un'istanza della classe HardwareSerial. Sarà presumibilmente sempre
`Serial` (`Debug(Serial, <pin>)`)

\param pinLed Pin del led a disposizione della classe (che dovrà detenere il monopolio
sul suo utilizzo). Normalmente è il pin 13, visto che sulla scheda Arduino UNO
c'è già un LED collegato ad essa.


*/
Debug::Debug(HardwareSerial *serial, int pinLed) {

    _pinLed = pinLed;
    _usaSerial = DEBUG_DEFAULT_USA_SERIAL;
    _baudComunicazioneSeriale = DEBUG_DEFAULT_BAUD_SERIAL;
    _stampaMessaggi = DEBUG_DEFAULT_STAMPA_MESS;
    _stampaMinimo = DEBUG_DEFAULT_STAMPA_MINIMO;
    _ignoraCodice = DEBUG_DEFAULT_IGNORA_CODICE;
    _durataLuceMessaggio = DEBUG_DEFAULT_LUCE_MESS;
    _durataLuceErrore = DEBUG_DEFAULT_LUCE_ERR;
    _durataLuceErroreFatale = DEBUG_DEFAULT_LUCE_ERRFAT;
    _durataBuioDopoNotifica = DEBUG_DEFAULT_DURATA_BUIO_DOPO_NOTIFICA;
    _aspettaFineNotifica = DEBUG_DEFAULT_ASPETTA_FINE_NOTIFICA;

    //salva un riferimento a serial
    _serial = serial;

    //prepara il LED
    pinMode(_pinLed, OUTPUT);

}


/**
Con "messaggio" si intende qualsiasi tipo di notifica a parte gli errori. Sono
quindi messaggi:
- i checkpoints
- i risultati finali o parziali dei calcoli svolti nel programma
- segnalazioni di qualsiasi tipo di input esterno

I messaggi, come gli errori, sono rappresentati da un numero (definito in
un'apposita tabella di `#define`). Al messaggio può essere associato un codice:
ad esempio si potrebbe avere il messaggio "calcolo la radice quadrata di" con
codice "16", e poi il messaggio "risultato" con codice "4"; oppure l'errore
"la funzione func() non ha restituito 0 ma..." con codice "3".

Inoltre, se non si è selezionata la modalità "stampaMinimo", accanto a ogni messaggio
o errore apparirà il momento (in ms dall'inizio del programma) in cui si è verificato.

Se in un dato punto non è importante che il programma non venga rallentato più
del minimo necessario e per qualche motivo si vuole vedere ogni segnale del LED
singolarmente, si può impostare su `true`il terzo parametro.

\param numero Il numero che rappresenta il messaggio
\param codice [opzionale] Il codice/numero/... associato a quel messaggio
\param aspettaFineNotifica [opzionale] Blocca il programma fino a che il LED è spento
*/
void Debug::messaggio(int numero, long codice, bool aspettaFineNotifica) {

    Debug::accendiLed(_durataLuceMessaggio);

    //esci dalla la funzione se non si vuole che siano stampati i messaggi comuni
    if (!_stampaMessaggi)
    return;

    if(_usaSerial) {

        if(_stampaMinimo) {

            _serial->print(numero);      //stampa il nr. che rappresenta il messaggio

            if (codice && !_ignoraCodice) {
                _serial->print(":");
                _serial->print(codice);  //ev. stampa il codice
            }

            _serial->print("\n");        //vai a capo
        }

        else { //cioé if !_stampaMinimo

            _serial->print(millis());   //stampa il tempo
            _serial->print("\t");

            _serial->print(numero);     //stampa il nr. che rappresenta il messaggio

            if (codice) {
                _serial->print(":");
                _serial->print(codice);  //ev. stampa il codice
            }

            _serial->print("\n");       //vai a capo
        }
    }

    if(_aspettaFineNotifica || aspettaFineNotifica)
    Debug::aspettaFineNotifica();
}



/**
Con questa funzione si possono segnalare tutti gli errori che avvengono nel
programma ma che non ne compromettono totalmente il funzionamento, ma anche gli
errori fatali nel caso in cui si voglia vedere il comportamento dopo l'errore.

I numeri che rappresentano gli errori sono distinti da quelli dei messaggi (gli
errori sono segnalati in modo differente sia dal LED sia sul monitor seriale),
quindi le due liste di `#define` (quella dei messaggi e quella degli errori)
iniziano entrambe da 1 e hanno una numerazione indipendente.

Per più informazioni sul significato di `numero` e `codice` cfr. la documentazione
della funzione `messaggio(int, long)`.

Ancheper informazioni sul terzo parametro cfr. la documentazione della funzione
`messaggio(int, long)`.

\param numero Il numero che rappresenta il messaggio
\param codice [opzionale] Il codice/numero/... associato a quel messaggio
\param aspettaFineNotifica [opzionale] Blocca il programma fino a che il LED è spento
*/
void Debug::errore(int numero, long codice, bool aspettaFineNotifica) {

    Debug::accendiLed(_durataLuceErrore);

    if(_usaSerial) {

        if(_stampaMinimo) {

            _serial->print("E");          //segnala che si tratta di un errore
            _serial->print(numero);      //stampa il nr. che rappresenta l'errore

            if (codice && !_ignoraCodice) {
                _serial->print(":");
                _serial->print(codice);  //ev. stampa il codice
            }

            _serial->print("\n");        //vai a capo
        }

        else { //cioé if !_stampaMinimo

            _serial->print("\n"); //salta una riga

            _serial->print(millis());   //stampa il tempo
            _serial->print("\t");

            _serial->print("errore ");   //scrivi che è un messaggio
            _serial->print(numero);     //stampa il nr. che rappresenta il messaggio

            if (codice) {
                _serial->print(":");
                _serial->print(codice);  //ev. stampa il codice
            }

            _serial->print("\n\n");       //vai a capo e lascia una riga vuota
        }
    }

    if(_aspettaFineNotifica || aspettaFineNotifica)
    Debug::aspettaFineNotifica();

}


/**
Questa funzione segnala un errore fatale e blocca il programma.
La segnalazione avviente, come sempre, in due modi: tramite il LED e tramite il
monitor seriale.
-   Il LED lampeggia incessantemente, restando acceso per `_durataLuceErroreFatale`
ms e spento per lo stesso tempo (i tempi non sono precisi).
-   Il monitor seriale manda a ogni lampeggiamento un messaggio di errore con il
tempo attuale, il tempo a cui si è verificato l'errore e il numero/ev. codice
dell'errore.

Dopo `DEBUG_TEMPO_RESET_SERIAL` ms il programma inizia a riinizializzare il
monitor seriale prima di ogni messaggio di errore. In questo modo anche se si
connette il computer al robot solo dopo che si è verificato l'errore si può
sapere che errore c'è stato.

I numeri che rappresentano gli errori fatali sonodistinti sia da quelli dei messaggi
sia da quelli degli errori, perciò avranno una lista di `#define` indipendete.

\param numero Il numero che rappresenta l'errore
\param codice [facoltativo] Il codice/numero/... associato a quell'errore
*/
void Debug::erroreFatale(int numero, long codice) {

    //salva l'ora a cui si è boccato il programma
    unsigned long tempoBlocco = millis();

    if(_usaSerial)
    _serial->print("\n");       //salta una riga (se sta usando serial)


    for (int i = 0; i < 20; i++)
    if(_usaSerial)
    _serial->print("-");      //stampa una riga di ----- (se sta usando serial)

    if(_usaSerial)
    _serial->print("\n");       //vai a capo (se sta usando serial)



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
        _serial->begin(_baudComunicazioneSeriale);

        //questa condizione sarà certamente vera a partire da quando il monitor
        // inizierà ad essere resettato
        if(_usaSerial) {

            _serial->print("Errore fatale a ");  //stampa "Errore fatale a 876578 ms:"
            _serial->print(tempoBlocco);
            _serial->print(" ms:\n");   //va a capo

            _serial->print(millis());   //stampa il tempo attuale
            _serial->print("\t");

            _serial->print("err. ");
            _serial->print(numero);     //stampa il nr. che rappresenta il messaggio

            if (codice) {
                _serial->print(":");
                _serial->print(codice);   //ev. stampa il codice
            }

            _serial->print("\n\n");     //va a capo e salta una riga

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
Questa funzione è fondamentale per l'utilizzo del LED da parte della classe Debug.
Le altre funzioni possono accendere la spia luminosa ma non spegnerla; questa
funzione si occupa quindi di controllare se è "ora" di spegnere il LED e se è il
caso lo spegne.

\warning Questa funzione deve essere chiamata regolarmente e frequentemente per
poter avere dei segnali luminosi della giusta durata, altrimenti la luce resta
accesa per un tempo arbitrario che non dipende dal tempo richiesto dalla funzione
che l'ha accesa (ad es `messaggio()`) ma dalla posizione della chiamata di questa
funzione nel codice.
*/
void Debug::controllaLed() {
    if (!_ledAcceso)
    return;
    if (_tempoAccensioneLed + _durataLuceLed < millis()) {
        Debug::spegniLed();
    }
}


/**
\param usaSerial la nuova impostazione
*/
void Debug::impostaUsaSerial(bool usaSerial) {
    _usaSerial = usaSerial;
}


void Debug::impostaStampaMessaggi(bool stampaMessaggi) {
    _stampaMessaggi = stampaMessaggi;
}


void Debug::impostaStampaMinimo(bool stampaMinimo) {
    _stampaMinimo = stampaMinimo;
}


/**
\param mess durata della luce per i messaggi
\param err durata della luce per gli errori
\param errFat durata della luce nel lampeggiamento per gli errori fatali

Per lasciare invariato un valore, passare un numero negativo come argomento
corrispondente a quel valore; ad es.:
```cpp
impostaDurateLuci(-1, 350, -1);
```
modifica solo la durata della luce per gli errori.
*/
void Debug::impostaDurateLuci(int mess, int err, int errFat) {
    if (mess >= 0)
    _durataLuceMessaggio = mess;
    if (err >= 0)
    _durataLuceErrore = err;
    if (errFat >= 0)
    _durataLuceErroreFatale = errFat;
}

void Debug::impostaDurataBuioDopoNotifica(int durata) {
    _durataBuioDopoNotifica = durata;
}


/**
\return `true` se sta usando la comunicazione seriale, `false` altrimenti
*/
bool Debug::staUsandoSerial() {
    return _usaSerial;
}

/**
\param durata "Richiesta" di durata della luce. La funzione `controllaLed()`
leggerà questo valore e si occuperà di spegnere il LED dopo lo scadere del tempo
qui determinato.
*/
void Debug::accendiLed(int durata) {
    digitalWrite(_pinLed, HIGH);
    _ledAcceso = true;
    _tempoAccensioneLed = millis();
    _durataLuceLed = durata;

}


void Debug::spegniLed() {
    digitalWrite(_pinLed, LOW);
    _ledAcceso = false;
    _durataLuceLed = 0;
}

/**
Il tempo di attesa supplementare dopo lo spegnimento del LED è necessario per vedere
lo "stacco" fra due segnali luminosi se due notifiche sono molto vicine traloro.
*/
void Debug::aspettaFineNotifica() {

    while(_ledAcceso)
    Debug::controllaLed();

    delay(_durataBuioDopoNotifica);
}
