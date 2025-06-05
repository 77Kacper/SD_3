#ifndef HASHTABLECHAINING_HPP
#define HASHTABLECHAINING_HPP

#include <cstddef>     // size_t 

namespace ChainHash
{

    const int TABLE_SIZE = 10007;    // mała liczba pierwsza

    struct Node
    {
        int   key;
        Node* next;
        Node(int k, Node* n = nullptr) : key(k), next(n) {}
    };

    struct Table;
    void clear(Table& ht);

    struct Table
    {
        Node*       buckets[TABLE_SIZE];
        std::size_t currentSize;

        Table() : currentSize(0)
        {
            for (int i = 0; i < TABLE_SIZE; ++i)
                buckets[i] = nullptr;
        }

        ~Table()
        {
            clear(*this);
        }
    };

    int hash(int key) { return key % TABLE_SIZE; }


    std::size_t size(const Table& ht) { return ht.currentSize; }


    bool find(const Table& ht, int key)
    {
        const Node* cur = ht.buckets[ hash(key) ];
        while (cur)
        {
            if (cur->key == key) return true;
            cur = cur->next;
        }
        return false;
    }

    bool insert(Table& ht, int key)
    {
        int   idx = hash(key);
        Node* cur = ht.buckets[idx];

        while (cur)
        {
            if (cur->key == key) return false;
            cur = cur->next;
        }

        Node* node = new Node(key, ht.buckets[idx]);

        ht.buckets[idx] = node;
        ++ht.currentSize;
        return true;
    }

    bool remove(Table& ht, int key)
    {
        int   idx  = hash(key);
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
        for (int i = 0; i < TABLE_SIZE; ++i)
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
