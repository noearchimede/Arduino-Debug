/**
\file

Questo file contiene tutte le stringhe di testo che possono essere mostrate sul
monitor seriale.

Il prefisso S_ significa String e segna le stringhe di testo da stampare sul
monitor seriale.
Il prefisso C_IN_ significa Char ricevuto come input e rappresenta i caratteri
inviati dall'utente tramite il monitor seriale.

\note è un file riservato alla classe Debug, quindi per non inqunare lo spazio
globale includerlo solo dai file sorgente

*/

#define S_SEP_T_NR                  "\t"
#define S_SEP_NR_COD                ":"

#define S_MESS                      "\t"
#define S_ERR_MIN                   "E"
#define S_ERR                       "err\t"
#define S_ERRFAT                    "err\t"
#define S_BREAK                     "break\t"
#define S_FINE_BREAK                "fine break"
#define S_ASSEGNA                   "assegn\t"
#define S_FINE_ASSEGNA              "fine assegn"

// S_ERRFAT_PRIMA_DI_TEMPO[millis dell'errore]S_ERRFAT_DOPO_TEMPO
#define S_ERRFAT_PRIMA_DI_TEMPO     "Errore fatale a "
#define S_ERRFAT_DOPO_TEMPO         " ms:\n"

#define S_NR_PUNTI_LINEA_TIMEOUT    20
#define S_CHAR_MODELLO_TIMEOUT      "."
#define S_CHAR_LINEA_TIMEOUT        "\'"

#define S_NR_PUNTI_LINEA_ERRFAT     30
#define S_CHAR_LINEA_ERRFAT         "-"


#define S_ASSEGNA_ULONG             "uint: "
#define S_ASSEGNA_LONG              "int: "
#define S_ASSEGNA_FLOAT             "float: "
#define S_ASSEGNA_BOOL              "bool: "
#define S_ASSEGNA_CHAR_NON_VALIDO   "err"
#define S_ASSEGNA_NR_TROPPO_LUNGO   "l"
#define S_ASSEGNA_BOOL_FALSE        "false"
#define S_ASSEGNA_BOOL_TRUE         "true"
#define S_ASSEGNA_FUORI_LIMITI      "lim"
#define S_ASSEGNA_CHIEDI_CONFERMA   "ok?[y/n]"


//'\0' per consentire qualsiasi carattere, un altro carattere per richiedere quello
#define C_IN_SBLOCCA_BREAKPOINT     '\0'

#define C_IN_ASSEGNA_BOOL_TRUE_1    '1'
#define C_IN_ASSEGNA_BOOL_TRUE_2    't'
#define C_IN_ASSEGNA_BOOL_FALSE_1   '0'
#define C_IN_ASSEGNA_BOOL_FALSE_2   'f'

#define C_IN_SEPARATORE_MIGLIAIA    '\''
#define C_IN_PUNTO                  '.'
#define C_IN_MENO                   '-'

#define C_IN_CONFERMA_SI            'y'
#define C_IN_CONFERMA_NO            'n'
