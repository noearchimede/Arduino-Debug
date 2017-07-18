/** \file
\brief Funzioni vuote

Se si disabilita la classe (`DEBUG_ABILITA` non defiito) al posto delle funzioni
vere saranno chiamati questi "fantasmi". Questo permette di disattivare il sistema
di debug (guadagnando spazio in memoria e tempo nel'esecuzione del programma) senza
dover togliere ogni chiamata alle sue funzioni dal programma.

\note le funzioni acquisite da HardwareSerial (ad es print()) non saranno "disabilitate"
ma non funzioneranno perché non sarà chiamata `HardwareSerial::begin`

\date 17 luglio 2017

*/

#include "Debug.hpp"


#ifndef DEBUG_ABILITA


//il constructor non è toccato dall'impostazione `DEBUG_ABILITA`
void Debug::begin(unsigned long baud, byte config) {}
void Debug::messaggio(int a, long b, bool c) {}
void Debug::errore(int a, long b, bool c) {}
void Debug::erroreFatale(int a, long b) {}
void Debug::breakpoint(int a, long b, unsigned long c)  {}
/* Le funzioni assegnaValore se sono chiamate sono indispensabili per il programma
(il valore della variabile che dovrebbero modificare potrebbe essere assurdo prima
della chiamata). Quindi se si disabilita la classe il compilatore deve impedirne
la chiamata; il modo più semplice per fare questo è eliminarle del tutto. Quindi
non ne esiste una verisone vuota. */
void Debug::controllaLed() {}
void Debug::impostaUsaSerial(bool a) {}
void Debug::impostaStampaMessaggi(bool a) {}
void Debug::impostaStampaMinimo(bool a) {}
void Debug::impostaDurateLuci(int a, int b, int c) {}
void Debug::impostaDurataBuioDopoNotifica(int a) {}
bool Debug::staUsandoSerial() {return false;}

#endif
