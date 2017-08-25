/**
\file

\brief Implementazione della classe `Assegna`
\date 23 agosto 2017

\note questo file è `#include`d alla fine di Assegna.hpp perché è l'implementazione
di una classe template.
*/

#include "Assegna.hpp"


//---------------------------- Funzioni private --------------------------------



template<typename T> void Assegna<T>::eseguiFunzioni() {

    analizzaTipo();
    stampaTipoRichiesto();
    nonConfermato: //vedi `goto` sotto
    if(requisiti.valoreBool) {
        salvaInputBool();
    }
    else {
        erroreLimiti: //vedi `goto` sotto
        salvaInputNumerico();
        creaArrayCifre();
        trasformaTestoInNumero();
        if(requisiti.intero) if(!controllaLimitiInt()) goto erroreLimiti;
    }
    if(!chiediConferma()) goto nonConfermato;
    impostaVariabileUtente();
}





template<typename T> void Assegna<T>::analizzaTipo() {
    TypeTest test(*pointerVariabileUtente);
    //salva il nome del tipo
    tipoT = test.name;
    //salva le caratteristiche rilevanti
    requisiti.valoreBool = test.isBool;
    requisiti.positivo = test.isInteger;
    requisiti.intero = !test.isFloat;
}


///serve per generare un errore di compilazione in `stampaTipoRichiesto()`
static void TypeTest(void) {}

template<typename T> void Assegna<T>::stampaTipoRichiesto() {
    switch (tipoT) {
        //char sarà considerato un int8_t
        //[u]int64_t non sono considerati validi

        case TypeTest::TypeName::b:
        _monitor.print("bool");
        break;

        case TypeTest::TypeName::u8:
        _monitor.print("uint8");
        break;
        case TypeTest::TypeName::u16:
        _monitor.print("uint16");
        break;
        case TypeTest::TypeName::u32:
        _monitor.print("uint32");
        break;

        case TypeTest::TypeName::i8:
        case TypeTest::TypeName::c:
        _monitor.print("int8");
        break;
        case TypeTest::TypeName::i16:
        _monitor.print("int16");
        break;
        case TypeTest::TypeName::i32:
        _monitor.print("int32");
        break;

        case TypeTest::TypeName::f:
        _monitor.print("float");
        break;
        case TypeTest::TypeName::d:
        _monitor.print("double");
        break;

        case TypeTest::TypeName::u64:
        case TypeTest::TypeName::i64:
        TypeTest();
        break;
    }

    _monitor.print(": ");
}



template<typename T> void Assegna<T>::salvaInputBool() {

    while(true) {

        //imposta tutte le variabili di `input` al valore di default
        input.reset();

        //aspetta finché arriva qualcosa nel buffer seriale
        while (!_monitor.available());

        //leggi il primo carattere inviato dal monitor
        char c = _monitor.read();

        if(c == '0' || c == 'f' || c == 'F') {
            _monitor.print("false");
            _monitor.print("\n");
            //l'indice è per forza 0, perché se serve un bool il programma
            // analizza solo il primo carattere
            risultatoParziale.valore.valoreBool = false;

            return; //valore valido salvato
        }
        else if(c == '1' || c == 't' || c == 'T') {
            _monitor.print("true");
            _monitor.print("\n");
            risultatoParziale.valore.valoreBool = true;

            return; //valore valido salvato
        }

        input.errore = Input::Errore::carattereNonValido;
        stampaErroreImmissione();
        //ricomincia salvaInputBool()
    }

}



