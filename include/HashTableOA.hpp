#ifndef HASHTABLEOA_HPP
#define HASHTABLEOA_HPP

#include <cstddef>      // size_t

const int  OA_SIZE = 10007;

const char OA_EMPTY = 0;
const char OA_OCCUP = 1;
const char OA_DEL   = 2;

struct OA_Slot
{
    int  key;
    int  value;
    char flag;
};

struct OA_Table
{
    OA_Slot     slot[OA_SIZE];
    std::size_t used;

    OA_Table() : used(0)
    {
        for (int i = 0; i < OA_SIZE; ++i)
            slot[i].flag = OA_EMPTY;
    }
};

int oa_hash(int key)
{
    int h = key % OA_SIZE;
    if (h < 0) h += OA_SIZE;
    return h;
}

std::size_t oa_size(const OA_Table& t) { return t.used; }

// wyszukiwanie (zwraca wartość przez referencję)
bool oa_find(const OA_Table& t, int key, int& outValue)
{
    int h = oa_hash(key);
    for (int i = 0; i < OA_SIZE; ++i)
    {
        int idx = (h + i) % OA_SIZE;
        char f  = t.slot[idx].flag;

        if (f == OA_EMPTY) return false;                 // pusta komórka – nie ma
        if (f == OA_OCCUP && t.slot[idx].key == key) {
            outValue = t.slot[idx].value;                // zwracamy znalezioną wartość
            return true;
        }
    }
    return false;
}

// wstawianie / aktualizacja 
bool oa_insert(OA_Table& t, int key, int value)
{
    if (t.used == OA_SIZE) return false;

    int h = oa_hash(key);
    int firstFree = -1;

    for (int i = 0; i < OA_SIZE; ++i)
    {
        int idx = (h + i) % OA_SIZE;
        char f  = t.slot[idx].flag;

        if (f == OA_OCCUP && t.slot[idx].key == key) {
            t.slot[idx].value = value;
            return false;
        }

        if (f != OA_OCCUP) { firstFree = idx; break; }
    }

    t.slot[firstFree].key   = key;
    t.slot[firstFree].value = value;
    t.slot[firstFree].flag  = OA_OCCUP;
    ++t.used;
    return true;
}

// usuwanie 
bool oa_remove(OA_Table& t, int key)
{
    int h = oa_hash(key);
    for (int i = 0; i < OA_SIZE; ++i)
    {
        int idx = (h + i) % OA_SIZE;
        char& f = t.slot[idx].flag;

        if (f == OA_EMPTY) return false;                 // nie ma klucza
        if (f == OA_OCCUP && t.slot[idx].key == key)
        {
            f = OA_DEL;
            --t.used;
            return true;
        }
    }
    return false;
}

// czyszczenie tablicy
void oa_clear(OA_Table& t)
{
    for (int i = 0; i < OA_SIZE; ++i)
        t.slot[i].flag = OA_EMPTY;
    t.used = 0;
}

#endif
