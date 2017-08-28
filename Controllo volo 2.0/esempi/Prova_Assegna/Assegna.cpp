/**
\file

\brief Implementazione della classe `Assegna`
\date 23, 28 agosto 2017


*/
#include "Assegna.hpp"



void Assegna::eseguiFunzioni(NomeTipo nomeTipo) {
    
    analizzaTipo(nomeTipo);
    stampaTipoRichiesto();

/**/nonConfermato:/******/

    if(tipo.valoreBool) {
        salvaInputBool();
    }
    else {

/******/erroreLimiti:/**/
        salvaInputNumerico();
        creaArrayCifre();
        trasformaTestoInNumero();
        if(tipo.intero) if(!controllaLimitiInt()) goto erroreLimiti;
    }
    if(!chiediConferma()) goto nonConfermato;

}





void Assegna::analizzaTipo(NomeTipo nomeTipo) {

  tipo.nome = nomeTipo;
  
    switch(tipo.nome) {
      
        case NomeTipo::b:   tipo.impostaProprieta(1,1,1); break;

        case NomeTipo::u8:  tipo.impostaProprieta(0,1,1); break;
        case NomeTipo::u16: tipo.impostaProprieta(0,1,1); break;
        case NomeTipo::u32: tipo.impostaProprieta(0,1,1); break;

        case NomeTipo::i8:  tipo.impostaProprieta(0,0,1); break;
        case NomeTipo::i16: tipo.impostaProprieta(0,0,1); break;
        case NomeTipo::i32: tipo.impostaProprieta(0,0,1); break;

        case NomeTipo::f:   tipo.impostaProprieta(0,0,0); break;
        case NomeTipo::d:   tipo.impostaProprieta(0,0,0); break;
    }
}


void Assegna::stampaTipoRichiesto() {

    switch (tipo.nome) {

        case NomeTipo::b:
        _monitor.print("bool");
        break;

        case NomeTipo::u8:
        _monitor.print("uint8");
        break;
        case NomeTipo::u16:
        _monitor.print("uint16");
        break;
        case NomeTipo::u32:
        _monitor.print("uint32");
        break;

        case NomeTipo::i8:
        _monitor.print("int8");
        break;
        case NomeTipo::i16:
        _monitor.print("int16");
        break;
        case NomeTipo::i32:
        _monitor.print("int32");
        break;

        case NomeTipo::f:
        _monitor.print("float");
        break;
        case NomeTipo::d:
        _monitor.print("double");
        break;
    }

    _monitor.print(":\t");
}



void Assegna::salvaInputBool() {

    while(true) {

        //imposta tutte le variabili di `input` al valore di default
        input.reset();

        //aspetta finché arriva qualcosa nel buffer seriale
        while (!_monitor.available());

        //leggi il primo carattere inviato dal monitor
        char c = _monitor.read();

        if(c == '0' || c == 'f' || c == 'F') {
            //l'indice è per forza 0, perché se serve un bool il programma
            // analizza solo il primo carattere
            risultato.valoreBool = false;

            return; //valore valido salvato
        }
        else if(c == '1' || c == 't' || c == 'T') {
            risultato.valoreBool = true;

            return; //valore valido salvato
        }

        input.errore = Input::Errore::carattereNonValido;
        stampaErroreImmissione();
        //ricomincia salvaInputBool()
    }

}



