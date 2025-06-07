#include "HashTableAVL.hpp"

// Pomocnicza funkcja – wysokość węzła
int avl_node_height(AVLNode* node)
{
    if (node)
        return node->height;
    else
        return 0;
}

// Pomocnicza – aktualizuje wysokość danego węzła
void avl_update_height(AVLNode* node)
{
    int hl = avl_node_height(node->left);
    int hr = avl_node_height(node->right);
    int max;

    if (hl > hr)
        max = hl;
    else
        max = hr;

    node->height = 1 + max;
}


// Różnica wysokości
int avl_balance_factor(AVLNode* node)
{
    if (node)
        return avl_node_height(node->right) - avl_node_height(node->left);
    else
        return 0;
}


// Rotacja w lewo
AVLNode* avl_rotate_left(AVLNode* a)
{
    AVLNode* b = a->right;
    a->right = b->left;
    b->left = a;
    avl_update_height(a);
    avl_update_height(b);
    return b;
}

// Rotacja w prawo
AVLNode* avl_rotate_right(AVLNode* a)
{
    AVLNode* b = a->left;
    a->left = b->right;
    b->right = a;
    avl_update_height(a);
    avl_update_height(b);
    return b;
}

// Równoważenie poddrzewa z korzeniem w node
AVLNode* avl_balance(AVLNode* node)
{
    avl_update_height(node);
    int bf = avl_balance_factor(node);

    if (bf == 2)
    {
        if (avl_balance_factor(node->right) < 0)
            node->right = avl_rotate_right(node->right);
        return avl_rotate_left(node);
    }
    if (bf == -2)
    {
        if (avl_balance_factor(node->left) > 0)
            node->left = avl_rotate_left(node->left);
        return avl_rotate_right(node);
    }
    return node;
}

// Wstawianie lub aktualizacja (rekurencyjnie)
AVLNode* avl_insert_node(AVLNode* node, int key, int value, bool& inserted)
{
    if (!node)
    {
        inserted = true;
        return new AVLNode(key, value);
    }

    if (key < node->key)
        node->left = avl_insert_node(node->left, key, value, inserted);
    else if (key > node->key)
        node->right = avl_insert_node(node->right, key, value, inserted);
    else
    {
        node->value = value;
        inserted = false;
    }

    return avl_balance(node);
}

// Wyszukiwanie
bool avl_find(const AVLTable& t, int key, int& outVal)
{
    int index = key % AVL_TABLE_SIZE;
    if (index < 0)
        index += AVL_TABLE_SIZE;

    AVLNode* cur = t.buckets[index];
    while (cur)
    {
        if (key == cur->key)
        {
            outVal = cur->value;
            return true;
        }

        if (key < cur->key)
            cur = cur->left;
        else
            cur = cur->right;
    }

    return false;
}


// Wstawianie
bool avl_insert(AVLTable& t, int key, int value)
{
    int index = key % AVL_TABLE_SIZE;
    if (index < 0) index += AVL_TABLE_SIZE;

    bool inserted = false;
    t.buckets[index] = avl_insert_node(t.buckets[index], key, value, inserted);
    if (inserted) ++t.used;
    return inserted;
}

// Znajdź najmniejszy węzeł
AVLNode* avl_find_min(AVLNode* node)
{
    if (node->left)
        return avl_find_min(node->left);
    else
        return node;
}


// Usuwa najmniejszy węzeł
AVLNode* avl_remove_min(AVLNode* node)
{
    if (!node->left)
        return node->right;
    node->left = avl_remove_min(node->left);
    return avl_balance(node);
}

// Usuwanie
AVLNode* avl_remove_node(AVLNode* node, int key, bool& removed)
{
    if (!node) return nullptr;

    if (key < node->key)
        node->left = avl_remove_node(node->left, key, removed);
    else if (key > node->key)
        node->right = avl_remove_node(node->right, key, removed);
    else
    {
        removed = true;
        AVLNode* left = node->left;
        AVLNode* right = node->right;
        delete node;

        if (!right) return left;
        AVLNode* min = avl_find_min(right);
        min->right = avl_remove_min(right);
        min->left = left;
        return avl_balance(min);
    }
    return avl_balance(node);
}

bool avl_remove(AVLTable& t, int key)
{
    int index = key % AVL_TABLE_SIZE;
    if (index < 0) index += AVL_TABLE_SIZE;

    bool removed = false;
    t.buckets[index] = avl_remove_node(t.buckets[index], key, removed);
    if (removed) --t.used;
    return removed;
}

// Czyszczenie poddrzewa
void avl_delete_tree(AVLNode* node)
{
    if (!node) return;
    avl_delete_tree(node->left);
    avl_delete_tree(node->right);
    delete node;
}

// Czyszczenie całej tablicy
void avl_clear(AVLTable& t)
{
    for (int i = 0; i < AVL_TABLE_SIZE; ++i)
    {
        avl_delete_tree(t.buckets[i]);
        t.buckets[i] = nullptr;
    }
    t.used = 0;
}

// Hash
int avl_hash(int key)
{
    int h = key % AVL_TABLE_SIZE;
    if (h < 0) h += AVL_TABLE_SIZE;
    return h;
}

// Rozmiar
size_t avl_size(const AVLTable& t)
{
    return t.used;
}

// Destruktor
AVLTable::~AVLTable()
{
    avl_clear(*this);
}
