#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <string>
#include <conio.h>

using namespace std;

// Colori ANSI per rendere la console piu' ordinata
#define RESET  "\033[0m"
#define ROSSO  "\033[31m"
#define VERDE  "\033[32m"
#define GIALLO "\033[33m"
#define BLU    "\033[34m"
#define VIOLA  "\033[35m"
#define CYAN   "\033[36m"

const int MAX_DOMANDE = 100;
const int MAX_PUNTEGGI = 100;
const int TEMPO_RISPOSTA = 30;
const int PUNTI_CORRETTA = 10;
const int PENALITA_TEMPO = 5;

const string FILE_DOMANDE = "questions.txt";
const string FILE_DOMANDE_BACKUP = "../questions.txt";
const string FILE_PUNTEGGI = "scores.txt";

struct Domanda {
    string categoria;
    string testo;
    string opzioneA;
    string opzioneB;
    string opzioneC;
    string opzioneD;
    char rispostaEsatta;
};

struct Punteggio {
    string nome;
    int punti;
};

void pulisciInput()
{
    cin.clear();
    cin.ignore(10000, '\n');
}

int leggiIntero(string messaggio)
{
    int valore;

    cout << messaggio;

    while (!(cin >> valore)) {
        cout << ROSSO << "Inserisci un numero valido: " << RESET;
        pulisciInput();
    }

    pulisciInput();
    return valore;
}

char leggiCarattere(string messaggio)
{
    char scelta;

    cout << messaggio;
    cin >> scelta;
    pulisciInput();

    return (char)toupper((unsigned char)scelta);
}

bool leggiRispostaTimer(char &risposta)
{
    time_t inizio = time(NULL);

    cout << CYAN << "Risposta (A/B/C/D - " << TEMPO_RISPOSTA
         << " secondi): " << RESET;

    while (time(NULL) - inizio < TEMPO_RISPOSTA) {
        if (_kbhit()) {
            risposta = (char)_getch();
            risposta = (char)toupper((unsigned char)risposta);

            if (risposta == 'A' || risposta == 'B' ||
                risposta == 'C' || risposta == 'D') {
                cout << risposta << endl;
                return true;
            }
        }
    }

    cout << endl;
    return false;
}

string testoCategoria(string categoria)
{
    if (categoria == "Science")
        return "Scienza";
    else if (categoria == "History")
        return "Storia";
    else if (categoria == "Technology")
        return "Tecnologia";
    else
        return "Tutte";
}

int dividiRiga(string riga, string campi[], char separatore)
{
    int posizione = 0;
    string pezzo = "";

    for (int i = 0; i < (int)riga.length(); i++) {
        if (riga[i] == separatore) {
            if (posizione < 7)
                campi[posizione] = pezzo;

            posizione++;
            pezzo = "";
        } else {
            pezzo += riga[i];
        }
    }

    if (posizione < 7)
        campi[posizione] = pezzo;

    return posizione + 1;
}

bool aggiungiDomandaArray(Domanda elencoDomande[], int &numeroDomande,
                          string categoria, string testo, string A, string B,
                          string C, string D, char risposta)
{
    risposta = (char)toupper((unsigned char)risposta);

    if (numeroDomande >= MAX_DOMANDE)
        return false;

    if (categoria == "" || testo == "" || A == "" || B == "" || C == "" || D == "")
        return false;

    if (risposta != 'A' && risposta != 'B' && risposta != 'C' && risposta != 'D')
        return false;

    elencoDomande[numeroDomande].categoria = categoria;
    elencoDomande[numeroDomande].testo = testo;
    elencoDomande[numeroDomande].opzioneA = A;
    elencoDomande[numeroDomande].opzioneB = B;
    elencoDomande[numeroDomande].opzioneC = C;
    elencoDomande[numeroDomande].opzioneD = D;
    elencoDomande[numeroDomande].rispostaEsatta = risposta;
    numeroDomande++;

    return true;
}

bool apriFileDomande(ifstream &file)
{
    file.open(FILE_DOMANDE.c_str());

    if (file.is_open())
        return true;

    file.clear();
    file.open(FILE_DOMANDE_BACKUP.c_str());

    return file.is_open();
}

