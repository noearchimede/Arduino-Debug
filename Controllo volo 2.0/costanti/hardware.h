/**
\file
\brief Questo file contiene informazioni sull'hardware.

Scopo del file: definire delle costanti che rappresentino l'hardware (ad es. nomi dei pin).

*/


/** \defgroup Nomi_motori Nomi motori
    \details In questo programma i motori sono chiamati come i punti cardinali per
    facilitare la visualizzazione della loro posizione fisica (ad es. è evidente
    che Nord sta di fronte a Sud, mentre l'opposizione (equivalente, vedi sotto)
    tra 0 e 2 è più ambigua).
    @{
*/
#define MOTORE_NORD     0
#define MOTORE_EST      1
#define MOTORE_SUD      2
#define MOTORE_OVEST    3

/// @}


//##############################################################################


#include <inttypes.h>

/**
\brief Struct di costanti riguardanti l'hardware.
\details In questa struct sono presenti le costanti che in tutto il resto del programma
rappresenteranno:
- i pin;
-
-
*/
struct Hardware {

    /**
    \brief Costanti che rappresentano _tutti_ i pin del microcontrollore.
    \details La numerazione adottata è quella della scheda Arduino UNO. Questa
    scelta apparentemente insensata è giustificata dal fatto che anche in `Arduino.h`
    si usa questa numerazione
    */
    struct {

        ///pin dei motori, nell'ordine nord-est-sud-ovest (senso orario)
        const uint8_t motori [4] = {3,5,6,11};

        ///pin dell'interrupt dell'accelerometro-giroscopio MUP6050
        const uint8_t interruptMpu = 2;

        ///pin della comunicazione radio
        const uint8_t ricezioneRf = 4;

        ///pin del led di debug
        const uint8_t ledDebug = 13;

        ///pin del led della comunicazione radio
        const uint8_t ledRf = 12;

    } pin;

    /**
    \brief Costanti legate all'MPU6050, tranne i pin a cui è connesso (quelli sono
    insieme alle costanti per gli altri pin)
    */
    struct {

        ///indirizzo dell'accelerometro-giroscopio MUP6050 su bus I2C
        const uint8_t indirizzoMpu = 0x68;

        ///valori per la calibrazione di MPU605. Ottenuti con il programma salvato
        ///tra gli esempi nel file `calibrazione_mpu6050.ino`
        const uint8_t calibrazioneMpu [6] = {220, -4202, 1589, 109, -16, 41}; //MPU6050

    } mpu6050;


};

extern Hardware hardware;
