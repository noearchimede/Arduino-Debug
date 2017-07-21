/**
\file

\brief funzioni per l'assegnazione di un valore scelto dall'utente a una variabile
del programma in esecuzione

Il file contiene le funzioni pubbliche `assegnaValore` per ogni tipo di variabile
possibile e le funzioni private che servono ad implementare `assegnaValore`

\date 16 - 17 luglio 2017

*/

#include "Debug.hpp"

#ifdef DEBUG_ABILITA
#ifdef DEBUG_ABILITA_ASSEGNA

#include "Debug_string.hpp"

/**
Questa funzione permette di chiedere all'utente di assegnare un valore a una
variabile nel programma in esecuzione.
La variabile può essere di qualsiasi tipo (per ogni tipo è definita una funzione
apposita).

\note funziona solo se `_usaHardwareSerial == true`

\param var pointer alla variabile da modificare
\param numero un numero corrispondente a un messaggio che specifica ad es. quale
variabile sta per essere modificata
\param codice codice associato al numero, come per le funzioni precedenti. Può essere
ad es. usato per indicare il valore attuale della variabile da modificare.
*/
void Debug::assegnaValore(bool* pointer, int numero, long codice) {
    if(_usaHardwareSerial) {
        while(true) {

            Debug::azioniPrimaAssegnaValore(numero, codice);

            Debug::ottieniNumeroSerial(NULL, NULL, NULL, pointer);

            //per i bool non c'è controllo dei limiti

            if(Debug::confermaAssegnaValore()) {
                break;
            }
        }

        Debug::azioniDopoAssegnaValore();
    }
}
void Debug::assegnaValore(char* pointer, int numero, long codice) {
    if(_usaHardwareSerial) {
        if(_usaHardwareSerial) {
            long valore;
            while(true) {
                Debug::azioniPrimaAssegnaValore(numero, codice);
                Debug::ottieniNumeroSerial(NULL, &valore, NULL, NULL);
                if(!Debug::controllaLimiti(valore, -128, 127)) continue;
                if(Debug::confermaAssegnaValore()) break;
            }
            *pointer = (char)valore;
            Debug::azioniDopoAssegnaValore();
        }
    }
}
void Debug::assegnaValore(byte* pointer, int numero, long codice) {
    if(_usaHardwareSerial) {
        unsigned long valore;
        while(true) {
            Debug::azioniPrimaAssegnaValore(numero, codice);
            Debug::ottieniNumeroSerial(&valore, NULL, NULL, NULL);
            if(!Debug::controllaLimiti(valore, 0, 255)) continue;
            if(Debug::confermaAssegnaValore()) break;
        }
        *pointer = (byte)valore;
        Debug::azioniDopoAssegnaValore();
    }
}
void Debug::assegnaValore(int* pointer, int numero, long codice) {
    if(_usaHardwareSerial) {
        long valore;
        while(true) {
            Debug::azioniPrimaAssegnaValore(numero, codice);
            Debug::ottieniNumeroSerial(NULL, &valore, NULL, NULL);
            if(!Debug::controllaLimiti(valore, -32768, 32767)) continue;
            if(Debug::confermaAssegnaValore()) break;
        }
        *pointer = (int)valore;
        Debug::azioniDopoAssegnaValore();
    }
}
void Debug::assegnaValore(unsigned int* pointer, int numero, long codice) {
    if(_usaHardwareSerial) {
        unsigned long valore;
        while(true) {
            Debug::azioniPrimaAssegnaValore(numero, codice);
            Debug::ottieniNumeroSerial(&valore, NULL, NULL, NULL);
            if(!Debug::controllaLimiti(valore, 0, 65535)) continue;
            if(Debug::confermaAssegnaValore()) break;
        }
        *pointer = (unsigned int)valore;
        Debug::azioniDopoAssegnaValore();
    }
}
void Debug::assegnaValore(long* pointer, int numero, long codice) {
    if(_usaHardwareSerial) {
        long valore;
        while(true) {
            Debug::azioniPrimaAssegnaValore(numero, codice);
            Debug::ottieniNumeroSerial(NULL, &valore, NULL, NULL);
            //non ha senso controllare i limiti, perché se eccede a questo punto è già stato
            // reso un valore casuale nel dominio di unsigned long
            if(Debug::confermaAssegnaValore()) break;
        }
        *pointer = valore;
        Debug::azioniDopoAssegnaValore();
    }
}
void Debug::assegnaValore(unsigned long* pointer, int numero, long codice) {
    if(_usaHardwareSerial) {
        unsigned long valore;
        while(true) {
            Debug::azioniPrimaAssegnaValore(numero, codice);
            Debug::ottieniNumeroSerial(&valore, NULL, NULL, NULL);
            //non ha senso controllare i limiti, perché se eccede a questo punto è già stato
            // reso un valore casuale nel dominio di long
            if(Debug::confermaAssegnaValore()) break;
        }
        *pointer = valore;
        Debug::azioniDopoAssegnaValore();
    }
}
void Debug::assegnaValore(float* pointer, int numero, long codice) {
    if(_usaHardwareSerial) {
        while(true) {
            Debug::azioniPrimaAssegnaValore(numero, codice);
            Debug::ottieniNumeroSerial(NULL, NULL, pointer, NULL);
            if(Debug::confermaAssegnaValore()) break;
        }
        Debug::azioniDopoAssegnaValore();
    }
}



