***Attenzione: principalmente un esercizio di programmazione, usare questa libreria non è il modo ottimale per produrre log su un programma Arduino***


Arduino Debug
=============

Questa libreria serve a semplificare il debug di qualsiasi programma per le
piattaforme Arduino.

La libreria offre:

- un sistema di log su più livelli (errori, avvisi, informazioni)
- un sistema di debug che consente di:
  -  inserire breakpoints che possono essere "sbloccati" inviando un carattere
  qualsiasi tramite un monitor seriale;
  -  inserire breakpoints con una durata predeterminata, che se non sbloccati
  tramite il monitor seriale si sbloccano automaticamente allo scadere del tempo
  -  bloccare irrimediabilmente il programma in caso di errore fatale
  -  impostare tramite il monitor seriale il valore di variabili di qualsiasi
  tipo aritmetico (bool, int e uint di qualsiasi dimensione, float/double)


Per una descrizione dettagliate dell'interfaccia vedi la pagina Debug.

---

**Utilitzzo delle funzioni di logging**


Le funzioni `info`, `warn` e `err` stampano dei messaggi sul monitor seriale
e fanno accendere un led per un tempo che dipende dal tipo di messaggio.
L'unica differenza tra di esse è il livello di importanza: i messaggi info
possono essere disabilitati lasciando warn ed err, e warn lasciando err.
Inoltre i messaggi err sono visivamente in rilievo nell'output.

Ogni messaggi ha un nome o un codice. Il nome è una stringa di testo, il codice
è un numero che rappresenta univocamente il messaggio. Conviene usare il
nome se si ha abbastanza memoria a disposizione e altrimenti il codie, che
evidentemente rende più impegnativo leggere il file di log.
Inoltre a ogni messaggio è possibile associare un valore di qualsiasi tipo
aritmetico (bool, intero con o senza segno o decimale), che sarà stampato
accanto al nome.

Scelta delle funzioni:
- `info(...)`: informazione sul corretto svolgimento del programma, es.:
    svolto un certo calcolo, nuovo sensore connesso, ...
- `warn(...)`: avviso su un potenziale problema che non dovrebbe accadere
    ma non compromette irreversibilmente lo svolgimento del programma, es.:
    tentativo di connessione a un sensore già connesso, temperatura: -400°C
- `err(...)`: errore, cioè avvenimento indesiderato che compromette, o
    potrebbe verosimilmente compromettere il programma, es.: myFloat è nan,
    divisione per 0
