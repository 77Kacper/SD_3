#include <iostream>
#include "HashTableChaining.hpp"

int main()
{
    ChainHash::Table ht;              // tworzymy pustą tablicę

    ChainHash::insert(ht, 42);
    ChainHash::insert(ht, 17);
    ChainHash::remove(ht, 42);
    ChainHash::insert(ht, 37);
    ChainHash::insert(ht, 98);

    std::cout << "size = " << ChainHash::size(ht) << "\n";  // powinno wypisać 3
}