/**
La funzione ha tre parametri: un pointer per ciascuno dei tre tipi di numero possibili.
Per chiamarla bisogna assegnare al parametro del tipo desiderato un pointer valido
e agli altri due parametri NULL. Se si passano più pointer validi la funzione usa
il tipo corrispondente al parametro più a sinistra.

Sul monitor seriale appare il tipo di valore richiesto; poi il programma aspetta.
Quando l'utente invia dei caratteri la funzione controlla se sono numeri e se sono
del tipo giusto, e se l'input è valido lo converte in numero e lo salva nella
variabile il cui ponter le è stato passato come argomento.

I caratteri accettati per inserire numeri sono:
- cifre decimali (0 - 9);
- _[float]_ punto `.` (la virgola non vale), ovviamente una volta sola
- _[float e unsigned long]_ segno meno `-`, ovviamente solo una volta all'inizio
- apostrofo `'`, ignorato dal programma ma talvolta utile per contare mentre si scrivere

Se il valore richiesto è un bool i caratteri validi sono:
- `0` e `f` par `false`
- `1` e `t` per `true`

\warning La funzione non controlla se il numero è troppo grande! Se si eccede il
limite del tipo scelto il valore sarà casuale.

\param risultatoULong pointer a un unsigned long; passare `NULL` per usare un altro tipo
\param risultatoLong pointer a un long; passare `NULL` per usare un altro tipo
\param risultatoFloat pointer a un float; passare `NULL` per usare un altro tipo
\param risultatoBool pointer a un bool; passare `NULL` per usare un altro tipo

*/
void Debug::ottieniNumeroSerial(unsigned long* risultatoULong, long* risultatoLong, float* risultatoFloat, bool* risultatoBool) {

    //stabilisce che tipo di dati serve
    bool serveULong = false;
    bool serveLong = false;
    bool serveFloat = false;
    bool serveBool = false;
    if (risultatoULong != NULL) serveULong = true;
    else if (risultatoLong != NULL) serveLong = true;
    else if (risultatoFloat != NULL) serveFloat = true;
    else if (risultatoBool != NULL) serveBool = true;

    //la funzione si divide in due parti: (1) ricezione dei dati e (2) "calcolo" del numero in base
    // al testo inserito (che è un'array di `char`).

    //### 1. RICEZIONE DATI ####

    //indice dell'ultima cifra (la meno significativa) nell'array
    int indiceUltimaCifra;
    //è intero? sì fino a prova contraria
    bool intero;
    //È un valore positivo? sì fino a prova contraria
    bool positivo;
    //indice dell'ultima cifra a sinistra della virgola
    int indiceUnita;
    //tutti i caratteri ricevuti (10 è il massimo di cifre decimali per un long)
    char testo[10];

    while (true) {

        _hardwareSerial.print(S_PREFISSO_ASSEGNA);

        //inizializza tutti i valori (in caso di errore si ricomincia da qui)
        indiceUltimaCifra = 0;
        intero = true;
        positivo = true;
        indiceUnita = 0;
        for (int i = 0; i < 10; i++) {
            testo[i] = '0';
        }

        //condizioni di errore che dipendono dalla richiesta
        bool siaPositivo, siaIntero;
        if (serveULong) { //intero, positivo
            siaPositivo = true;
            siaIntero = true;
        }
        else if (serveLong) { //intero, pos/neg
            siaPositivo = false;
            siaIntero = true;
        }
        else if (serveFloat) { //int/dec, pos/neg
            siaPositivo = false;
            siaIntero = false;
        }


        //segnala che tipo di numero immettere (e quindi, implicitamente, anche che al programma
        // serve un numero)
        if (serveULong) _hardwareSerial.print(S_ASSEGNA_ULONG);
        else if (serveLong) _hardwareSerial.print(S_ASSEGNA_LONG);
        else if (serveFloat) _hardwareSerial.print(S_ASSEGNA_FLOAT);
        else if (serveBool) _hardwareSerial.print(S_ASSEGNA_BOOL);


        //l'ultimo carattere rivcevuto
        char c;
        //il valore ricevuto non è un numero (contiene almeno un segno che non è una cifra)
        bool carattereNonValido = false;
        //il valore ricevuto ha troppe cifre (più di 10)
        bool numeroTroppoLungo = false;


        //aspetta finché arriva qualcosa nel buffer seriale
        while (!_hardwareSerial.available());

        //togli un elemento alla volta dal buffer fino a quando è vuoto
        int indice = 0;
        while (_hardwareSerial.available()) {

            c = _hardwareSerial.read();

            //stampa l'input prima dell'elaborazione in numero
            //_hardwareSerial.print(c);


            //innanzitutto gestisce il caso serveBool, che è molto più semplice
            if(serveBool) {

                //rimuovi tutti i caratteri rimanenti dal buffer seriale
                delay(2);
                while (_hardwareSerial.available()) {
                    _hardwareSerial.read();
                    delay(2);
                }

                if(c == C_IN_ASSEGNA_BOOL_FALSE_1 || c == C_IN_ASSEGNA_BOOL_FALSE_2) {
                    _hardwareSerial.print(S_ASSEGNA_BOOL_FALSE);
                    _hardwareSerial.print("\n");
                    *risultatoBool = false;
                    return;
                }
                else if(c == C_IN_ASSEGNA_BOOL_TRUE_1 || c == C_IN_ASSEGNA_BOOL_TRUE_2) {
                    _hardwareSerial.print(S_ASSEGNA_BOOL_TRUE);
                    _hardwareSerial.print("\n");
                    *risultatoBool = true;
                    return;
                }
                else {
                    carattereNonValido = true;
                    break; //da `while (_hardwareSerial.available())`
                }

                //in nessun caso si arriva qui

            }//if(serveBool)


            //5 POSSIBILITÀ per non-bool

            //1.1 se il carattere è uno zero iniziale va semplicemente ignorato. Questo non
            // impedisce di scrivere ad es. 0.1 (si può scrivere comunque 0.1 o .1, sono equivalenti)
            if (c == '0' && indice == 0) {}
            //1.2 se il carattere è C_IN_SEPARATORE_MIGLIAIA va ugualmente ignorato (è un segnaposto
            // per l'utente, che può usarlo a suo piacimento)
            else if (c == C_IN_SEPARATORE_MIGLIAIA) {}

            //2. se il carattere è una cifra aggiungilo alla stringa.
            else if (('0' <= c && c <= '9')) {

                //controlla che l'indice non _hardwareSeriali il valore massimo
                if (indice == 10) {
                    numeroTroppoLungo = true;
                    break; //da `while (_hardwareSerial.available())`
                }

                //salva il carattere
                testo[indice] = c;
                indice++;
            }

            //3. se il carattere è C_IN_PUNTO segnane la indice
            // Il punto è valido solo se è unico in tutta la stringa (cioè se non ce
            // ne sono stati altri prima) e se il numero non deve per forza essere intero
            else if (c == C_IN_PUNTO && intero && !siaIntero) {
                intero = false;

                //subito dopo l'utilizzo l'indice aumenta, quindi qui bisogna ridurre il numero di 1
                indiceUnita = indice - 1;
            }

            //4. se il carattre è C_IN_MENO, ricordalo
            // Il segno meno è valido solo se è uno solo e in prima indice e se il
            // numero non deve per forza essere positivo
            else if (c == C_IN_MENO && positivo && indice == 0 && !siaPositivo) {
                positivo = false;
            }

            //5. se non è un numero, punto o segno meno segnala il problema ed esci dal loop
            else {
                carattereNonValido = true;
                break; //da `while (_hardwareSerial.available())`
            }

            //lascia tempo al buffer di riempirsi (2 ms bastano anche a 9600 baud)
            delay(2);

        }

        if (carattereNonValido || numeroTroppoLungo) {

            //rimuovi tutti i caratteri dal buffer seriale
            delay(2);
            while (_hardwareSerial.available()) {
                _hardwareSerial.read();
                delay(2);
            }

            //segnala l'errore e ricomincia la funzione da capo (siamo in un `while(true)`!)

            //se il problema è la lunghezza stampa un messaggio di errore specifico
            if (numeroTroppoLungo) {
                _hardwareSerial.print(S_ASSEGNA_NR_TROPPO_LUNGO);
            }
            //altrimenti segnala un errore generico
            else {
                _hardwareSerial.print(S_ASSEGNA_CHAR_NON_VALIDO);
            }

            _hardwareSerial.print("\n");

            continue; //`while(true)`
        }

        //se si arriva qui il numero inserito va bene

        //se il numero è intero, l'unità si trova all'ultimo indice usato (subito dopo l'utilizzo
        // l'indice aumenta, quindi qui bisogna ridurre il numero di 1)
        if (intero) {
            indiceUnita = indice - 1;
        }

        //in ogni caso il numero totale di cifre corrisponde all'ultimo indice (subito dopo l'utilizzo
        // l'indice aumenta, quindi qui bisogna ridurre il numero di 1)
        indiceUltimaCifra = indice - 1;


        //se si arriva qui il numero è valido e registrato nell'array `testo`, e
        // il buffer seriale è vuoto. Si può quindi uscire dal `while(true)`
        break; //da `while(true)`

    }


    //### 2. CALCOLO DEL NUMERO ###

    //definizione delle potenze di 10 per non doverle calcolare ogni volta
    long potDi10[10] = {
        1,//0
        10,//1
        100,//2
        1000,//3
        10000,//4
        100000,//5
        1000000,//6
        10000000,//7
        100000000,//8
        1000000000 //9
    };

    //trasforma l'array di char `testo` in un'array di numeri
    for (int i = 0; i < 10; i++) {
        testo[i] -= '0';
    }


    if (serveULong) {

        *risultatoULong = 0;

        for (int i = 0; i <= indiceUltimaCifra; i++) {
            *risultatoULong += testo[i] * potDi10[indiceUltimaCifra - i];
        }

        _hardwareSerial.print(*risultatoULong);

    }//if (serveULong)

    else if (serveLong) {

        *risultatoLong = 0;

        for (int i = 0; i <= indiceUltimaCifra; i++) {
            *risultatoLong += testo[i] * potDi10[indiceUltimaCifra - i];
        }
        if (!positivo) {
            *risultatoLong = 0 - *risultatoLong;
        }

        _hardwareSerial.print(*risultatoLong);

    }//else if (serveLong)

    else if (serveFloat) {

        *risultatoFloat = 0;

        for (int i = 0; i <= indiceUnita; i++) {
            *risultatoFloat += testo[i] * potDi10[indiceUnita - i];
        }
        for (int i = indiceUnita + 1; i <= indiceUltimaCifra; i++) {
            *risultatoFloat += (float) testo[i] / (float) potDi10[i - indiceUnita];
        }
        if (!positivo) {
            *risultatoFloat = 0 - *risultatoFloat;
        }

        _hardwareSerial.print(*risultatoFloat, indiceUltimaCifra - indiceUnita);

    }//else if (serveFloat)

    _hardwareSerial.print("\n");

} //fine funzione `ottieniNumeroSerial`


