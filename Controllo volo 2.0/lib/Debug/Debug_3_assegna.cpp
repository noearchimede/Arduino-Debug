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


static uint64_t intPow(int base, int exp) {
    uint64_t x = 1;
    for(int i = 0; i < exp; i++) x *= base;
    return x;
}


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
    if(!_usaHardwareSerial) return;
    NumeroQualsiasi valore (tipo_bo);
    azioniPrimaAssegnaValore(numero, codice);
    ottieniNumeroSerial(valore);
    azioniDopoAssegnaValore();
}
void Debug::assegnaValore(int8_t* pointer, int numero, long codice) {
    if(!_usaHardwareSerial) return;
    NumeroQualsiasi valore (tipo_i8);
    azioniPrimaAssegnaValore(numero, codice);
    ottieniNumeroSerial(valore);
    azioniDopoAssegnaValore();
}
void Debug::assegnaValore(uint8_t* pointer, int numero, long codice) {
    if(!_usaHardwareSerial) return;
    NumeroQualsiasi valore (tipo_u8);
    azioniPrimaAssegnaValore(numero, codice);
    ottieniNumeroSerial(valore);
    azioniDopoAssegnaValore();
}
void Debug::assegnaValore(int16_t* pointer, int numero, long codice) {
    if(!_usaHardwareSerial) return;
    NumeroQualsiasi valore (tipo_i16);
    azioniPrimaAssegnaValore(numero, codice);
    ottieniNumeroSerial(valore);
    azioniDopoAssegnaValore();
}
void Debug::assegnaValore(uint16_t* pointer, int numero, long codice) {
    if(!_usaHardwareSerial) return;
    NumeroQualsiasi valore (tipo_u16);
    azioniPrimaAssegnaValore(numero, codice);
    ottieniNumeroSerial(valore);
    azioniDopoAssegnaValore();
}
void Debug::assegnaValore(int32_t* pointer, int numero, long codice) {
    if(!_usaHardwareSerial) return;
    NumeroQualsiasi valore (tipo_i32);
    azioniPrimaAssegnaValore(numero, codice);
    ottieniNumeroSerial(valore);
    azioniDopoAssegnaValore();
}
void Debug::assegnaValore(uint32_t* pointer, int numero, long codice) {
    if(!_usaHardwareSerial) return;
    NumeroQualsiasi valore (tipo_u32);
    azioniPrimaAssegnaValore(numero, codice);
    ottieniNumeroSerial(valore);
    azioniDopoAssegnaValore();
}
void Debug::assegnaValore(float* pointer, int numero, long codice) {
    if(!_usaHardwareSerial) return;
    NumeroQualsiasi valore (tipo_f);
    azioniPrimaAssegnaValore(numero, codice);
    ottieniNumeroSerial(valore);
    azioniDopoAssegnaValore();
}



