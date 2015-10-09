//
//  CocoHashTable.h
//  HashTable
//
//  Created by Александр Малышев on 22.03.15.
//  Copyright (c) 2015 SecurityQQ. All rights reserved.
//

#ifndef __HashTable__CocoHashTable__
#define __HashTable__CocoHashTable__
#include <stdio.h>
#include <vector>
#include <assert.h>
#include <iostream>
#include <memory.h>
#include <array>

typedef unsigned long long ull;
#define NullString ""
#define defaultTableSize 8



class CocoHashTable {
public:
    
    CocoHashTable();
    ~CocoHashTable();
    
    void add      (const std::string &key);
    void remove   (const std::string &key);
    bool contains (const std::string &key);
    
    
    
private:
    ull getHash ( const std::string &key);
    ull hash1(    const std::string &key);
    ull hash2(    const std::string &key);
    ull tryToHash(const std::string &key, ull i);
    
    void getPrime();
    ull prime;
    
    std::vector<std::string> table;
    
    void rehash();
    void growHashTable();
    
    ull k, l, a, b; // seeds for hash functions
    
    
};

CocoHashTable:: CocoHashTable() {
    prime = 573259391;
    for( ull i = 0; i < defaultTableSize; ++i ) {
        table.push_back(NullString);
    }
    getPrime();
}
CocoHashTable:: ~CocoHashTable() {
    table.erase(table.begin(), table.end());
}
void CocoHashTable:: getPrime() {
    k = (random() % (prime - 1)) + 1;
    l = (random() % (prime - 1)) + 1;
    b = random() % prime;
    a = random() % prime;
    while( l == k ) {
        l = random() % prime;
    }
    
}
ull CocoHashTable:: getHash(const std::string &key) {
    char *str = new char[key.size()];
    memcpy(str, key.data(), key.length() + 1);
    ull hash = 0;
    for (ull i = 0; i < key.length(); ++i) {
        hash += (unsigned char)(str[i]);
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    delete [] str;
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

ull CocoHashTable:: hash1(const std::string &key) {
    ull hash = 0;
    hash = ((getHash(key) * k + b) % prime) % table.size();
    return hash % table.size();
}
ull CocoHashTable:: hash2(const std::string &key) {
    ull hash = 0;
    hash = ((getHash(key) * l + a) % prime) % table.size();
    return hash % table.size();
}

bool CocoHashTable:: contains(const std::string &key) {
    return (key == table[hash1(key)] || key == table[hash2(key)]);
}

void CocoHashTable:: remove(const std::string &key) {
    ull index1 = hash1(key);
    ull index2 = hash2(key);
    if( table[index1] == key ) {
        table[index1] = NullString;
    }
    if (table[index2] == key) {
        table[index2] = NullString;
    }
}

void CocoHashTable::add(const std::string &key) {
    std::string temp(key);
    std::array<std::string, 2> checkedKeys = {"", ""};
    int ind1 = -1;
    int ind2 = -1;
    do {
        ind1 = true;
        ind2 = true;
        ull index1 = hash1(temp);
        ull index2 = hash2(temp);
        //        std::cout<<index1 << " " << index2 << std::endl;
//        std::cout.flush();
        if( table[index1] == NullString ) {
            table[index1] = temp;
            return;
        }
        if( table[index2] == NullString ) {
            table[index2] = temp;
            return;
        }
        for( int i = 0; i < checkedKeys.size(); ++i ) {
            if( table[index1] == checkedKeys[i] ) {
                ind1 = i;
                break;
            }
        }
        if( ind1 != -1 ) {
            checkedKeys[ind1] = temp;
            std::swap(temp, table[index1]);
            continue;
        }
        for( int i = 0; i < checkedKeys.size(); ++i ) {
            if( table[index2] == checkedKeys[i] ) {
                ind2 = i;
                break;
            }
        }
        if( ind2 != -1 ) {
            checkedKeys[ind2] = temp;
            std::swap(temp, table[index2]);
            continue;
        }
    } while( ind1 == -1 || ind2 == -1 );
    
    rehash();
    add(temp);
}

void CocoHashTable:: rehash() {
    std::vector<std::string> keys;
    for( ull i = 0; i < table.size(); ++i ) {
        if( table[i] != NullString ) {
            keys.push_back(table[i]);
        }
    }
    
    growHashTable();
    getPrime();
    for( ull i = 0; i < keys.size(); ++i ) {
        add(keys[i]);
    }
    
}

void CocoHashTable:: growHashTable() {
    ull newSize = table.size() * 2;
    table.assign(newSize, NullString);
}

#endif /* defined(__HashTable__CocoHashTable__) */