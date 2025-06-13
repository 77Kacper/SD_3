#include "HashTableAVL.hpp"

// Konstruktor z dynamiczną alokacją
AVLTable::AVLTable() {
    capacity = 10007;
    used = 0;
    buckets = new AVLNode*[capacity]();
}

// Pomocnicza funkcja – wysokość węzła
int avl_node_height(AVLNode* node)
{
    if (node)
        return node->height;
    else
        return 0;
}

// Destruktor
AVLTable::~AVLTable()
{
    avl_clear(*this);
     delete[] buckets;
}

// Pomocnicza funkcja – wysokość węzła
int avl_node_height(AVLNode* node) {
    if (node != nullptr)
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
    int index = key % t.capacity;
    if (index < 0)
        index += t.capacity;

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

void avl_copy_tree(AVLNode* node, AVLNode** new_buckets, size_t new_capacity) {
    if (node == nullptr) return;
    avl_copy_tree(node->left, new_buckets, new_capacity);
    avl_copy_tree(node->right, new_buckets, new_capacity);

    size_t index = node->key % new_capacity;
    if (index < 0) index += new_capacity;

    bool inserted = false;
    new_buckets[index] = avl_insert_node(new_buckets[index], node->key, node->value, inserted);
}

void avl_rehash(AVLTable& t) {
    size_t new_capacity = t.capacity * 2;
    AVLNode** new_buckets = new AVLNode*[new_capacity]();

    for (size_t i = 0; i < t.capacity; ++i) {
        avl_copy_tree(t.buckets[i], new_buckets, new_capacity);
        avl_delete_tree(t.buckets[i]);
    }

    delete[] t.buckets;
    t.buckets = new_buckets;
    t.capacity = new_capacity;
}

// Wstawianie
bool avl_insert(AVLTable& t, int key, int value) {
    if (t.used > 0.75 * t.capacity) {
        avl_rehash(t);
    }

    int index = key % t.capacity;
    if (index < 0) index += t.capacity;

    bool inserted = false;
    t.buckets[index] = avl_insert_node(t.buckets[index], key, value, inserted);
    if (inserted) t.used++;
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
    int index = key % t.capacity;
    if (index < 0) index += t.capacity;

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
    for (int i = 0; i < t.capacity; ++i)
    {
        avl_delete_tree(t.buckets[i]);
        t.buckets[i] = nullptr;
    }
    t.used = 0;
}

// Hash
int avl_hash(int key, size_t capacity) {
    int h = key % capacity;
    if (h < 0)
        return h + capacity;
    else
        return h;
}

// Rozmiar
size_t avl_size(const AVLTable& t)
{
    return t.used;
}