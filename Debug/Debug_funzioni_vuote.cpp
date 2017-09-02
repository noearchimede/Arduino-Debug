/** \file
\brief Funzioni vuote

Se si disabilita la classe (definendo `DEBUG_DISABILITA`) al posto delle funzioni
vere saranno chiamati questi "fantasmi". Questo permette di disattivare il sistema
di debug (guadagnando spazio in memoria e tempo nel'esecuzione del programma) senza
dover togliere ogni chiamata alle sue funzioni dal programma.

\date 17 luglio 2017, 19 agosto 2017

*/


#include "Debug.hpp"


#ifdef DEBUG_DISABILITA


//il constructor non è toccato dall'impostazione `DEBUG_ABILITA`
void Debug::begin(long baudSerial) {}
void Debug::begin(long baudSerial, int pinLed) {}

void Debug::controllaLed() {}

void Debug::info(int codice) {}
template <typename T> void Debug::info(int codice, T val) {}
void Debug::info(String &nome) {}
template <typename T> void Debug::info(String &nome, T val) {}

void Debug::warn(int codice) {}
template <typename T> void Debug::warn(int codice, T val) {}
void Debug::warn(String &nome) {}
template <typename T> void Debug::warn(String &nome, T val) {}

void Debug::err(int codice) {}
template <typename T> void Debug::err(int codice, T val) {}
void Debug::err(String &nome) {}
template <typename T> void Debug::err(String &nome, T val) {}

void Debug::errFat() {}

void Debug::breakpoint() {}
void Debug::breakpoint(unsigned long attesaMassima) {}


/* Le funzioni assegnaValore se sono chiamate sono indispensabili per il programma
(il valore della variabile che dovrebbero modificare potrebbe essere assurdo prima
della chiamata). Quindi se si disabilita la classe il compilatore deve impedirne
la chiamata; il modo più semplice per fare questo è eliminarle del tutto. Per
questo non ne esiste una verisone vuota. */

void Debug::abilitaComunicazione(bool x) {}
void Debug::abilitaLed(bool x) {}
void Debug::impostaLivello(Debug::Livello x) {}
void Debug::verbose(bool x) {}
void Debug::ignoraVal(bool x) {}
void Debug::aspettaFineNotica(bool x) {}
void Debug::impostaPinLed(byte x) {}
void Debug::abilitaInterrupt(bool x) {}
void Debug::impostaDurataLuce(unsigned long informazione, unsigned long avviso, unsigned long errore, unsigned long erroreFatale) {}
void Debug::impostaBaudComunicazione(long x) {}



#endif
