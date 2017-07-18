/**
\file

\brief impostazioni globali per la classe Debug

Questo file permette di configurare la classe Debug prima della compilazione, in
modo che ciò che non serve non sia compilato e quindi caricato inutilmente nella
ristretta memoria del microcontrollore.

\date 17 luglio 2017
*/

/** \name Impostazioni generali

Non esistono altre impostazioni oltre a quelle qui presenti; per disattivarne una
metterla come commento (se la si cancella poi si rischia di dimenticarla)

@{ */

/**
\brief abilita la classe in generale.
\details Se disabilitata le sue funzioni esisteranno ma saranno vuote
*/
#define DEBUG_ABILITA //commenta questa linea per cambiare impostazioni
/*
*/
#ifndef DEBUG_ABILITA
#warning "La classe Debug è disabilitata. Tutte le sue funzioni sono vuote."
#endif

/**
\brief la classe può usare HardwareSerial
\details Non ha senso usare questa classe, che eredita HardwareSerial, senza
permetterle di usare la comunicazione seriale. Questa impostazione però può
servire ad es. per brevi test del programma senza i rallentamenti dovuti
all'utilizzo di serial.
*/
#define DEBUG_ABILITA_SERIAL //commenta questa linea per cambiare impostazioni
/*
*/
#ifndef DEBUG_ABILITA_SERIAL
#warning "La classe Debug non può utilizzare comunicazione seriale."
#endif

/**
\brief Abilita la funzione di assegnazione di un valore a una variabile nel programma.
\note Se questa impostazione è commentata la funzione non esisterà (quindi non si
potrà usarla nel codice)
*/
#define DEBUG_ABILITA_ASSEGNA //commenta questa linea per cambiare impostazioni
/*
*/
#ifndef DEBUG_ABILITA_ASSEGNA
#warning "Le funzioni di assegnazione valore della classe Debug non saranno compilate"
#endif

/**
\brief Abilita la funzione di assegnazione di un valore a una variabile nel programma.
\note Se questa impostazione è commentata la funzione esisterà ma sarà vuota (quindi
la si potrà chiamare nel programma ma non si otterrà nessun effetto)
*/
#define DEBUG_ABILITA_BREAKPOINT //commenta questa linea per cambiare impostazioni
/*
*/
#ifndef DEBUG_ABILITA_BREAKPOINT
#warning "I breakpoint della classe Debug sono disabilitati (sono funzioni vuote)"
#endif

/// @}



/// \name Valore default di impostazioni modificabili nel programma
/// @{

///\brief La classe può usare la porta seriale?
///\details Da usare solo localmente; se si vuole disattivare Serial globalmente
///commentare DEBUG_ABILITA_SERIAL nelle impostazioni (si può usare anche questa
///impostazione "locale", ma usando quella globale si risparmia memoria su Arduino)
#define DEBUG_DEFAULT_USA_SERIAL               true

///\brief la classe ha a disposizione un led? Di solito si, collegato al pin 13.
///\details Se si imposta `false` la classe eseguirà comunque tutte le funzioni legate al
///LED, ma non toccherà il pin. DEBUG_PIN_LED quindi può avere qualsiasi valore (non sarà
///mai usato)
#define DEBUG_DEFAULT_USA_LED                  false
///Pin del LED
#define DEBUG_DEFAULT_PIN_LED                  13

///Abilita i breakpoints?
#define DEBUG_DEFAULT_CONSENTI_BREAKPOINT       true        //default true
///imposta di default un timeout per i breakpoints
#define DEBUG_DEFAULTA_USA_TIMEOUT_BREAK        false       //default false
///\brief Tempo di attesa massimo in un breakpoint, dopo il quale il programma riprende
///\note Valore massimo: 1 minuto (60000)
#define DEBUG_DEFAULT_TIMEOUT_BREKPOINT         5000        //default 5000

///Stampa anche i messaggi (`true`) o solo gli errori?
#define DEBUG_DEFAULT_STAMPA_MESS               true        //default true
///Stampa solo il minimo indispensabille (`true`) o produci un output più leggibile?
#define DEBUG_DEFAULT_STAMPA_MINIMO             false       //default false
///\brief Ignora il codice eventualmente associato alle notifiche
///\warning ha effetto solo se anche `DEBUG_DEFAULT_STAMPA_MINIMO` è `true`
#define DEBUG_DEFAULT_IGNORA_CODICE             false       //default false

///Dopo ogni notifica aspetta che il led sia spento prima di proseguire
#define DEBUG_DEFAULT_ASPETTA_FINE_NOTIFICA     false       //default false
///Se `DEBUG_ASPETTA_FINE_NOTIFICA`è `true`, questo è il tempo minimo tra due segnali LED
#define DEBUG_DEFAULT_DURATA_BUIO_DOPO_NOTIFICA 40          //default 40

///Tempo per cui il LED resta acceso per ogni messaggio (in millisecondi)
#define DEBUG_DEFAULT_LUCE_MESS                 50          //default 50
///Tempo per cui il LED resta acceso per ogni errore (in millisecondi)
#define DEBUG_DEFAULT_LUCE_ERR                  300         //default 300
///Durata della luce (e del "buio") nel lampeggiamento in caso di errore fatale
#define DEBUG_DEFAULT_LUCE_ERRFAT               1500        //default 1500



/// @}


#if DEBUG_DEFAULT_TIMEOUT_BREKPOINT > 60000
#error "DEBUG_DEFAULT_TIMEOUT_BREKPOINT deve essere minore o uguale a 1 min (60000 ms)"
#endif

#if DEBUG_DEFAULT_IGNORA_CODICE == true && DEBUG_DEFAULT_STAMPA_MINIMO == false
#error "DEBUG_DEFAULT_IGNORA_CODICE ha effetto solo se anche DEBUG_DEFAULT_STAMPA_MINIMO è impostato"
#endif
