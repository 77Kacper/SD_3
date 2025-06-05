//g++ -std=c++17 -O2 src/main.cpp -Iinclude -o demo
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>

#include "HashTableChaining.hpp"
#include "HashTableOA.hpp"

using std::cin;
using std::cout;
using std::size_t;
using std::numeric_limits;
using std::streamsize;

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
         << "  1 <klucz>     – insert\n"
         << "  2 <klucz>     – remove\n"
         << "  3 <klucz>     – find\n"
         << "  4             – size (ile elementów)\n"
         << "  0             – quit\n";
}

int main()
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    int choice = 0;
    while (choice != 1 && choice != 2)
    {
        clearScreen();
        cout << "=== Mini-demo tablic mieszajacych ===\n"
             << "Wybierz wariant:\n"
             << "  1 – Separate Chaining\n"
             << "  2 – Open Addressing (liniowe)\n"
             << "> ";
        cin >> choice;
        if (!cin) return 1;
    }

    ChainHash::Table chainHT;
    OA_Table         oaHT;   

    int cmd;
    int key;

    while (true)
    {
        clearScreen();
        cout << "### Wariant " << choice
             << " | liczba elementów = ";
        if (choice == 1)
            cout << ChainHash::size(chainHT);
        else
            cout << oa_size(oaHT);
        cout << "\n";
        printMenu();
        cout << "\n> ";

        if (!(cin >> cmd)) break;

        if (cmd == 0) break;

        switch (cmd)
        {
            case 1:
                if (cin >> key)
                {
                    bool ok;
                    if (choice == 1)
                        ok = ChainHash::insert(chainHT, key);
                    else
                        ok = oa_insert(oaHT, key);

                    if (ok)
                        cout << "OK – dodano";
                    else
                        cout << "Klucz już istnieje / tablica pełna";
                }
                else { cin.clear(); }
                break;

            case 2:
                if (cin >> key)
                {
                    bool ok;
                    if (choice == 1)
                        ok = ChainHash::remove(chainHT, key);
                    else
                        ok = oa_remove(oaHT, key);

                    if (ok)
                        cout << "Usunięto";
                    else
                        cout << "Brak takiego klucza";
                }
                else { cin.clear(); }
                break;

            case 3:
                if (cin >> key)
                {
                    bool ok;
                    if (choice == 1)
                        ok = ChainHash::find(chainHT, key);
                    else
                        ok = oa_find(oaHT, key);

                    if (ok)
                        cout << "Znaleziono";
                    else
                        cout << "Nie ma";
                }
                else { cin.clear(); }
                break;

            case 4:
                if (choice == 1)
                    cout << "Liczba elementów = " << ChainHash::size(chainHT);
                else
                    cout << "Liczba elementów = " << oa_size(oaHT);
                break;

            default:
                cout << "Nieznana komenda";
        }

        pauseProgram();
    }

    cout << "\nKoniec programu.\n";
    return 0;
}