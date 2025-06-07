// g++ -std=c++17 -O2 src/main.cpp -Iinclude -o demo
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>

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
         << "  0                   - quit\n";
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

            default:
                cout << "Nieznana komenda";
        }

        pauseProgram();
    }

    cout << "\nKoniec programu.\n";
    return 0;
}
