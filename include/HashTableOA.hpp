#ifndef HASHTABLEOA_HPP
#define HASHTABLEOA_HPP

#include <cstddef>      //size_t 

const int OA_SIZE = 10007;

// Kod stanu komórki 
const char OA_EMPTY   = 0;
const char OA_OCCUP   = 1;
const char OA_DEL     = 2;

// Jedna komórka (slot) 
struct OA_Slot
{
    int  key;
    char flag;
};

// Cała tablica 
struct OA_Table
{
    OA_Slot   slot[OA_SIZE];
    std::size_t used;  

    OA_Table() : used(0)
    {
        for (int i = 0; i < OA_SIZE; ++i)
            slot[i].flag = OA_EMPTY;
    }
};

int oa_hash(int k) { return k % OA_SIZE; }

std::size_t oa_size(const OA_Table& t) { return t.used; }

bool oa_find(const OA_Table& t, int key)
{
    int h = oa_hash(key);
    for (int i = 0; i < OA_SIZE; ++i)
    {
        OA_Slot s = t.slot[(h + i) % OA_SIZE];

        if (s.flag == OA_EMPTY)      return false;
        if (s.flag == OA_OCCUP && s.key == key) return true;
    }
    return false;
}

bool oa_insert(OA_Table& t, int key)
{
    if (t.used == OA_SIZE) return false;

    int h = oa_hash(key);
    int firstFree = -1;

    for (int i = 0; i < OA_SIZE; ++i)
    {
        int idx = (h + i) % OA_SIZE;
        char f  = t.slot[idx].flag;

        if (f == OA_OCCUP && t.slot[idx].key == key) return false;

        if (f != OA_OCCUP) { firstFree = idx; break; }
    }

    t.slot[firstFree].key  = key;
    t.slot[firstFree].flag = OA_OCCUP;
    ++t.used;
    return true;
}

bool oa_remove(OA_Table& t, int key)
{
    int h = oa_hash(key);
    for (int i = 0; i < OA_SIZE; ++i)
    {
        int idx = (h + i) % OA_SIZE;
        char& f = t.slot[idx].flag;

        if (f == OA_EMPTY)         return false;          // nie ma klucza 
        if (f == OA_OCCUP && t.slot[idx].key == key)
        {
            f = OA_DEL; --t.used;  return true;           // oznacz jako usunięty 
        }
    }
    return false;
}

void oa_clear(OA_Table& t)
{
    for (int i = 0; i < OA_SIZE; ++i)
        t.slot[i].flag = OA_EMPTY;
    t.used = 0;
}

#endif
