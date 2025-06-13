#ifndef HASHTABLE_AVL_HPP
#define HASHTABLE_AVL_HPP

#include <cstddef>   // size_t
using namespace std;

// Rozmiar tablicy - mała liczba pierwsza
const int AVL_TABLE_SIZE = 10007;

// węzeł drzewa AVL
struct AVLNode
{
    int key;
    int value;
    int height;
    AVLNode* left;
    AVLNode* right;

    AVLNode(int k, int v)
        : key(k), value(v), height(1), left(nullptr), right(nullptr) {}
};

// tablica mieszająca – każdy kubełek to korzeń AVL
struct AVLTable
{
    AVLNode** buckets;
    size_t used;          // liczba par w całej tablicy
    size_t capacity;

    // konstruktor – zeruje kubełki
    AVLTable();

    // destruktor
    ~AVLTable();
};

int avl_hash(int key, size_t capacity);
size_t avl_size(const AVLTable& t);
bool avl_find(const AVLTable& t, int key, int& outVal);
bool avl_insert(AVLTable& t, int key, int value);
bool avl_remove(AVLTable& t, int key);
void avl_clear(AVLTable& t);
void avl_rehash(AVLTable& t);

#endif /* HASHTABLE_AVL_HPP */