int caricaDomande(Domanda elencoDomande[], int &numeroDomande)
{
    ifstream file;
    string riga;
    int righeSbagliate = 0;

    numeroDomande = 0;

    if (!apriFileDomande(file)) {
        cout << ROSSO << "File questions.txt non trovato." << RESET << endl;
        cout << GIALLO << "Mettilo nella stessa cartella del programma." << RESET << endl;
        return 0;
    }

    while (getline(file, riga)) {
        if (riga == "" || riga[0] == '#')
            continue;

        if (riga[riga.length() - 1] == '\r')
            riga.erase(riga.length() - 1);

        string campi[7];
        int quantiCampi = dividiRiga(riga, campi, '|');

        if (quantiCampi == 7) {
            bool inserita = aggiungiDomandaArray(elencoDomande, numeroDomande,
                                                 campi[0], campi[1], campi[2],
                                                 campi[3], campi[4], campi[5],
                                                 campi[6][0]);

            if (!inserita)
                righeSbagliate++;
        } else {
            righeSbagliate++;
        }
    }

    file.close();

    if (righeSbagliate > 0)
        cout << GIALLO << "Righe non valide nel file domande: "
             << righeSbagliate << RESET << endl;

    return numeroDomande;
}

void salvaDomande(Domanda elencoDomande[], int numeroDomande)
{
    ofstream file(FILE_DOMANDE.c_str());

    if (!file.is_open()) {
        cout << ROSSO << "Errore durante il salvataggio delle domande." << RESET << endl;
        return;
    }

    for (int i = 0; i < numeroDomande; i++) {
        file << elencoDomande[i].categoria << "|"
             << elencoDomande[i].testo << "|"
             << elencoDomande[i].opzioneA << "|"
             << elencoDomande[i].opzioneB << "|"
             << elencoDomande[i].opzioneC << "|"
             << elencoDomande[i].opzioneD << "|"
             << elencoDomande[i].rispostaEsatta << endl;
    }

    file.close();
}

void caricaPunteggi(Punteggio classifica[], int &numeroPunteggi)
{
    ifstream file(FILE_PUNTEGGI.c_str());
    string nome;
    int punti;

    numeroPunteggi = 0;

    if (!file.is_open())
        return;

    while (file >> nome >> punti && numeroPunteggi < MAX_PUNTEGGI) {
        classifica[numeroPunteggi].nome = nome;
        classifica[numeroPunteggi].punti = punti;
        numeroPunteggi++;
    }

    file.close();
}

void salvaPunteggi(Punteggio classifica[], int numeroPunteggi)
{
    ofstream file(FILE_PUNTEGGI.c_str());

    if (!file.is_open()) {
        cout << ROSSO << "Errore durante il salvataggio dei punteggi." << RESET << endl;
        return;
    }

    for (int i = 0; i < numeroPunteggi; i++)
        file << classifica[i].nome << " " << classifica[i].punti << endl;

    file.close();
}

void scambiaPunteggi(Punteggio &a, Punteggio &b)
{
    Punteggio temp = a;
    a = b;
    b = temp;
}

void ordinaPunteggiPerPunti(Punteggio classifica[], int numeroPunteggi)
{
    // Bubble Sort decrescente: il punteggio piu' alto va in alto.
    for (int i = 0; i < numeroPunteggi - 1; i++) {
        for (int j = 0; j < numeroPunteggi - 1 - i; j++) {
            if (classifica[j].punti < classifica[j + 1].punti)
                scambiaPunteggi(classifica[j], classifica[j + 1]);
        }
    }
}

void ordinaPunteggiPerNome(Punteggio classifica[], int numeroPunteggi)
{
    // La Binary Search funziona solo se i nomi sono ordinati.
    for (int i = 0; i < numeroPunteggi - 1; i++) {
        for (int j = 0; j < numeroPunteggi - 1 - i; j++) {
            if (classifica[j].nome > classifica[j + 1].nome)
                scambiaPunteggi(classifica[j], classifica[j + 1]);
        }
    }
}

int cercaGiocatoreBinaria(Punteggio classifica[], int numeroPunteggi, string nome)
{
    int sinistra = 0;
    int destra = numeroPunteggi - 1;

    while (sinistra <= destra) {
        int centro = (sinistra + destra) / 2;

        if (classifica[centro].nome == nome)
            return centro;

        if (classifica[centro].nome < nome)
            sinistra = centro + 1;
        else
            destra = centro - 1;
    }

    return -1;
}

void mostraClassifica(Punteggio classifica[], int numeroPunteggi)
{
    ordinaPunteggiPerPunti(classifica, numeroPunteggi);

    cout << "\n" << CYAN << "-------- CLASSIFICA --------" << RESET << endl;

    if (numeroPunteggi == 0) {
        cout << GIALLO << "Nessun punteggio salvato." << RESET << endl;
        return;
    }

    cout << VIOLA << "Pos\tNome\t\tPunti" << RESET << endl;

    for (int i = 0; i < numeroPunteggi; i++) {
        cout << i + 1 << "\t"
             << classifica[i].nome << "\t\t"
             << classifica[i].punti << endl;
    }
}

