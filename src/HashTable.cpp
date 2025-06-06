#include "HashTable.hpp"

// Konstruktor węzła
HashNode::HashNode(int k, int v, HashNode* n)
    : key(k), value(v), next(n) {}

// Konstruktor tablicy mieszającej
HashTable::HashTable() : elementCount(0)
{
    for (int i = 0; i < HASH_TABLE_SIZE; i++)
        buckets[i] = nullptr;
}

// Destruktor tablicy mieszającej
HashTable::~HashTable()
{
    clear();
}

// Funkcja mieszająca
int HashTable::hash(int key)
{
    int h = key % HASH_TABLE_SIZE;
    if (h < 0)
        h += HASH_TABLE_SIZE;
    return h;
}

// Wstawienie nowej pary klucz-wartość
bool HashTable::insert(int key, int value)
{
    int index = hash(key);
    HashNode* current = buckets[index];
    
    // Sprawdzamy czy klucz już istnieje
    while (current != nullptr)
    {
        if (current->key == key)
        {
            current->value = value; // Aktualizujemy wartość
            return false; // Klucz już istniał
        }
        current = current->next;
    }
    
    // Dodajemy nowy węzeł na początek listy
    HashNode* newNode = new HashNode(key, value, buckets[index]);
    buckets[index] = newNode;
    elementCount++;
    return true; // Dodano nowy element
}

// Usunięcie pary z danym kluczem
bool HashTable::remove(int key)
{
    int index = hash(key);
    HashNode* current = buckets[index];
    HashNode* previous = nullptr;
    
    while (current != nullptr)
    {
        if (current->key == key)
        {
            // Znaleźliśmy element do usunięcia
            if (previous == nullptr)
                buckets[index] = current->next; // Pierwszy element w liście
            else
                previous->next = current->next; // Środkowy lub ostatni element
            
            delete current;
            elementCount--;
            return true;
        }
        previous = current;
        current = current->next;
    }
    return false; // Nie znaleziono klucza
}

// Wyszukiwanie wartości dla danego klucza
bool HashTable::find(int key, int& outValue)
{
    int index = hash(key);
    HashNode* current = buckets[index];
    
    while (current != nullptr)
    {
        if (current->key == key)
        {
            outValue = current->value;
            return true;
        }
        current = current->next;
    }
    return false; // Nie znaleziono
}

// Zwraca liczbę elementów w tablicy
size_t HashTable::size()
{
    return elementCount;
}

// Sprawdza czy tablica jest pusta
bool HashTable::empty()
{
    return elementCount == 0;
}

// Czyści całą tablicę
void HashTable::clear()
{
    for (int i = 0; i < HASH_TABLE_SIZE; i++)
    {
        HashNode* current = buckets[i];
        while (current != nullptr)
        {
            HashNode* temp = current->next;
            delete current;
            current = temp;
        }
        buckets[i] = nullptr;
    }
    elementCount = 0;
}