template<typename T> void Assegna<T>::salvaInputNumerico() {

    while(true) {

        //imposta tutte le variabili di `input` al valore di default
        input.reset();

        //l'ultimo carattere ricevuto
        char c;
        //indice a cui inserire il prossimo carattere in `input.testo[]`
        int indice = 0;

        //se il primo carattere è uno 0 diventa true. Se è true e il secondo
        // carattere è una x si entra in modalità hex (o si segnala che quesa
        // non è possibile per il tipo richiesto)
        bool primoCharZero = false;

        //aspetta finché arriva qualcosa nel buffer seriale
        while (!_monitor.available());

        //togle un elemento alla volta dal buffer fino a quando è vuoto
        while (_monitor.available()) {

            c = _monitor.read();

            //7 POSSIBILITÀ

            //1.  se il carattere è uno spazio o un apostrofo sarà ignorato
            //    (è un segnaposto per l'utente, che può usarlo a suo
            //    piacimento, di solito per raggruppare le cifre tre a tre)
            if (c == ' ' || c == '\'') {}

            //2.1 se il carattere è uno zero iniziale sarà ignorato, ma il
            //    programma ricorda che se il prossimo carattere è una x si
            //    entra in modalità esadecimale
            else if (c == '0' && indice == 0) {
                primoCharZero = true;
            }
            //2.2 se il primo carattere era uno 0 e il secondo è una x entra,
            //    se possibile, in modalità hex. Se il primo non era uno 0
            //    segnla un errore di carattere non valido.
            //    `indice` è 0 perché se il primo char era uno 0 non aumenta
            else if (c == 'x' && indice == 0) {
                if(!primoCharZero) {
                    input.errore = Input::Errore::carattereNonValido;
                    break;
                }
                else if(requisiti.intero && requisiti.positivo) {
                    input.base = Input::Base::hex;
                }
                else {
                    input.errore = Input::Errore::modalitaHexImpossibile;
                    break;
                }
            }

            //3.  se il carattere è una B maiuscola iniziale e il tipo T è
            //    uint8_t entra in modalità BIN.
            else if (c == 'B') {
                if(tipoT == TypeTest::TypeName::u8) {
                    input.base = Input::Base::bin;
                }
                else {
                    input.errore = Input::Errore::modalitaBinImpossibile;
                    break;
                }
            }

            //4.  se il carattere è un punto o una virgola ricordane la posizione
            //    Il punto è valido solo se è unico in tutta la stringa (cioè se
            //    non ce ne sono stati altri prima)
            else if ((c == '.' || c== ',') && input.intero) {
                if(requisiti.intero) {
                    input.errore = Input::Errore::interoHaPunto;
                    break;
                }
                input.intero = false;
                //l'indice dell'ultima cifra salvata è uno meno del valore attuale
                input.indiceUnita = indice - 1;
            }

            //5.  se il carattre è un segno meno, ricordalo. Il segno meno è
            //    valido solo se è uno solo e in prima posizione.
            else if (c == '-' && input.positivo && indice == 0) {
                if(requisiti.positivo) {
                    input.errore = Input::Errore::positivoHaVirgola;
                    break;
                }
                input.positivo = false;
            }

            //6.  se il carattere è una cifra aggiungilo alla stringa.
            else if (('0' <= c && c <= '9')) {
                //controlla che l'indice non raggiunga il valore massimo
                if (indice == 10) {
                    input.errore = Input::Errore::numeroTroppoLungo;
                    break;
                }
                //salva il carattere
                input.testo[indice] = c;
                indice++;
            }

            //7.  se il carattere immesso non ha attivato nessuna delle opzioni
            //    soprastanti non è valido.
            else {
                input.errore = Input::Errore::carattereNonValido;
                break;
            }

            //Si arriva qui se finora tutti i caratteri sono validi (questo loop
            //sta analizzando i caratteri uno a uno)

            //lascia tempo al buffer di riempirsi
            delay(2);
        }

        if(input.errore = Input::Errore::ok) {

            //se il numero è intero, l'unità si trova all'ultimo indice usato
            // (subito dopo l'utilizzo l'indice aumenta, quindi qui bisogna
            // ridurre il numero di 1)
            if (requisiti.intero) input.indiceUnita = indice - 1;

            //in ogni caso il numero totale di cifre corrisponde all'ultimo
            // indice
            input.indiceUltimaCifra = indice - 1;

            //il numero è valido, pronto per essere letto e trasformato
            return;
        }

        //si arriva qui se nel loop è sato segnalato un errore

        //svuota il buffer
        while(_monitor.available()) {
            delay(2);
            _monitor.read();
        }

        //segnala all'utente l'errore riscontrato
        stampaErroreImmissione();

        //ricomincia `salvaInputNumerico()`
    }

}




///per orgni errore di immisione stampa una stringa di testo specifica
template<typename T> void Assegna<T>::stampaErroreImmissione() {

    switch (input.errore) {
        case Input::Errore::carattereNonValido:
        _monitor.print("err");
        return;
        case Input::Errore::numeroTroppoLungo:
        _monitor.print("len");
        return;
        case Input::Errore::interoHaPunto:
        _monitor.print("int");
        return;
        case Input::Errore::positivoHaMeno:
        _monitor.print("uns");
        return;
        case Input::Errore::modalitaHexImpossibile:
        _monitor.print("hex imp");
        return;
        case Input::Errore::modalitaBinImpossible:
        _monitor.print("bin imp");
        return;
    }

}