void cercaGiocatore(Punteggio classifica[], int numeroPunteggi)
{
    string nome;

    cout << "\nNome giocatore da cercare: ";
    cin >> nome;
    pulisciInput();

    ordinaPunteggiPerNome(classifica, numeroPunteggi);

    int posizione = cercaGiocatoreBinaria(classifica, numeroPunteggi, nome);

    if (posizione == -1) {
        cout << ROSSO << "Giocatore non trovato." << RESET << endl;
    } else {
        cout << VERDE << "Giocatore trovato." << RESET << endl;
        cout << "Nome: " << classifica[posizione].nome << endl;
        cout << "Punti: " << classifica[posizione].punti << endl;
    }
}

void inserisciPunteggioManuale(Punteggio classifica[], int &numeroPunteggi)
{
    string nome;
    int punti;

    if (numeroPunteggi >= MAX_PUNTEGGI) {
        cout << ROSSO << "Non c'e' spazio per altri punteggi." << RESET << endl;
        return;
    }

    cout << "\nNome giocatore: ";
    cin >> nome;
    pulisciInput();

    ordinaPunteggiPerNome(classifica, numeroPunteggi);

    if (cercaGiocatoreBinaria(classifica, numeroPunteggi, nome) != -1) {
        cout << ROSSO << "Questo nome e' gia' presente." << RESET << endl;
        return;
    }

    punti = leggiIntero("Punti: ");

    if (punti < 0)
        punti = 0;

    classifica[numeroPunteggi].nome = nome;
    classifica[numeroPunteggi].punti = punti;
    numeroPunteggi++;

    salvaPunteggi(classifica, numeroPunteggi);
    cout << VERDE << "Punteggio inserito." << RESET << endl;
}

void modificaPunteggio(Punteggio classifica[], int numeroPunteggi)
{
    string nome;
    int nuoviPunti;

    cout << "\nNome giocatore da modificare: ";
    cin >> nome;
    pulisciInput();

    ordinaPunteggiPerNome(classifica, numeroPunteggi);

    int posizione = cercaGiocatoreBinaria(classifica, numeroPunteggi, nome);

    if (posizione == -1) {
        cout << ROSSO << "Giocatore non trovato." << RESET << endl;
        return;
    }

    nuoviPunti = leggiIntero("Nuovo punteggio: ");

    if (nuoviPunti < 0)
        nuoviPunti = 0;

    classifica[posizione].punti = nuoviPunti;
    salvaPunteggi(classifica, numeroPunteggi);

    cout << VERDE << "Punteggio modificato." << RESET << endl;
}

void eliminaPunteggio(Punteggio classifica[], int &numeroPunteggi)
{
    string nome;

    cout << "\nNome giocatore da eliminare: ";
    cin >> nome;
    pulisciInput();

    ordinaPunteggiPerNome(classifica, numeroPunteggi);

    int posizione = cercaGiocatoreBinaria(classifica, numeroPunteggi, nome);

    if (posizione == -1) {
        cout << ROSSO << "Giocatore non trovato." << RESET << endl;
        return;
    }

    for (int i = posizione; i < numeroPunteggi - 1; i++)
        classifica[i] = classifica[i + 1];

    numeroPunteggi--;
    salvaPunteggi(classifica, numeroPunteggi);

    cout << VERDE << "Punteggio eliminato." << RESET << endl;
}

void salvaRisultatoPartita(Punteggio classifica[], int &numeroPunteggi,
                           string nome, int punti)
{
    ordinaPunteggiPerNome(classifica, numeroPunteggi);

    int posizione = cercaGiocatoreBinaria(classifica, numeroPunteggi, nome);

    if (posizione == -1) {
        if (numeroPunteggi < MAX_PUNTEGGI) {
            classifica[numeroPunteggi].nome = nome;
            classifica[numeroPunteggi].punti = punti;
            numeroPunteggi++;
            cout << VERDE << "Punteggio salvato in classifica." << RESET << endl;
        }
    } else {
        if (punti > classifica[posizione].punti) {
            classifica[posizione].punti = punti;
            cout << VERDE << "Nuovo record personale salvato." << RESET << endl;
        } else {
            cout << GIALLO << "Il record precedente rimane: "
                 << classifica[posizione].punti << RESET << endl;
        }
    }

    ordinaPunteggiPerPunti(classifica, numeroPunteggi);
    salvaPunteggi(classifica, numeroPunteggi);
}

