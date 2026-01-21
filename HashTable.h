#ifndef DATA_STRUCTURES_WET_2_HASHTABLE_H
#define DATA_STRUCTURES_WET_2_HASHTABLE_H

#endif //DATA_STRUCTURES_WET_2_HASHTABLE_H

#pragma once

enum state {EMPTY, OCCUPIED}; // for the state of the Nodes in the array

template <class T>
struct HashNode {
    int key;
    T* data;
    state nodeState;

    HashNode(int key, T* data) : key(key), data(data), nodeState(OCCUPIED) {}
    ~HashNode() = default;
    HashNode() : key(0), data(nullptr), nodeState(EMPTY) {}
    HashNode(const HashNode& other) = delete;
    HashNode& operator=(const HashNode& other) = delete;
};

template <class T>
class HashTable {
private:
    HashNode<T>** table;
    // list of primes for hashtable capacities
    static constexpr int primeList[] = {101, 211, 431, 863, 1741, 3491, 6983, 13967, 27941, 55897,
    111799, 223603, 447209, 894427, 1788859, 3577721, 7155461, 14310929, 28621871, 57243761,
    114487529, 228975071, 457950151, 915900307, 1831800623};
    int size; // holds the current number of Nodes in the table
    int capacity; // holds the current length of the table
    int primeIndex; // holds the index of the current capacity in the array of primes

public:
    HashTable() : size(0), capacity(primeList[0]), primeIndex(0) { // O(1)
        table = new HashNode<T>*[capacity]();
    }
    ~HashTable() { // O(n)
        for (int i = 0; i < capacity; i++) {
            if (table[i] != nullptr) {
                delete table[i];
            }
        }
        delete[] table;
    }

    HashTable(const HashTable& other) = delete;
    HashTable& operator=(const HashTable& other) = delete;

    int hash(int key) { // function returns h(x) = x % capacity - O(1)
        return key % capacity;
    }

    int step(int key) { // function returns the r(x) = 1 + (x % 5) - O(1)
        return 1 + key % 5;
    }

    int getHashValue(int key) { // function returns the index according to the hashing, O(1) avg
        int hashValue = hash(key);
        int stepValue = step(key);
        while (table[hashValue] != nullptr && table[hashValue]->nodeState == OCCUPIED) {
            hashValue = (hashValue + stepValue) % capacity;
        }
        return hashValue;
    }

    void rehash() { // function increases the table's capacity - O(1) amortized
        HashNode<T>** oldTable = table;
        int oldCapacity = capacity;
        size = 0;
        capacity = primeList[primeIndex+1];
        primeIndex++;
        table = new HashNode<T>*[capacity]();
        for (int i = 0; i < oldCapacity; i++) {
            if (oldTable[i] != nullptr && oldTable[i]->nodeState == OCCUPIED)
                moveNode(oldTable[i]);
        }
        delete[] oldTable;
    }
    void moveNode(HashNode<T>* node) { // function moves the Node into the table - O(1) avg
        int hashValue = getHashValue(node->key);
        table[hashValue] = node;
        size++;
    }

    T* find(int key) { // function returns the data if found - O(1) avg
        int hashValue = hash(key);
        int stepValue = step(key);
        int counter = 0;
        while (counter < capacity) {
            if (table[hashValue] == nullptr) {
                return nullptr; // key doesn't exist
            }
            if (table[hashValue]->key == key) {
                return table[hashValue]->data; // key found
            }
            // keep searching
            hashValue = (hashValue + stepValue) % capacity;
            counter++;
        }
        return nullptr; // table fully searched - doesn't exist
    }

    void insert(int key, T* data) { // function inserts the data into the table - O(1) amortized avg
        int hashValue = getHashValue(key);
        HashNode<T>* toInsert = new HashNode<T>(key, data);
        table[hashValue] = toInsert;
        size++;
        if (size == capacity)
            rehash();
    }
};