/**
\file

\brief Funzioni per l'assegnazione di un valore a una variabile del programma
\date 23 agosto 2017

Il file contiene una classe il cui constructor (l'unica funzione pubblica)
permette di assegnare un valore a una variabile di un qualsiasi tipo aritmetico.
Questa classe è intesa esclusivamente per l'usa da parte della classe Assegna nel
file `Assegna.hpp`, che fornisce un'interfccia più chiara e semplice.
*/


#ifndef assegna_template_hpp
#define assegna_template_hpp

#include "Arduino.h"//per HardwareSerial (nel constructor)
#include "inttypes.h"
#include "Comunicazione.hpp"
#include "Type_test.hpp"


template <typename T>
class Assegna_template {
public:

    Assegna_template(HardwareSerial& hwserial, T* pointer) : _monitor(hwserial) {
        pointerVariabileUtente = pointer;
        eseguiFunzioni();
        };

private:

    //-------------------------------FUNZIONI-----------------------------------

    void eseguiFunzioni();

    ///analizza il tipo dela variabile passata come argomento a `operator()`
    /**\note Se il tipo non è aritmetico in questa funzione si verifica un errore
    di compilazione: `no matching function for call to 'TypeTest::TypeTest(#)'`
    (dove # è il tipo della variabile).*/
    void analizzaTipo();
    ///stampa sul monitor il nome del tipo richiesto
    void stampaTipoRichiesto();
    ///\brief  riceve un valore bool, controlla che sia valido e se si lo salva
    ///come risultato definitivo.
    void salvaInputBool();
    /// riceve un numero, controlla che sia valido e se si lo salva provvisoriamente
    void salvaInputNumerico();
    ///per orgni errore di immisione stampa una stringa di testo specifica
    void stampaErroreImmissione();
    ///traduci l'array di char in un'array di numeri
    void creaArrayCifre();
    ///trasforma l'input in valore numerico di tipo [u]int64_t
    void trasformaTestoInNumero();
    ///\brief controlla che i numeri interi (signed e unsigned) non eccedano i
    // limiti del tipo a cui saranno assegnati. Se eccedono restituisce false.
    bool controllaLimitiInt();
    ///chiedi all'utente di confermare il valore immesso
    bool chiediConferma();
    ///salva il risultato nella variabile passata (tramite pointer) dall'utente
    void impostaVariabileUtente();



    //-------------------------------VARIABILI----------------------------------


    //variabile passata dall'utente a questa classe
    T* pointerVariabileUtente;

    //vero nome di T
    TypeTest::TypeName tipoT;

    //risultato parziale, può essere modificato varie volte da varie funzioni
    struct RisultatoParziale {
        enum Tipo {b,i,f} tipo;
        union {bool valoreBool; int64_t integer; double floating;} valore;
    } risultatoParziale;

    //requisiti dettati dal tipo T
    struct {
        bool valoreBool;
        bool positivo;
        bool intero;
    } requisiti;

    //caratteristiche del valore mano a mano immesso ed analizzato
    struct Input {

        //10 è il numero di cifre decimali del più grande `uint64_t`

        char testo[10];//numero immesso come sequenza di caratteri
        char cifre[10];//numero immesso come sequenza di cifre

        bool intero;//true -> il numero non ha cifre decimali
        bool positivo;//true > è positivo

        //es. per le prossime due variabili:
        // 0123 45: indice nell'array
        // 3814.72 -> indiceUltimaCifra == 5, indiceUnità == 3
        // 5363    -> indiceUltimaCifra == 3, indiceUnità == 3
        int indiceUltimaCifra;//indice della cifra meno significativa, la più s. è a 0
        int indiceUnita;//indice della cifra che rappresenta l'unità

        enum Base {dec = 10, hex = 16, bin = 2} base;

        enum Errore {
            ok,
            carattereNonValido,
            numeroTroppoLungo,
            interoHaPunto,
            positivoHaMeno,
            modalitaHexImpossibile,
            modalitaBinImpossibile
        } errore;

        //imposta tutte le variabili al valore di default
        void reset() {
            memset(testo, 0, sizeof(testo));
            memset(cifre, 0, sizeof(cifre));
            intero = true;
            positivo = true;
            indiceUltimaCifra = 0;
            indiceUnita = 0;
            base = dec;
            errore = ok;
        }

    } input;

    //interfaccia di comunicazione con l'utente
    Comunicazione _monitor;

};




















/**
\file

\brief Implementazione della classe `Assegna`
\date 23 agosto 2017

\note questo file è `#include`d alla fine di Assegna.hpp perché è l'implementazione
di una classe template.
*/



//---------------------------- Funzioni private --------------------------------



template<typename T> void Assegna_template<T>::eseguiFunzioni() {

    analizzaTipo();
    stampaTipoRichiesto();

/**/nonConfermato:/**/

    if(requisiti.valoreBool) {
        salvaInputBool();
    }
    else {

    /**/erroreLimiti:/**/
        salvaInputNumerico();
        creaArrayCifre();
        trasformaTestoInNumero();
        if(requisiti.intero) if(!controllaLimitiInt()) goto erroreLimiti;
    }
    if(!chiediConferma()) goto nonConfermato;
    impostaVariabileUtente();
}





template<typename T> void Assegna_template<T>::analizzaTipo() {

    TypeTest test(*pointerVariabileUtente);

    //salva il nome del tipo
    tipoT = test.name;

    //char --> int8_t
    if(tipoT == TypeTest::TypeName::c) tipoT = TypeTest::TypeName::i8;

    //salva le caratteristiche rilevanti
    requisiti.valoreBool = test.isBool;
    requisiti.positivo = !test.isSigned;
    requisiti.intero = !test.isFloat;

}