/**
\brief Questa funzione consente all'utente di inserire un valore nel programma
in esecuzione tramite il monitor seriale.

Il valore inserito può essere di qualsiasi tipo (il tipo viene deciso al momento
della chiamata della funzione, che avviene in una serie di funzioni dedicate
ciascuna a un tipo specifico, vedi sopra).
Se il valore è di un tipo intero questa funzione oltre a controllare, come sempre,
se il valore immesso è valido, controllerà anche che non sia troppo grande o troppo piccolo per il tipo scelto.

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
*/
void Debug::ottieniNumeroSerial(NumeroQualsiasi& risultato) {

    //risultato perziale. Alla fine della funzione il suo valore sarà assegnato
    //a `risultato.valore`
    union {
        bool b;
        float r;
        int64_t z;
    } risultatoParziale;

    //insieme numerico a cui appartiene il numero (più generale del tipo)
    //e condizioni specifiche del tipo
    bool siaPositivo, siaIntero;
    enum {b, n, z, r} insiemeNumerico;

    switch (risultato.tipo) {

        case tipo_bo:
        insiemeNumerico = b;
        break;

        case tipo_u8:
        case tipo_u16:
        case tipo_u32:
        insiemeNumerico = n;
        siaPositivo = true;
        siaIntero = true;
        break;

        case tipo_i8:
        case tipo_i16:
        case tipo_i32:
        insiemeNumerico = z;
        siaPositivo = false;
        siaIntero = true;
        break;

        case tipo_f:
        insiemeNumerico = r;
        siaPositivo = false;
        siaIntero = false;
        break;

        //non dovrebbe mai accadere
        default:
        return;
    }


    //scrivi il tipo richesto sul monitor seriale
    switch (risultato.tipo) {

        case tipo_bo:
        print(S_ASSEGNA_BOOL);
        break;
        case tipo_u8:
        print(S_ASSEGNA_UINT8);
        break;
        case tipo_u16:
        print(S_ASSEGNA_UINT16);
        break;
        case tipo_u32:
        print(S_ASSEGNA_UINT32);
        break;
        case tipo_i8:
        print(S_ASSEGNA_INT8);
        break;
        case tipo_i16:
        print(S_ASSEGNA_INT16);
        break;
        case tipo_i32:
        print(S_ASSEGNA_INT32);
        break;
        case tipo_f:
        print(S_ASSEGNA_FLOAT);
        break;
    }

    print(S_ASSEGNA_POSTFISSO_TIPO);


    //loop ripetuto fino a quando il valore inserito è valido, non eccede i
    // limiti del tipo richiesto ed è confermato dall'utente
    while(true) {

        //### variabili che rappresentazo il numero immesso dall'utente prima
        //dell'elaborazione ###

        //tutti i caratteri ricevuti (10 è il massimo di cifre decimali per un long)
        char testo[10];
        //è intero/positivo? sì fino a prova contraria
        bool intero, positivo;
        //indice dell'ultima cifra (la meno significativa) nell'array
        int indiceUltimaCifra;
        //indice dell'ultima cifra a sinistra della virgola
        int indiceUnita;

        //loop ripetuto fino a quando il valore inserito è valido (ma potrebbe
        //ancora essere troppo grande o troppo piccolo (ad es. un uint8_t con
        //valore 5000 o -4)
        while(true) {

            indiceUltimaCifra = 0;
            intero = true;
            positivo = true;
            indiceUnita = 0;

            //possibili errori nell'immissione dei dati da parte dell'utente
            enum {
                ok,
                carattereNonValido,
                numeroTroppoLungo,
                positivoHaVirgola,
                interoHaPunto
            } errore;
            errore = ok;

            //l'ultimo carattere rivcevuto
            char c;
            //indice a cui inserire il prossimo carattere valido nell'array `testo`
            int indice = 0;


            //aspetta finché arriva qualcosa nel buffer seriale
            while (!available());

            //togli un elemento alla volta dal buffer fino a quando è vuoto
            while (available()) {

                c = read();

                //##################### È RICHIESTO UN BOOL ############################

                if(risultato.tipo == tipo_bo) {

                    if(c == C_IN_ASSEGNA_BOOL_FALSE_1 || c == C_IN_ASSEGNA_BOOL_FALSE_2) {
                        print(S_ASSEGNA_BOOL_FALSE);
                        print("\n");
                        //l'indice è per forza 0, perché se serve un bool il programma
                        // analizza solo il primo carattere
                        testo[0] = '0';
                        break;
                    }
                    else if(c == C_IN_ASSEGNA_BOOL_TRUE_1 || c == C_IN_ASSEGNA_BOOL_TRUE_2) {
                        print(S_ASSEGNA_BOOL_TRUE);
                        print("\n");
                        testo[0] = '1';
                        break;
                    }
                    else {
                        errore = carattereNonValido;
                        break;
                    }

                    //il programma non può arrivare qui

                }//if(tipoNecessario == BOOL)


                //#################### È RICHIESTO UN NUMERO ###########################

                else { // if(tipoNecessario == BOOL)`

                    //5 POSSIBILITÀ

                    //1.1 se il carattere è uno zero iniziale va semplicemente ignorato. Questo non
                    // impedisce di scrivere ad es. 0.1 (si può scrivere comunque 0.1 o .1, sono equivalenti)
                    if (c == '0' && indice == 0) {}
                    //1.2 se il carattere è C_IN_SEPARATORE_MIGLIAIA va ugualmente ignorato (è un segnaposto
                    // per l'utente, che può usarlo a suo piacimento)
                    else if (c == C_IN_SEPARATORE_MIGLIAIA) {}

                    //2. se il carattere è C_IN_PUNTO (il punto) ricordane la posizione
                    // Il punto è valido solo se è unico in tutta la stringa (cioè se non ce
                    // ne sono stati altri prima)
                    else if (c == C_IN_PUNTO && intero) {
                        if(siaIntero) {
                            errore = interoHaPunto;
                            break;
                        }
                        intero = false;
                        //l'indice dell'ultima cifra salvata è uno meno del valore attuale
                        indiceUnita = indice - 1;
                    }

                    //3. se il carattre è C_IN_MENO, ricordalo
                    // Il segno meno è valido solo se è uno solo e in prima posizione
                    else if (c == C_IN_MENO && positivo && indice == 0) {
                        if(siaPositivo) {
                            errore =  positivoHaVirgola;
                            break;
                        }
                        positivo = false;
                    }

                    //4. se il carattere è una cifra aggiungilo alla stringa.
                    else if (('0' <= c && c <= '9')) {
                        //controlla che l'indice non raggiunga2 il valore massimo
                        if (indice == 10) {
                            errore = numeroTroppoLungo;
                            break;
                        }
                        //salva il carattere
                        testo[indice] = c;
                        indice++;
                    }

                    //5. se non è un numero, punto o segno meno segnala il problema ed esci dal loop
                    else {
                        errore = carattereNonValido;
                        break;
                    }

                } //`else` di `if(tipoNecessario == BOOL)`

                //Si arriva qui se il tipo richiesto è non-bool e se finora tutti i
                // caratteri sono validi (questo loop sta analizzando i caratteri uno a uno)

                //lascia tempo al buffer di riempirsi (2 ms bastano anche a 9600 baud)
                delay(2);

            }//while (available())

            //rimuovi tutti i caratteri rimanenti nel buffer seriale
            while (available()) {
                delay(2);
                read();
            }

            //A questo punto ci sono due possibilità:
            // 1. errore == ok: `testo` contiene un'array di valori validi
            // 2. errore != ok: `testo` non ha senso, bisogna chiedere un nuovo input all'utente

            //############################ gestione errori #############################

            switch (errore) {

                case carattereNonValido:
                print(S_ASSEGNA_CHAR_NON_VALIDO);
                continue;

                case numeroTroppoLungo:
                print(S_ASSEGNA_NR_TROPPO_LUNGO);
                continue;

                case interoHaPunto:
                print(S_ASSEGNA_INTERO_HA_PUNTO);
                continue;

                case positivoHaVirgola:
                print(S_ASSEGNA_POSITIVO_HA_VIRGOLA);
                continue;

                case ok:
                //si può continuare
                break;
            }

            //si arriva qui solo se non ci sono errori (gli errori rimandano all'inizio del loop)


            //###################### gestione valori corretti ##########################

            //se il numero è intero, l'unità si trova all'ultimo indice usato (subito dopo l'utilizzo
            // l'indice aumenta, quindi qui bisogna ridurre il numero di 1)
            if (intero) indiceUnita = indice - 1;

            //in ogni caso il numero totale di cifre corrisponde all'ultimo indice (subito dopo l'utilizzo
            // l'indice aumenta, quindi qui bisogna ridurre il numero di 1)
            indiceUltimaCifra = indice - 1;

            //se si arriva qui il numero è valido e registrato nell'array `testo`, e
            // il buffer seriale è vuoto. Si può quindi uscire dal `while(true)`
            break; //da `while(true)`

        }//`while(true)`


        //-------------------------------------------------------------------------
        // seconda parte della funzione: lettura dei caratteri immessi dall'utente
        //-------------------------------------------------------------------------


        //trasforma l'array di char `testo` in un'array di numeri
        for (int i = 0; i < 10; i++) {
            testo[i] -= '0';
        }


        //interpreta le cifre nell'array in modo diverso a seconda del tipo richiesto
        //e le salva provvisoriamente nella variabile adeguata
        switch (insiemeNumerico) {

            //bool --------------------------------------------------------
            case b:
            risultatoParziale.b = testo[0];
            break;

            // integer (signed/unsigned) ---------------------------------
            case n:
            case z:
            for (int i = 0; i <= indiceUltimaCifra; i++) {
                risultatoParziale.z += testo[i] * intPow(10, indiceUltimaCifra - i);
            }
            if (!positivo) risultatoParziale.z = 0 - risultatoParziale.z;
            break;

            //float -------------------------------------------------------
            case r:
            for (int i = 0; i <= indiceUnita; i++) {
                risultatoParziale.r += testo[i] * intPow(10, indiceUnita - i);
            }
            for (int i = indiceUnita + 1; i <= indiceUltimaCifra; i++) {
                risultatoParziale.r += (float) testo[i] / (float) intPow(10, i - indiceUnita);
            }
            if (!positivo) {
                risultatoParziale.r = 0 - risultatoParziale.r;
            }
            break;
        }


        //### se il numero è di un tipo intero controlla che ne rispetti i limiti ###

        if(insiemeNumerico == n || insiemeNumerico == z) {

            //imposta max e min in base al tipo
            int64_t min, max;
            switch(risultato.tipo) {
                case tipo_u8: min = 0; max = 255; break;
                case tipo_u16: min = 0; max = 65535; break;
                case tipo_u32: min = 0; max = 4294967295; break;
                case tipo_i8: min = -128; max = 127; break;
                case tipo_i16: min = -32768; max = 32767; break;
                case tipo_i32: min = -2147483648; max = 2147483647; break;

                //nei due casi seguenti non c'è controllo dei limiti
                case tipo_bo:
                case tipo_f:
                break;
            }
            //controlla i limiti
            if(risultatoParziale.z > max) {
                print(S_PREFISSO_ASSEGNA);
                print(S_ASSEGNA_MAX);
                if(risultato.tipo == tipo_u32) print((uint32_t) max);
                else print((int32_t) max);
                print("\n");
                continue;
            }
            else if (risultatoParziale.z < min){
                print(S_PREFISSO_ASSEGNA);
                print(S_ASSEGNA_MIN);
                if(risultato.tipo == tipo_u32) print((uint32_t) min);
                else print((int32_t) min);
                print("\n");
                continue;
            }
        }


        //### chiede all'utente di confermare il numero immesso ###

        bool conferma = false;

        //manda un messaggio di richiesta conferma
        print(S_PREFISSO_ASSEGNA);
        print(S_ASSEGNA_CHIEDI_CONFERMA);

        //aspetta e interpreta la risposta dell'utente
        while(true) {
            char c = read();
            if(c == C_IN_CONFERMA_SI) {
                conferma = true;
                break;
            }
            if(c == C_IN_CONFERMA_NO) {
                break;
            }
        }
        print("\n");
        if(!conferma) continue;


        //si arriva qui solo se il numero va bene ed è stato confermato
        break; //unico break da questo loop

    }//while(true)


    //salva il risultato
    switch(risultato.tipo) {
        case tipo_bo:
        risultato.valore.b = risultatoParziale.b;
        break;
        case tipo_u8:
        risultato.valore.u8 = (uint8_t)risultatoParziale.z;
        break;
        case tipo_u16:
        risultato.valore.u16 = (uint16_t)risultatoParziale.z;
        break;
        case tipo_u32:
        risultato.valore.u32 = (uint32_t)risultatoParziale.z;
        break;
        case tipo_i8:
        risultato.valore.i8 = (int8_t)risultatoParziale.z;
        break;
        case tipo_i16:
        risultato.valore.i16 = (int16_t)risultatoParziale.z;
        break;
        case tipo_i32:
        risultato.valore.i32 = (int16_t)risultatoParziale.z;
        break;
        case tipo_f:
        risultato.valore.fl = risultatoParziale.r;
        break;
    }

}





