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
         << "  1 <klucz> <wartosc> – insert\n"   // ★ zmiana
         << "  2 <klucz>           – remove\n"
         << "  3 <klucz>           – find\n"
         << "  4                   – size\n"
         << "  0                   – quit\n";
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

    int cmd=0;
    int key=0;
    int val=0;

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
            if (cin >> key >> val)
            {
                bool ok;
                if (choice == 1)
                    ok = ChainHash::insert(chainHT, key, val);
                else
                    ok = oa_insert(oaHT, key, val);

                if (ok)  cout << "OK – dodano nową parę";
                else     cout << "Klucz istniał – wartość zaktualizowana";
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
                int found;
                bool ok;
                if (choice == 1)
                    ok = ChainHash::find(chainHT, key, found);
                else
                    ok = oa_find(oaHT, key, found);

                if (ok)  cout << "Znaleziono – value = " << found;
                else     cout << "Nie ma";
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