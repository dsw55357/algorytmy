#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <list>
#include <vector>
#include <stdexcept>

template <typename K, typename V>
class HashTable {
private:
    std::vector<std::list<std::pair<K, V>>> table;
    int capacity;
    int size;

    int hashFunction(const K& key) const {
        return std::hash<K>{}(key) % capacity;
    }

public:
    HashTable(int capacity = 100) : capacity(capacity), size(0) {
        table.resize(capacity);
    }

    void insert(const K& key, const V& value) {
        int index = hashFunction(key);
        for (auto& kv : table[index]) {
            if (kv.first == key) {
                kv.second = value; // Update existing value
                return;
            }
        }
        table[index].emplace_back(key, value);
        ++size;
    }

    V get(const K& key) const {
        int index = hashFunction(key);
        for (const auto& kv : table[index]) {
            if (kv.first == key) {
                return kv.second;
            }
        }
        throw std::runtime_error("Key not found");
    }

    bool remove(const K& key) {
        int index = hashFunction(key);
        auto& chain = table[index];
        for (auto it = chain.begin(); it != chain.end(); ++it) {
            if (it->first == key) {
                chain.erase(it);
                --size;
                return true;
            }
        }
        return false;
    }

    bool contains(const K& key) const {
        int index = hashFunction(key);
        for (const auto& kv : table[index]) {
            if (kv.first == key) {
                return true;
            }
        }
        return false;
    }

    int getSize() const {
        return size;
    }

    void clear() {
        table.clear();
        table.resize(capacity);
        size = 0;
    }
};

#endif // HASHTABLE_H