/**
\breif Questa funzione è chiamata all'inizio di assegnaValore.

- stampa alcune informazioni prima di chiedere all'utente l'immissione
di un valore;
- accende il led.
*/
void Debug::azioniPrimaAssegnaValore(int numero, long codice) {

    if(_usaHardwareSerial) {

        //accedne il led per un tempo indefinito
        accendiLed(0);


        print("\n"); //salta una riga

        print(millis());   //stampa il tempo
        print(S_SEP_T_NR);

        print(S_ASSEGNA);   //scrivi che è un'assengazione di un valore
        print(numero);     //stampa il nr. che rappresenta questa assegnazione

        if (codice) {
            print(S_SEP_NR_COD);
            print(codice);  //ev. stampa il codice
        }

        print("\n");      //vai a capo

    }
}




/**
\breif Questa funzione è chiamata dopo che un valore è stato immesso, confermato
e salvato.

- stampa alcune informazioni dopo aver ottenuto dall'utente un valore valido;
- spegne il led
*/
void Debug::azioniDopoAssegnaValore() {

    //scrivi l'ora della fine della "pausa"
    print("\n");
    print(millis());
    print(S_SEP_T_NR);
    print(S_FINE_ASSEGNA);
    print("\n");

    spegniLed();
}

#endif
#endif