/**
Questa funzione contiene una parte di codice comune a tutte le funzioni `assegnaValore()`:
stampa alcune informazioni prima di chiedere all'utente l'immissione di un valore
e accende il led
*/
void Debug::azioniPrimaAssegnaValore(int numero, long codice) {

    if(_usaHardwareSerial) {

        //accedne il led per un tempo indefinito
        Debug::accendiLed(0);


        _hardwareSerial.print("\n"); //salta una riga

        _hardwareSerial.print(millis());   //stampa il tempo
        _hardwareSerial.print(S_SEP_T_NR);

        _hardwareSerial.print(S_ASSEGNA);   //scrivi che è un'assengazione di un valore
        _hardwareSerial.print(numero);     //stampa il nr. che rappresenta il breakpoint

        if (codice) {
            _hardwareSerial.print(S_SEP_NR_COD);
            _hardwareSerial.print(codice);  //ev. stampa il codice
        }

        _hardwareSerial.print("\n");      //vai a capo

    }
}


/**
Questa funzione controlla che il valore che si sta per assegnare a una variabile
non ne ecceda i limiti.
*/

bool Debug::controllaLimiti(long var, long min, long max) {

    if(var > max) {
        _hardwareSerial.print(S_PREFISSO_ASSEGNA);
        _hardwareSerial.print(S_ASSEGNA_MAX_UGUALE);
        _hardwareSerial.print(max);
        _hardwareSerial.print("\n");
        return false;
    }

    if (var < min){
        _hardwareSerial.print(S_PREFISSO_ASSEGNA);
        _hardwareSerial.print(S_ASSEGNA_MIN_UGUALE);
        _hardwareSerial.print(min);
        _hardwareSerial.print("\n");
        return false;
    }

    return true;
}