void Assegna::salvaInputNumerico() {

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
            //    entra in modalità esadecimale. Ovviamente se è preceduto
            //    da un punto non è uno "zero iniziale"
            else if (c == '0' && indice == 0 && input.intero) {
                primoCharZero = true;
            }
            //2.2 se il primo carattere era uno 0 e il secondo è una x entra,
            //    se possibile, in modalità hex. Se il primo non era uno 0
            //    segnla un errore di carattere non valido.
            //    `indice` è 0 perché se il primo char era uno 0 non aumenta
            else if (c == 'x' && indice == 0 && primoCharZero) {
                if(tipo.intero && tipo.positivo) {
                    input.base = Input::Base::hex;
                }
                else {
                    input.errore = Input::Errore::modalitaHexImpossibile;
                    break;
                }
            }

            //3.  se il carattere è una B maiuscola iniziale e il tipo T è
            //    uint8_t entra in modalità BIN.
            else if (c == 'B' && indice == 0) {
                if(tipo.nome == NomeTipo::u8) {
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
                if(tipo.intero) {
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
                if(tipo.positivo) {
                    input.errore = Input::Errore::positivoHaMeno;
                    break;
                }
                input.positivo = false;
            }

            //6.  se il carattere è una cifra aggiungilo alla stringa.
            else if (\
            //## base decimale
            (input.base == Input::Base::dec && '0' <= c && c <= '9') || \
            //## base esadecimale
            (input.base == Input::Base::hex && (('0' <= c && c <= '9') \
            || ('A' <= c && c <= 'F') || ('a' <= c && c <= 'f'))) || \
            //## base binaria
            (input.base == Input::Base::bin && ('0' == c || c == '1')) \
            ) {
                //controlla che l'indice non raggiunga il valore massimo
                if (indice == sizeof(input.testo)/sizeof(input.testo[0])) {
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

        if(input.errore == Input::Errore::ok) {

            //se il numero è intero l'unità si trova all'ultimo indice usato
            // (subito dopo l'utilizzo l'indice aumenta, quindi qui bisogna
            // ridurre il numero di 1)
            if (input.intero) input.indiceUnita = indice - 1;

            //in ogni caso il numero totale di cifre corrisponde all'ultimo
            // indice
            input.indiceUltimaCifra = indice - 1;

            //il numero è valido, pronto per essere letto e trasformato
            return;
        }


        //si arriva qui se nel loop è sato segnalato un errore

        //svuota il buffer
        delay(2);
        while(_monitor.available()) {
            _monitor.read();
            delay(2);
        }

        //segnala all'utente l'errore riscontrato
        stampaErroreImmissione();

        //ricomincia `salvaInputNumerico()`
    }

}




///per orgni errore di immisione stampa una stringa di testo specifica
void Assegna::stampaErroreImmissione() {

    switch (input.errore) {
        case Input::Errore::carattereNonValido:
        _monitor.print("err");
        break;
        case Input::Errore::numeroTroppoLungo:
        _monitor.print("len");
        break;
        case Input::Errore::interoHaPunto:
        _monitor.print("int");
        break;
        case Input::Errore::positivoHaMeno:
        _monitor.print("uns");
        break;
        case Input::Errore::modalitaHexImpossibile:
        _monitor.print("no hex");
        break;
        case Input::Errore::modalitaBinImpossibile:
        _monitor.print("no bin");
        break;
    }

    _monitor.print("\n|\t");

}


void Assegna::creaArrayCifre() {

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

            case 'A': case 'a': input.cifre[i] = 10; break;
            case 'B': case 'b': input.cifre[i] = 11; break;
            case 'C': case 'c': input.cifre[i] = 12; break;
            case 'D': case 'd': input.cifre[i] = 13; break;
            case 'E': case 'e': input.cifre[i] = 14; break;
            case 'F': case 'f': input.cifre[i] = 15; break;

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

void Assegna::trasformaTestoInNumero() {

    // int (signed/unsigned)
    if(tipo.intero) {

        risultato.integer = 0;

        for (int i = 0; i <= input.indiceUltimaCifra; i++) {
            risultato.integer += \
            input.cifre[i] * intPow(input.base, input.indiceUltimaCifra - i);
        }
        if (!input.positivo) {
            risultato.integer = -risultato.integer;
        }
    }

    //float
    else {
        risultato.floating = 0;

        for (int i = 0; i <= input.indiceUnita; i++) {
            risultato.floating += \
            input.cifre[i] * intPow(input.base, input.indiceUnita - i);
        }
        for (int i = input.indiceUnita + 1; i <= input.indiceUltimaCifra; i++) {
            risultato.floating += \
            (double) input.cifre[i] / (double) intPow(input.base, i - input.indiceUnita);
        }
        if (!input.positivo) {
            risultato.floating = -risultato.floating;
        }
    }
}



bool Assegna::controllaLimitiInt() {

  //questa funzione funziona solo per i numeri interi
  if(!tipo.intero) return false;
  
    uint8_t  u8max = 255;
    uint16_t u16max = 65535;
    uint32_t u32max = 4294967295;

    //imposta max e min in base al tipo
    int64_t min, max;
    switch(tipo.nome) {
        case NomeTipo::u8:  max = u8max;  min = 0; break;
        case NomeTipo::u16: max = u16max; min = 0; break;
        case NomeTipo::u32: max = u32max; min = 0; break;

        case NomeTipo::i8:  max = u8max/2;  min = -(max+1); break;
        case NomeTipo::i16: max = u16max/2; min = -(max+1); break;
        case NomeTipo::i32: max = u32max/2; min = -(max+1); break;

    }
    //controlla i limiti
    if(risultato.integer > max) {
        _monitor.print("max: ");
        //Serial.print non accetta valori più grandi di [u]int32_t. Quindi tutti
        // i limiti possono essere stampati come int32_t tranne uint32_t.
        if(tipo.nome == NomeTipo::u32) _monitor.print((uint32_t) max, input.base);
        else _monitor.print((int32_t) max, input.base);
        _monitor.print("\n|\t");
        return false;
    }
    else if (risultato.integer < min){
        _monitor.print("min: ");
        if(tipo.nome == NomeTipo::u32) _monitor.print((uint32_t) min, input.base);
        else _monitor.print((int32_t) min, input.base);
        _monitor.print("\n|\t");
        return false;
    }
    return true;
}




bool Assegna::chiediConferma() {

    bool conferma = false;

    //stampa il valore da confermare
      if(tipo.valoreBool) {
        _monitor.print(risultato.valoreBool);
      }
      else if(tipo.intero) {
          if(tipo.nome == NomeTipo::u32){
              _monitor.print((uint32_t)risultato.integer, input.base);
          }
          else {
              _monitor.print((int32_t)risultato.integer, input.base);
          }
      }
      else {
          int nrDecimali = input.indiceUltimaCifra - input.indiceUnita;
          _monitor.print(risultato.floating, nrDecimali);
      }
      
    _monitor.print(" ");

    //manda un messaggio di richiesta conferma
    _monitor.print("ok?[y/n]");

    //aspetta e interpreta la risposta dell'utente
    while(true) {
        char c = _monitor.read();
        if(c == 'y') {
            _monitor.print("\n");
            conferma = true;
            break;
        }
        if(c == 'n') {
            _monitor.print("\n|\t");
            break;
        }
    }
    return conferma;

}
