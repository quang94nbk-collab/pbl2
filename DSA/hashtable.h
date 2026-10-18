#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <iostream>
#include <string>

using namespace std;

template <typename K>
struct MyHash {
    unsigned long long operator()(const K& key) const {
        return static_cast<unsigned long long>(key);
    }
};

template <>
struct MyHash<string> {
    unsigned long long operator()(const string& key) const {
        unsigned long long hashVal = 0;
        for (char c : key) {
            hashVal = hashVal * 31 + c; 
        }
        return hashVal;
    }
};

template <typename Key, typename Data>
struct HashNode {
    Key key;
    Data data;
    HashNode *next;
    HashNode *prev;
    
    HashNode(const Key& k, const Data& d) 
        : key(k), data(d), next(nullptr), prev(nullptr) {}
};

template <typename Key, typename Data>
class HashTable {
private:
    static const int HASH_SIZE = 101;
    HashNode<Key, Data> *table[HASH_SIZE];
    int hashFunction(const Key& k) const;

public:
    HashTable();
    ~HashTable();

    bool search(const Key& k) const;
    bool get(const Key& k, Data& outData) const;
    void insert(const Key& k, const Data& d);
    void erase(const Key& k);
    void print() const;
    void clear();
};

template <typename Key, typename Data>
int HashTable<Key, Data>::hashFunction(const Key& k) const {
    MyHash<Key> hasher;
    unsigned long long hashValue = hasher(k);
    
    return static_cast<int>(hashValue % HASH_SIZE);
}

template <typename Key, typename Data>
HashTable<Key, Data>::HashTable() {
    for (int i = 0; i < HASH_SIZE; ++i) {
        table[i] = nullptr;
    }
}

template <typename Key, typename Data>
HashTable<Key, Data>::~HashTable() {
    clear();
}

template <typename Key, typename Data>
bool HashTable<Key, Data>::search(const Key& k) const {
    int index = hashFunction(k);
    HashNode<Key, Data> *p = table[index];
    while (p != nullptr) {
        if (p->key == k) return true;
        p = p->next;
    }
    return false;
}

template <typename Key, typename Data>
bool HashTable<Key, Data>::get(const Key& k, Data& outData) const {
    int index = hashFunction(k);
    HashNode<Key, Data> *p = table[index];
    while (p != nullptr) {
        if (p->key == k) {
            outData = p->data;
            return true;
        }
        p = p->next;
    }
    return false;
}

template <typename Key, typename Data>
void HashTable<Key, Data>::insert(const Key& k, const Data& d) {
    int index = hashFunction(k);
    HashNode<Key, Data> *p = table[index];
    
    while (p != nullptr) {
        if (p->key == k) {
            p->data = d;
            return;
        }
        p = p->next;
    }
    
    HashNode<Key, Data> *newNode = new HashNode<Key, Data>(k, d);
    newNode->next = table[index];
    if (table[index] != nullptr) {
        table[index]->prev = newNode;
    }
    table[index] = newNode;
}

template <typename Key, typename Data>
void HashTable<Key, Data>::erase(const Key& k) {
    int index = hashFunction(k);
    HashNode<Key, Data> *p = table[index];
    
    while (p != nullptr) {
        if (p->key == k) {
            if (p->prev == nullptr) {
                table[index] = p->next;
            } else {
                p->prev->next = p->next;
            }
            
            if (p->next != nullptr) {
                p->next->prev = p->prev;
            }
            
            delete p;
            return;
        }
        p = p->next;
    }
}

template <typename Key, typename Data>
void HashTable<Key, Data>::print() const {
    for (int i = 0; i < HASH_SIZE; ++i) {
        if (table[i] != nullptr) {
            cout << "Bucket " << i << ": ";
            HashNode<Key, Data> *p = table[i];
            while (p != nullptr) {
                cout << "[" << p->key << " : " << p->data << "] <-> ";
                p = p->next;
            }
            cout << "nullptr\n";
        }
    }
}

template <typename Key, typename Data>
void HashTable<Key, Data>::clear() {
    for (int i = 0; i < HASH_SIZE; ++i) {
        HashNode<Key, Data> *p = table[i];
        while (p != nullptr) {
            HashNode<Key, Data> *q = p->next;
            delete p;
            p = q;
        }
        table[i] = nullptr;
    }
}

#endif
