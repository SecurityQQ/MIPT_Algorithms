//
//  main.cpp
//  OpenAdressForContest
//
//  Created by Александр Малышев on 22.03.15.
//  Copyright (c) 2015 SecurityQQ. All rights reserved.
//

#include <stdio.h>
#include <vector>
#include <assert.h>
#include <iostream>
#include <memory.h>

typedef unsigned long long ull;
#define NullString ""
#define DelString "DEL"
#define defaultTableSize (8)



class OpenAdressedHashTable {
public:
    
    OpenAdressedHashTable();
    OpenAdressedHashTable(ull tableSize);
    ~OpenAdressedHashTable();
    
    void add      (const std::string& key);
    void remove   (const std::string& key);
    bool contains (const std::string& key);
    
    
    
private:
    
    ull hash(const std::string &key);
    ull hash2(const std::string &key);
    void rehash();
    
    
    std::vector<std::string> table;
    
    void growHashTable();
    
    
};

OpenAdressedHashTable:: OpenAdressedHashTable() {
    for( ull i = 0; i < defaultTableSize; ++i ) {
        table.push_back(NullString);
    }
}
OpenAdressedHashTable:: OpenAdressedHashTable( ull tableSize) {
    for( ull i = 0;i < tableSize; ++i ) {
        table.push_back(NullString);
    }
}

OpenAdressedHashTable:: ~OpenAdressedHashTable() {
    
}


ull OpenAdressedHashTable::hash(const std::string &key){
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
    
    return hash % table.size();
}

ull OpenAdressedHashTable:: hash2(const std::string &key) {
    char *str = new char[key.size()];
    memcpy(str, key.data(), key.length() + 1);
    unsigned int hash = 0;
    
    for(; *str; str++)
    {
        hash += (unsigned char)(*str);
        hash -= (hash << 13) | (hash >> 19);
    }
    
    return hash;
}

void OpenAdressedHashTable:: rehash() {
    std::vector<std::string> keys;
    for( ull i = 0; i < table.size(); ++i ) {
        if( table[i] != DelString && table[i] != NullString ) {
            keys.push_back(table[i]);
        }
    }
    
    growHashTable();
    
    for( ull i = 0; i < keys.size(); ++i ) {
        add(keys[i]);
    }
    
}

void OpenAdressedHashTable:: growHashTable() {
    ull newSize = table.size() * 2;
    table.assign(newSize, NullString);
}


void OpenAdressedHashTable:: add(const std::string &key) {
    ull index = hash(key);
    ull step  = hash2(key);
    for( ull i = 0; i < table.size() ; ++i ) {
        index = (index + step) % table.size();
        if( table[index] == NullString || table[index] == DelString) {
            table[index] = key;
            return;
        }
    }
    
    rehash();
    add(key);
}

bool OpenAdressedHashTable:: contains(const std::string &key) {
    ull index = hash(key);
    ull step  = hash2(key);
    for( ull i = 0; i < table.size(); ++i ) {
        index = (index + step) % table.size();
        
        if( table[index] == key ) {
            return true;
        }
        if( table[index] == NullString ) {
            return false;
        }
    }
    return false;
}

void OpenAdressedHashTable:: remove(const std::string &key) {
    ull index = hash(key);
    ull step  = hash2(key);
    for( ull i = 0; i < table.size(); ++i ) {
        index = (index + step) % table.size();
        if( table[index] == NullString ) {
            return;
        }
        if( table[index] == key ) {
            table[index] = DelString;
        }
        
    }
}


int main(int argc, const char * argv[]) {
    OpenAdressedHashTable hashTable;
    char ch;
    std::string key;
    while( !std::cin.fail() ) {
        ch = EOF;
        std::cin>> ch >> key;
        switch (ch) {
            case '+':
                if( hashTable.contains(key) ) {
                    std::cout<<"FAIL" <<std::endl;
                } else {
                    hashTable.add(key);
                    std::cout<<"OK" <<std::endl;
                }
                break;
            case '?':
                if( hashTable.contains(key) ) {
                    std::cout<<"OK" <<std::endl;
                } else {
                    std::cout<<"FAIL" <<std::endl;
                }
                break;
            case '-':
                if( hashTable.contains(key) ) {
                    hashTable.remove(key);
                    std::cout<<"OK" <<std::endl;
                } else {
                    std::cout<<"FAIL" <<std::endl;
                }
                break;
            default:
                exit(0);
                try {
                    throw "Incorrect operation";
                } catch (char * errorMsg) {
                    std::cerr<< errorMsg <<std::endl;
                    exit(1);
                }
                break;
        }
    }
    return 0;
}