///serve per generare un errore di compilazione in `stampaTipoRichiesto()`
static void TypeTest(void) {}

//[u]int64_t non sono considerati validi, generano un err. di compilazione

template<typename T> void Assegna_template<T>::stampaTipoRichiesto() {

    switch (tipoT) {

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

    _monitor.print(":\t");
}



template<typename T> void Assegna_template<T>::salvaInputBool() {

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
            risultatoParziale.tipo = RisultatoParziale::Tipo::b;
            risultatoParziale.valore.valoreBool = false;

            return; //valore valido salvato
        }
        else if(c == '1' || c == 't' || c == 'T') {
            risultatoParziale.tipo = RisultatoParziale::Tipo::b;
            risultatoParziale.valore.valoreBool = true;

            return; //valore valido salvato
        }

        input.errore = Input::Errore::carattereNonValido;
        stampaErroreImmissione();
        //ricomincia salvaInputBool()
    }

}



template<typename T> void Assegna_template<T>::salvaInputNumerico() {

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
                if(requisiti.intero && requisiti.positivo) {
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
template<typename T> void Assegna_template<T>::stampaErroreImmissione() {

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


template<typename T> void Assegna_template<T>::creaArrayCifre() {

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

template<typename T> void Assegna_template<T>::trasformaTestoInNumero() {

    // int (signed/unsigned)
    if(requisiti.intero) {

        risultatoParziale.tipo = RisultatoParziale::Tipo::i;
        risultatoParziale.valore.integer = 0;

        for (int i = 0; i <= input.indiceUltimaCifra; i++) {
            risultatoParziale.valore.integer += \
            input.cifre[i] * intPow(input.base, input.indiceUltimaCifra - i);
        }
        if (!input.positivo) {
            risultatoParziale.valore.integer = -risultatoParziale.valore.integer;
        }
    }

    //float
    else {
        risultatoParziale.tipo = RisultatoParziale::Tipo::f;
        risultatoParziale.valore.floating = 0;

        for (int i = 0; i <= input.indiceUnita; i++) {
            risultatoParziale.valore.floating += \
            input.cifre[i] * intPow(input.base, input.indiceUnita - i);
        }
        for (int i = input.indiceUnita + 1; i <= input.indiceUltimaCifra; i++) {
            risultatoParziale.valore.floating += \
            (double) input.cifre[i] / (double) intPow(input.base, i - input.indiceUnita);
        }
        if (!input.positivo) {
            risultatoParziale.valore.floating = -risultatoParziale.valore.floating;
        }
    }
}



template<typename T> bool Assegna_template<T>::controllaLimitiInt() {
    uint8_t u8max = 255;
    uint16_t u16max = 65535;
    uint32_t u32max = 4294967295;

    //imposta max e min in base al tipo
    int64_t min, max;
    switch(tipoT) {
        case TypeTest::TypeName::u8:  max = u8max;  min = 0; break;
        case TypeTest::TypeName::u16: max = u16max; min = 0; break;
        case TypeTest::TypeName::u32: max = u32max; min = 0; break;

        case TypeTest::TypeName::i8:  max = u8max/2;  min = -(max+1); break;
        case TypeTest::TypeName::i16: max = u16max/2; min = -(max+1); break;
        case TypeTest::TypeName::i32: max = u32max/2; min = -(max+1); break;

    }
    //controlla i limiti
    if(risultatoParziale.valore.integer > max) {
        _monitor.print("max: ");
        //Serial.print non accetta valori più grandi di [u]int32_t. Quindi tutti
        // i limiti possono essere stampati come int32_t tranne uint32_t.
        if(tipoT == TypeTest::TypeName::u32) _monitor.print((uint32_t) max, input.base);
        else _monitor.print((int32_t) max, input.base);
        _monitor.print("\n|\t");
        return false;
    }
    else if (risultatoParziale.valore.integer < min){
        _monitor.print("min: ");
        if(tipoT == TypeTest::TypeName::u32) _monitor.print((uint32_t) min, input.base);
        else _monitor.print((int32_t) min, input.base);
        _monitor.print("\n|\t");
        return false;
    }
    return true;
}


template<typename T> bool Assegna_template<T>::chiediConferma() {

    bool conferma = false;

    //stampa il valore da confermare
    switch(risultatoParziale.tipo) {
      case RisultatoParziale::Tipo::b:
      _monitor.print(risultatoParziale.valore.valoreBool);
      break;
      case RisultatoParziale::Tipo::i:
      if(tipoT == TypeTest::TypeName::u32){
          _monitor.print((uint32_t)risultatoParziale.valore.integer, input.base);
      }
      else {
          _monitor.print((int32_t)risultatoParziale.valore.integer, input.base);
      }
      break;
      case RisultatoParziale::Tipo::f:
      int nrDecimali = input.indiceUltimaCifra - input.indiceUnita;
      _monitor.print(risultatoParziale.valore.floating, nrDecimali);
      break;
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


template<typename T> void Assegna_template<T>::impostaVariabileUtente() {

    switch (risultatoParziale.tipo) {
        case RisultatoParziale::Tipo::b:
        *pointerVariabileUtente = risultatoParziale.valore.valoreBool;
        break;
        case RisultatoParziale::Tipo::i:
        *pointerVariabileUtente = risultatoParziale.valore.integer;
        break;
        case RisultatoParziale::Tipo::f:
        *pointerVariabileUtente = risultatoParziale.valore.floating;
        break;
    }
}




#endif