string scegliCategoria()
{
    char scelta;

    cout << "\n" << CYAN << "Scegli categoria" << RESET << endl;
    cout << "A - Scienza" << endl;
    cout << "B - Storia" << endl;
    cout << "C - Tecnologia" << endl;
    cout << "D - Tutte" << endl;

    scelta = leggiCarattere("Scelta: ");

    if (scelta == 'A')
        return "Science";
    else if (scelta == 'B')
        return "History";
    else if (scelta == 'C')
        return "Technology";
    else
        return "ALL";
}

void mescolaDomande(int indici[], int n)
{
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = indici[i];
        indici[i] = indici[j];
        indici[j] = temp;
    }
}

void giocaQuiz(Domanda elencoDomande[], int numeroDomande,
               Punteggio classifica[], int &numeroPunteggi)
{
    string nome;
    string categoria;
    int indiciDomande[MAX_DOMANDE];
    int quanteTrovate = 0;
    int punti = 0;

    cout << "\nInserisci il tuo nome: ";
    cin >> nome;
    pulisciInput();

    categoria = scegliCategoria();

    for (int i = 0; i < numeroDomande; i++) {
        if (categoria == "ALL" || elencoDomande[i].categoria == categoria) {
            indiciDomande[quanteTrovate] = i;
            quanteTrovate++;
        }
    }

    if (quanteTrovate == 0) {
        cout << ROSSO << "Non ci sono domande in questa categoria." << RESET << endl;
        return;
    }

    mescolaDomande(indiciDomande, quanteTrovate);

    int domandeDaFare = quanteTrovate;

    if (domandeDaFare > 10)
        domandeDaFare = 10;

    cout << "\n" << CYAN << "Inizio quiz" << RESET << endl;
    cout << "Giocatore: " << GIALLO << nome << RESET << endl;
    cout << "Categoria: " << GIALLO << testoCategoria(categoria) << RESET << endl;
    cout << "Domande: " << GIALLO << domandeDaFare << RESET << endl;
    cout << "Tempo per domanda: " << GIALLO << TEMPO_RISPOSTA << " secondi" << RESET << endl;
    cout << "Punti: " << VERDE << "+" << PUNTI_CORRETTA << " corretta"
         << RESET << ", " << ROSSO << "-" << PENALITA_TEMPO
         << " se scade il tempo" << RESET << endl;

    for (int i = 0; i < domandeDaFare; i++) {
        int indice = indiciDomande[i];
        char rispostaUtente;

        cout << "\n" << BLU << "Domanda " << i + 1 << "/" << domandeDaFare << RESET << endl;
        cout << elencoDomande[indice].testo << endl;
        cout << "A) " << elencoDomande[indice].opzioneA << endl;
        cout << "B) " << elencoDomande[indice].opzioneB << endl;
        cout << "C) " << elencoDomande[indice].opzioneC << endl;
        cout << "D) " << elencoDomande[indice].opzioneD << endl;

        bool haRisposto = leggiRispostaTimer(rispostaUtente);

        if (!haRisposto) {
            cout << ROSSO << "Tempo scaduto: risposta considerata sbagliata."
                 << " -" << PENALITA_TEMPO << " punti." << RESET << endl;
            cout << GIALLO << "Risposta corretta: "
                 << elencoDomande[indice].rispostaEsatta << RESET << endl;

            punti -= PENALITA_TEMPO;

            if (punti < 0)
                punti = 0;
        } else if (rispostaUtente == elencoDomande[indice].rispostaEsatta) {
            cout << VERDE << "Risposta corretta! +" << PUNTI_CORRETTA << RESET << endl;
            punti += PUNTI_CORRETTA;
        } else {
            cout << ROSSO << "Risposta sbagliata." << RESET << endl;
            cout << GIALLO << "Risposta corretta: "
                 << elencoDomande[indice].rispostaEsatta << RESET << endl;
        }

        cout << VIOLA << "Punteggio attuale: " << punti << RESET << endl;
    }

    cout << "\n" << CYAN << "Risultato finale" << RESET << endl;
    cout << nome << " ha totalizzato " << VERDE << punti << RESET << " punti." << endl;

    salvaRisultatoPartita(classifica, numeroPunteggi, nome, punti);
}

