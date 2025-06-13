#ifndef HASHTABLECHAINING_HPP
#define HASHTABLECHAINING_HPP

#include <cstddef>     // size_t 

namespace ChainHash
{

    const int INITIAL_CAPACITY = 10007;    // mała liczba pierwsza

    struct Node
    {
        int   key;
        int   value;
        Node* next;

        Node(int k, int v, Node* n = nullptr)
            : key(k), value(v), next(n) {}
    };

    struct Table;
    void clear(Table& ht);

    struct Table
    {
        Node**       buckets;
        std::size_t currentSize;
        std::size_t capacity;

        Table()
        {
            capacity = INITIAL_CAPACITY;
            currentSize = 0;
            buckets = new Node*[capacity];
            for (std::size_t i = 0; i < capacity; ++i)
                buckets[i] = nullptr;
        }

        ~Table()
        {
            clear(*this);
            delete[] buckets;
        }
    };

    int hash(int key, std::size_t capacity)
    {
        int h = key % static_cast<int>(capacity);      // wynik może być ujemny
        if (h < 0)
            h += static_cast<int>(capacity);           // przesuwamy w zakres 0 … TABLE_SIZE-1
        return h;
    }



    std::size_t size(const Table& ht) { return ht.currentSize; }


    bool find(const Table& ht, int key, int& outValue)
    {
        const Node* cur = ht.buckets[ hash(key, ht.capacity) ];
        while (cur)
        {
            if (cur->key == key) {
                outValue = cur->value;   // zwracamy wartość przez referencję
                return true;
            }
            cur = cur->next;
        }
        return false;
    }

    void rehash(Table& ht)
    {
        std::size_t new_capacity = ht.capacity * 2;
        Node** new_buckets = new Node*[new_capacity];
        for (std::size_t i = 0; i < new_capacity; ++i)
            new_buckets[i] = nullptr;

        for (std::size_t i = 0; i < ht.capacity; ++i)
        {
            Node* cur = ht.buckets[i];
            while (cur)
            {
                Node* next = cur->next;
                int new_idx = hash(cur->key, new_capacity);
                cur->next = new_buckets[new_idx];
                new_buckets[new_idx] = cur;
                cur = next;
            }
        }

        delete[] ht.buckets;
        ht.buckets = new_buckets;
        ht.capacity = new_capacity;
    }

    bool insert(Table& ht, int key, int value)
    {
        if (ht.currentSize > 0.75 * ht.capacity)
            rehash(ht);

        int idx = hash(key, ht.capacity);
        Node* cur = ht.buckets[idx];

        while (cur)
        {
            if (cur->key == key) {
                cur->value = value;
                return false;
            }
            cur = cur->next;
        }

        Node* node = new Node(key, value, ht.buckets[idx]);
        ht.buckets[idx] = node;
        ++ht.currentSize;
        return true;
    }


    bool remove(Table& ht, int key)
    {
        int   idx  = hash(key, ht.capacity);
        Node* cur  = ht.buckets[idx];
        Node* prev = nullptr;

        while (cur)
        {
            if (cur->key == key)
            {
                if (prev) prev->next = cur->next;
                else      ht.buckets[idx] = cur->next;

                delete cur;
                --ht.currentSize;
                return true;
            }
            prev = cur;
            cur  = cur->next;
        }
        return false;
    }

    void clear(Table& ht)
    {
        for (std::size_t i = 0; i < ht.capacity; ++i)
        {
            Node* cur = ht.buckets[i];
            while (cur)
            {
                Node* nxt = cur->next;
                delete cur;
                cur = nxt;
            }
            ht.buckets[i] = nullptr;
        }
        ht.currentSize = 0;
    }

}

#endif
