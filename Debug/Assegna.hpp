/**
\file

\brief Funzioni per l'assegnazione di un valore a una variabile del programma
\date 23, 28 agosto 2017

Il file contiene una classe il cui constructor (l'unica funzione pubblica)
permette di assegnare un valore a una variabile di un qualsiasi tipo aritmetico.
Questa classe è intesa esclusivamente per l'usa da parte della classe Assegna nel
file `Assegna.hpp`, che fornisce un'interfccia più chiara e semplice.
*/

/**
\class Assegna
\brief Permette di assegnare un valore del tipo corretto a una qualsiasi variabile
aritmetica.

I tipi accettati sono:

 - `bool`
 - `uint8_t`, `uint16_t`, `uint32_t`
 - `int8_t`, `int16_t`, `int32_t`
 - `float`, `double `

Usando una volta questa classe (cioè chiamando il suo `operator =`) in un programma
se ne aumenta considerevolmente la dimensione in memoria. Indicativamente:

Elementi nel programma                       | Memoria necessaria
:--------------------------------------------|-------------------:
 programma vuoto                             |  464 bytes (1%)
 constructor (con Serial.begin())            | 1400 bytes (4%)
 solo una chiamata (assegnaValore(&x))       | 6800 bytes (21%)
 chiamata di tutti gli operator= disponibili | 7100 bytes (22%)


*/

#ifndef assegna_hpp
#define assegna_hpp

#include "Arduino.h"//per HardwareSerial (nel constructor)
#include "inttypes.h"
#include "Comunicazione.hpp"



class Assegna {

    enum NomeTipo {
        b,              //bool
        u8, u16, u32,   //unsigned int
        i8, i16, i32,   //int
        f, d            //float
    };


public:

    Assegna(HardwareSerial& hwserial) : _monitor(hwserial) {}

    void operator()(bool* ptr)     {esegui(NomeTipo::b),   *ptr = risultato.valoreBool;}
    void operator()(uint8_t* ptr)  {esegui(NomeTipo::u8),  *ptr = risultato.integer;}
    void operator()(uint16_t* ptr) {esegui(NomeTipo::u16), *ptr = risultato.integer;}
    void operator()(uint32_t* ptr) {esegui(NomeTipo::u32), *ptr = risultato.integer;}
    void operator()(int8_t* ptr)   {esegui(NomeTipo::i8),  *ptr = risultato.integer;}
    void operator()(int16_t* ptr)  {esegui(NomeTipo::i16), *ptr = risultato.integer;}
    void operator()(int32_t* ptr)  {esegui(NomeTipo::i32), *ptr = risultato.integer;}
    void operator()(char* ptr)     {esegui(NomeTipo::i8),  *ptr = risultato.integer;}
    void operator()(float* ptr)    {esegui(NomeTipo::f),   *ptr = risultato.floating;}
    void operator()(double* ptr)   {esegui(NomeTipo::d),   *ptr = risultato.floating;}


private:

    //risultato parziale, può essere modificato varie volte da varie funzioni
    union {bool valoreBool; int64_t integer; double floating;} risultato;

    //requisiti dettati dal tipo
    struct Tipo {
        //nome esatto
        NomeTipo nome;
        //proprietà
        bool valoreBool;
        bool positivo;
        bool intero;
        //impostazione proprieta
        void impostaProprieta (bool b, bool p, bool i) {valoreBool = b; positivo = p; intero = i;}

    } tipo;

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




    //-------------------------------FUNZIONI-----------------------------------


    //esegue nel corretto ordine tutte le funzioni che seguono
    void esegui(NomeTipo nomeTipo);

    ///analizza il tipo dela variabile passata come argomento a `operator()`
    /**\note Se il tipo non è aritmetico in questa funzione si verifica un errore
    di compilazione: `no matching function for call to 'TypeTest::TypeTest(#)'`
    (dove # è il tipo della variabile).*/
    void analizzaTipo(NomeTipo nomeTipo);
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
    /// limiti del tipo a cui saranno assegnati. Se eccedono restituisce false.
    bool controllaLimitiInt();
    ///chiedi all'utente di confermare il valore immesso
    bool chiediConferma();

};




#endif