void aggiungiDomandaDaMenu(Domanda elencoDomande[], int &numeroDomande)
{
    string categoria;
    string testo;
    string A;
    string B;
    string C;
    string D;
    char rispostaCorretta;
    char scelta;

    cout << "\nNuova domanda" << endl;
    cout << "A - Scienza" << endl;
    cout << "B - Storia" << endl;
    cout << "C - Tecnologia" << endl;

    scelta = leggiCarattere("Categoria: ");

    if (scelta == 'A')
        categoria = "Science";
    else if (scelta == 'B')
        categoria = "History";
    else
        categoria = "Technology";

    cout << "Testo domanda: ";
    getline(cin, testo);

    cout << "Opzione A: ";
    getline(cin, A);

    cout << "Opzione B: ";
    getline(cin, B);

    cout << "Opzione C: ";
    getline(cin, C);

    cout << "Opzione D: ";
    getline(cin, D);

    rispostaCorretta = leggiCarattere("Risposta corretta: ");

    if (aggiungiDomandaArray(elencoDomande, numeroDomande,
                             categoria, testo, A, B, C, D, rispostaCorretta)) {
        salvaDomande(elencoDomande, numeroDomande);
        cout << VERDE << "Domanda aggiunta al file." << RESET << endl;
    } else {
        cout << ROSSO << "Domanda non valida, non salvata." << RESET << endl;
    }
}

void stampaTitolo()
{
    cout << CYAN << endl;
    cout << "========================================" << endl;
    cout << "                TRIQUIZ                 " << endl;
    cout << "      Quiz con categorie e classifica   " << endl;
    cout << "========================================" << RESET << endl;
    cout << GIALLO << "Scienza  |  Storia  |  Tecnologia" << RESET << endl;
}

void stampaMenu(Domanda elencoDomande[], int &numeroDomande,
                Punteggio classifica[], int &numeroPunteggi)
{
    int scelta;

    do {
        cout << "\n" << CYAN << "------------ Menu principale ------------" << RESET << endl;
        cout << GIALLO << "1." << RESET << " Gioca" << endl;
        cout << GIALLO << "2." << RESET << " Mostra classifica" << endl;
        cout << GIALLO << "3." << RESET << " Cerca giocatore (Binary Search)" << endl;
        cout << GIALLO << "4." << RESET << " Aggiungi punteggio" << endl;
        cout << GIALLO << "5." << RESET << " Modifica punteggio" << endl;
        cout << GIALLO << "6." << RESET << " Elimina punteggio" << endl;
        cout << GIALLO << "7." << RESET << " Aggiungi domanda" << endl;
        cout << GIALLO << "8." << RESET << " Ordina e salva classifica" << endl;
        cout << GIALLO << "9." << RESET << " Esci" << endl;

        scelta = leggiIntero("Scelta: ");

        switch (scelta) {
            case 1:
                giocaQuiz(elencoDomande, numeroDomande, classifica, numeroPunteggi);
                break;

            case 2:
                mostraClassifica(classifica, numeroPunteggi);
                break;

            case 3:
                cercaGiocatore(classifica, numeroPunteggi);
                break;

            case 4:
                inserisciPunteggioManuale(classifica, numeroPunteggi);
                break;

            case 5:
                modificaPunteggio(classifica, numeroPunteggi);
                break;

            case 6:
                eliminaPunteggio(classifica, numeroPunteggi);
                break;

            case 7:
                aggiungiDomandaDaMenu(elencoDomande, numeroDomande);
                break;

            case 8:
                ordinaPunteggiPerPunti(classifica, numeroPunteggi);
                salvaPunteggi(classifica, numeroPunteggi);
                cout << VERDE << "Classifica ordinata con Bubble Sort e salvata." << RESET << endl;
                break;

            case 9:
                cout << VIOLA << "\nProgramma terminato. Ciao!" << RESET << endl;
                break;

            default:
                cout << ROSSO << "Scelta non valida." << RESET << endl;
                break;
        }

    } while (scelta != 9);
}

int main()
{
    Domanda elencoDomande[MAX_DOMANDE];
    Punteggio classifica[MAX_PUNTEGGI];
    int numeroDomande = 0;
    int numeroPunteggi = 0;

    srand((unsigned int)time(NULL));

    stampaTitolo();

    if (caricaDomande(elencoDomande, numeroDomande) == 0) {
        cout << ROSSO << "Nessuna domanda caricata, il programma si chiude." << RESET << endl;
        return 0;
    }

    caricaPunteggi(classifica, numeroPunteggi);

    cout << VERDE << "Domande caricate: " << numeroDomande << RESET << endl;
    cout << VERDE << "Punteggi caricati: " << numeroPunteggi << RESET << endl;

    stampaMenu(elencoDomande, numeroDomande, classifica, numeroPunteggi);

    return 0;
}
