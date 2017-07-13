/**
\file
\brief Implementazione minima di setup() e loop(), richiesta dal framework Arduino
*/

//forward declaration della "vera" funzione main di questo programma (uso questo
//nome perché `main()` è già usato da Arduino)
void controlloVolo(void);

void setup () {
    controlloVolo();
}

void loop () {}
