#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <fstream> 
#include <string>

#include "HashTableChaining.hpp"
#include "HashTableOA.hpp"
#include "HashTable.hpp"
#include "HashTableAVL.hpp"

using namespace std;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseProgram() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "\n### Naciśnij Enter, aby kontynuować";
    cin.get();
}

void printMenu()
{
    cout << "Komendy:\n"
         << "  1 <klucz> <wartosc> - insert\n"
         << "  2 <klucz>           - remove\n"
         << "  3 <klucz>           - find\n"
         << "  4                   - size\n"
         << "  5 <plik.txt>        - wczytaj wsadowo\n"   // +++
         << "  0                   - quit\n";
}

void loadFromFile(const string& fname, int choice,
                  ChainHash::Table& chainHT,
                  OA_Table&         oaHT,
                  HashTable&        ht,
                  AVLTable&         avlHT)
{
    ifstream fin(fname);
    if (!fin) {
        cout << "Nie mozna otworzyc pliku \"" << fname << "\"\n";
        return;
    }

    int cmd, key, val;
    while (fin >> cmd)        // czytamy kolejne komendy
    {
        if (cmd == 0) break;  // „0” w pliku = koniec wsadu

        switch (cmd)
        {
            case 1:   // insert
                if (fin >> key >> val)
                {
                    if      (choice == 1) ChainHash::insert(chainHT, key, val);
                    else if (choice == 2) oa_insert(oaHT, key, val);
                    else if (choice == 3) ht.insert(key, val);
                    else                  avl_insert(avlHT, key, val);
                }
                break;

            case 2:   // remove
                if (fin >> key)
                {
                    if      (choice == 1) ChainHash::remove(chainHT, key);
                    else if (choice == 2) oa_remove(oaHT, key);
                    else if (choice == 3) ht.remove(key);
                    else                  avl_remove(avlHT, key);
                }
                break;

            case 3:   // find (wynik podajemy, żeby było coś widać)
                if (fin >> key)
                {
                    int found;
                    bool ok = (choice == 1) ? ChainHash::find(chainHT, key, found) :
                               (choice == 2) ? oa_find(oaHT, key, found)            :
                               (choice == 3) ? ht.find(key, found)                  :
                                               avl_find(avlHT, key, found);
                    cout << "find(" << key << ") -> "
                         << (ok ? to_string(found) : "brak") << '\n';
                }
                break;

            case 4:   // size
                cout << "size = "
                     << ((choice == 1) ? ChainHash::size(chainHT) :
                         (choice == 2) ? oa_size(oaHT)           :
                         (choice == 3) ? ht.size()               :
                                         avl_size(avlHT))
                     << '\n';
                break;

            default:
                cout << "Nieznana komenda w pliku – pomijam\n";
        }
    }
    cout << "=== Koniec wsadu z \"" << fname << "\" ===\n";
}

int main()
{
    setlocale( LC_ALL, "pl_PL" );
    srand(static_cast<unsigned>(time(nullptr)));

    // Wybór wariantu
    int choice = 0;
    while (choice < 1 || choice > 4)
    {
        clearScreen();
        cout << "=== Mini-demo tablic mieszajacych ===\n"
             << "Wybierz wariant:\n"
             << "  1 - Separate Chaining (lista w kubelku)\n"
             << "  2 - Open Addressing (liniowe)\n"
             << "  3 - HashTable\n"
             << "  4 - AVL w kubełkach\n"
             << "> ";
        cin >> choice;
        if (!cin) return 1;
    }

    ChainHash::Table chainHT;   // wariant 1
    OA_Table         oaHT;      // wariant 2
    HashTable        ht;        // wariant 3
    AVLTable         avlHT;     // wariant 4

    int cmd = 0, key = 0, val = 0;

    // Główna pętla
    while (true)
    {
        clearScreen();
        cout << "### Wariant " << choice
             << " | liczba elementów = ";
        if      (choice == 1) cout << ChainHash::size(chainHT);
        else if (choice == 2) cout << oa_size(oaHT);
        else if (choice == 3) cout << ht.size();
        else                  cout << avl_size(avlHT);
        cout << "\n";

        printMenu();
        cout << "\n> ";

        if (!(cin >> cmd)) break;
        if (cmd == 0) break;

        switch (cmd)
        {
            // Insert
            case 1:
                if (cin >> key >> val)
                {
                    bool ok;
                    if      (choice == 1) ok = ChainHash::insert(chainHT, key, val);
                    else if (choice == 2) ok = oa_insert(oaHT, key, val);
                    else if (choice == 3) ok = ht.insert(key, val);
                    else                  ok = avl_insert(avlHT, key, val);

                    cout << (ok ? "OK – dodano nową parę"
                                : "Klucz istniał – wartość zaktualizowana");
                }
                else cin.clear();
                break;

            // Remove
            case 2:
                if (cin >> key)
                {
                    bool ok;
                    if      (choice == 1) ok = ChainHash::remove(chainHT, key);
                    else if (choice == 2) ok = oa_remove(oaHT, key);
                    else if (choice == 3) ok = ht.remove(key);
                    else                  ok = avl_remove(avlHT, key);

                    cout << (ok ? "Usunięto"
                                : "Brak takiego klucza");
                }
                else cin.clear();
                break;

            // Find
            case 3:
                if (cin >> key)
                {
                    int found;
                    bool ok;
                    if      (choice == 1) ok = ChainHash::find(chainHT, key, found);
                    else if (choice == 2) ok = oa_find(oaHT, key, found);
                    else if (choice == 3) ok = ht.find(key, found);
                    else                  ok = avl_find(avlHT, key, found);

                    if (ok) cout << "Znaleziono – value = " << found;
                    else    cout << "Nie ma";
                }
                else cin.clear();
                break;

            // Size
            case 4:
                if      (choice == 1) cout << "Liczba elementów = " << ChainHash::size(chainHT);
                else if (choice == 2) cout << "Liczba elementów = " << oa_size(oaHT);
                else if (choice == 3) cout << "Liczba elementów = " << ht.size();
                else                  cout << "Liczba elementów = " << avl_size(avlHT);
                break;

            // Wczytywanie z pliku 
            case 5:
            {
                string fname;
                if (cin >> fname)
                    loadFromFile(fname, choice, chainHT, oaHT, ht, avlHT);
                else cin.clear();
                break;
            }

            default:
                cout << "Nieznana komenda";
        }

        pauseProgram();
    }

    cout << "\nKoniec programu.\n";
    return 0;
}
