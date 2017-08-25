/**
\file

\brief Funzioni per l'assegnazione di un valore a una variabile del programma
\date 23 agosto 2017

Il file contiene un "functor" (una classe per cui è definita l'operazione ()) che
permette di assegnare un valore a una variabile di un qualsiasi tipo aritmetico.

*/

#ifndef assegna_hpp
#define assegna_hpp

#include "inttypes.h"
#include "Comunicazione.hpp"
#include "Type_test.hpp"


template <typename T>
class Assegna {
public:

    Assegna(HardwareSerial& hwserial);

    void operator() (T* pointer) {
        pointerVariabileUtente = pointer;
        eseguiFunzioni();
    }

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
        enum tipo {b,i,f};
        union {bool valoreBool; uint64_t integer; double floating;} valore;
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
            modalitaBinImpossible
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




#endif
