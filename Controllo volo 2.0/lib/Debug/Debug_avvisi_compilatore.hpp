/**\file
\brief Avvisi del precompiler riguardo a impostazioni impossibili o che possono
portare a comportamenti inaspettati.

\warning La lista non è esaustiva!

\date 20 luglio 2017
*/


#include "Debug_impostazioni.hpp"


#ifndef DEBUG_ABILITA
#warning "La classe Debug è disabilitata. Tutte le sue funzioni sono vuote."
#endif

#ifndef DEBUG_ABILITA_BREAKPOINT
#warning "I breakpoint della classe Debug sono disabilitati (sono funzioni vuote)"
#endif

#ifndef DEBUG_ABILITA_ASSEGNA
#warning "Le funzioni di assegnazione valore della classe Debug non saranno compilate"
#endif

#if DEBUG_DEFAULT_TIMEOUT_BREKPOINT > 60000
#error "DEBUG_DEFAULT_TIMEOUT_BREKPOINT deve essere minore o uguale a 1 min (60000 ms)"
#endif

#if DEBUG_DEFAULT_IGNORA_CODICE == true && DEBUG_DEFAULT_STAMPA_MINIMO == false
#error "DEBUG_DEFAULT_IGNORA_CODICE ha effetto solo se anche DEBUG_DEFAULT_STAMPA_MINIMO è impostato"
#endif

#ifndef DEBUG_ABILITA_INTERRUPT
#if DEBUG_DEFAULT_USA_LED == true
#warning "La clase Debug non ha a disposizione un interrupt per spegnere il LED.\
Sarà necessario chiamare regolarmente controllaLed()"
#endif
#else //DEBUG_ABILITA_INTERRUPT
#warning "La classe Debug usa l'interrupt TIMER0_COMPA per controllare il LED"
#endif