template<typename T> void Assegna<T>::creaArrayCifre() {

    int i = 0;
    for (char c : input.testo) {
        switch (c) {

            case '0': input.cifre[i] = 0; break;
            case '1': input.cifre[i] = 1; break;
            case '2': input.cifre[i] = 2; break;
            case '3': input.cifre[i] = 3; break;
            case '4': input.cifre[i] = 4; break;
            case '5': input.cifre[i] = 5; break;
            case '6': input.cifre[i] = 6; break;
            case '7': input.cifre[i] = 7; break;
            case '8': input.cifre[i] = 8; break;
            case '9': input.cifre[i] = 9; break;

            case 'A': input.cifre[i] = 10; break;
            case 'B': input.cifre[i] = 11; break;
            case 'C': input.cifre[i] = 12; break;
            case 'D': input.cifre[i] = 13; break;
            case 'E': input.cifre[i] = 14; break;
            case 'F': input.cifre[i] = 15; break;

        }
        i++;
    }
}


///serve nella funzione `void Assega::trasformaTestoInNumero()``
static inline uint64_t intPow(int base, int exp) {
    uint64_t x = 1;
    for(int i = 0; i < exp; i++) x *= base;
    return x;
}

template<typename T> void Assegna<T>::trasformaTestoInNumero() {
    // int (signed/unsigned)
    if(requisiti.positivo) {

        risultatoParziale.valore.integer = 0;

        for (int i = 0; i <= input.indiceUltimaCifra; i++) {
            risultatoParziale.valore.integer += \
            input.cifra[i] * intPow(10, input.indiceUltimaCifra - i);
        }
        if (!input.positivo) {
            risultatoParziale.valore.integer = -risultatoParziale.valore.integer;
        }
    }

    //float
    else {

        risultatoParziale.valore.floating = 0;

        for (int i = 0; i <= input.indiceUnita; i++) {
            risultatoParziale.valore.floating += \
            input.cifre[i] * intPow(10, input.indiceUnita - i);
        }
        for (int i = input.indiceUnita + 1; i <= input.indiceUltimaCifra; i++) {
            risultatoParziale.valore.floating += \
            (double) input.cifre[i] / (double) intPow(10, i - input.indiceUnita);
        }
        if (!input.positivo) {
            risultatoParziale.valore.floating = -risultatoParziale.valore.floating;
        }
    }
}



template<typename T> bool Assegna<T>::controllaLimitiInt() {
    uint8_t u8max = 255;
    uint16_t u16max = 65535;
    uint32_t u32max = 4294967295;

    //imposta max e min in base al tipo
    int64_t min, max;
    switch(tipoT) {
        case TypeTest::TypeName::u8:  max = u8max;  min = 0; break;
        case TypeTest::TypeName::u16: max = u16max; min = 0; break;
        case TypeTest::TypeName::u32: max = u32max; min = 0; break;

        case TypeTest::TypeName::u8:  max = u8max/2;  min = -(max+1); break;
        case TypeTest::TypeName::u16: max = u16max/2; min = -(max+1); break;
        case TypeTest::TypeName::u32: max = u32max/2; min = -(max+1); break;

    }
    //controlla i limiti
    if(risultatoParziale.valore.integer > max) {
        _monitor.print(" |\t");
        _monitor.print("max: ");
        //Serial.print non accetta valori più grandi di [u]int32_t. Quindi tutti
        // i limiti possono essere stampati come int32_t tranne uint32_t.
        if(tipoT = TypeTest::TypeName::u32) _monitor.print((uint32_t) max);
        else _monitor.print((int32_t) max);
        _monitor.print("\n");
        return false;
    }
    else if (risultatoParziale.valore.integer < min){
        _monitor.print(" |\t");
        _monitor.print("max: ");
        if(tipoT = TypeTest::TypeName::u32) _monitor.print((uint32_t) min);
        else _monitor.print((int32_t) min);
        _monitor.print("\n");
        return false;
    }
    return true;
}


template<typename T> bool Assegna<T>::chiediConferma() {

    bool conferma = false;

    //manda un messaggio di richiesta conferma
    _monitor.print(" |\t");
    _monitor.print("ok?[y/n]");

    //aspetta e interpreta la risposta dell'utente
    while(true) {
        char c = _monitor.read();
        if(c == 'y') {
            conferma = true;
            break;
        }
        if(c == 'n') {
            break;
        }
    }
    _monitor.print("\n");
    return conferma;

}


template<typename T> void Assegna<T>::impostaVariabileUtente() {

    switch (risultatoParziale.tipo) {
        case RisultatoParziale::tipo::b:
        *pointerVariabileUtente = risultatoParziale.valore.valoreBool;
        break;
        case RisultatoParziale::tipo::i:
        *pointerVariabileUtente = risultatoParziale.valore.integer;
        break;
        case RisultatoParziale::tipo::f:
        *pointerVariabileUtente = risultatoParziale.valore.floating;
        break;
    }
}