/**
Questa funzione contiene una parte di codice comune a tutte le funzioni `assegnaValore()`:
chiede se il valore inserito va bene o se l'utente vuole cambiare
*/
bool Debug::confermaAssegnaValore() {

    _hardwareSerial.print(S_PREFISSO_ASSEGNA);
    _hardwareSerial.print(S_ASSEGNA_CHIEDI_CONFERMA);

    char c;
    while(true) {
        //se non c'è niente `c == -1`
        c = _hardwareSerial.read();
        if(c == C_IN_CONFERMA_SI) {
            _hardwareSerial.print("\n");
            return true;
        }
        if(c == C_IN_CONFERMA_NO) {
            _hardwareSerial.print("\n");
            return false;
        }
    }

}


/**
Questa funzione contiene una parte di codice comune a tutte le funzioni `assegnaValore()`:
stampa alcune informazioni dopo aver ottenuto dall'utente un valore valido e spegne
il led
*/
void Debug::azioniDopoAssegnaValore() {

    //scrivi l'ora della fine della pausa
    _hardwareSerial.print(millis());
    _hardwareSerial.print(S_SEP_T_NR);
    _hardwareSerial.print(S_FINE_ASSEGNA);
    _hardwareSerial.print("\n");


    Debug::spegniLed();
}


#endif
#endif
