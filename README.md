# TriQuiz - Quiz a risposta multipla in C++

TriQuiz e' un'applicazione console scritta in C++ per gestire e giocare un quiz a risposta multipla.  
Il progetto usa domande divise per categoria, una classifica dei giocatori e un menu interattivo con colori per rendere il programma piu' chiaro da usare.

## Descrizione

Il programma permette all'utente di giocare a un quiz scegliendo una categoria tra:

- Scienza
- Storia
- Tecnologia
- Tutte le categorie

Le domande vengono caricate da un file esterno chiamato `questions.txt`.  
Alla fine della partita il programma aggiorna la classifica del giocatore.

## Funzionalita principali

- Menu interattivo in console
- Colori ANSI per migliorare la grafica del programma
- Caricamento delle domande da file esterno
- Categorie per le domande: Scienza, Storia e Tecnologia
- Risposte multiple con opzioni A, B, C e D
- Timer di 30 secondi per ogni domanda
- Risposta corretta: +10 punti
- Tempo scaduto: risposta considerata sbagliata e -5 punti
- Il punteggio non puo' andare sotto zero
- Classifica dei giocatori salvata su file
- CRUD completo sui punteggi:
  - aggiunta punteggio
  - ricerca giocatore
  - modifica punteggio
  - eliminazione punteggio
- Ordinamento della classifica con Bubble Sort
- Ricerca di un giocatore con Binary Search
- Possibilita' di aggiungere nuove domande dal menu

## Argomenti di programmazione usati

Nel progetto sono stati usati diversi argomenti studiati nel primo anno di programmazione:

- `struct`
- array
- funzioni con parametri
- passaggio per riferimento
- cicli `for` e `while`
- istruzioni `if`, `else` e `switch`
- gestione dell'input utente
- file di testo con `ifstream` e `ofstream`
- Bubble Sort
- Binary Search
- numeri casuali con `rand()`
- timer con `time()`

## File del progetto

- `triquiz.cpp`  
  File principale con tutto il codice C++ del programma.

- `questions.txt`  
  File esterno che contiene le domande del quiz.

- `triquiz.exe`  
  File eseguibile generato dopo la compilazione.
  

## Come avviare il programma

1. Scarica i file `triquiz.cpp` e `questions.txt`
2. Metti i due file nella stessa cartella
3. Compila il programma con un compilatore C++, per esempio:

```powershell
g++ triquiz.cpp -o TriQuiz
```

4. Esegui il file generato:

```powershell
.\TriQuiz.exe
```

5. Inserisci il nome del giocatore, scegli una categoria e divertiti con il quiz!
