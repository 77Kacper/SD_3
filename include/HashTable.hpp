#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <cstddef>
#include <iostream>
using namespace std;

// Rozmiar tablicy - mała liczba pierwsza
const int HASH_TABLE_SIZE = 10007;

// Struktura węzła dla listy jednokierunkowej
struct HashNode
{
    int key;
    int value;
    HashNode* next;
    
    HashNode(int k, int v, HashNode* n = nullptr);
};

// Główna struktura tablicy mieszającej
struct HashTable
{
    HashNode* buckets[HASH_TABLE_SIZE];
    size_t elementCount;
    
    HashTable();
    ~HashTable();
    
    // Funkcja mieszająca
    int hash(int key);
    
    // Wstawienie nowej pary klucz-wartość
    bool insert(int key, int value);
    
    // Usunięcie pary z danym kluczem
    bool remove(int key);
    
    // Wyszukiwanie wartości dla danego klucza
    bool find(int key, int& outValue);
    
    // Zwraca liczbę elementów w tablicy
    size_t size();
    
    // Sprawdza czy tablica jest pusta
    bool empty();
    
    // Czyści całą tablicę
    void clear();
};

#endif