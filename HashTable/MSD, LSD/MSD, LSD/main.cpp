//
//  main.cpp
//  MSD, LSD
//
//  Created by Александр Малышев on 09.11.14.
//  Copyright (c) 2014 SecurityQQ. All rights reserved.
//

#include <iostream>
#include <assert.h>
#include <stdio.h>
#include <cstring>
#include <cstdlib>

#define show(arr, n) for( int i = 0; i < n; ++i ) {\
std::cout <<arr[i] <<"\n";\
}\

#define getArr(arr, n) while( arr[i] != "\0")\
gets(arr[i]);\
}

typedef long long ll;

template <class T>
void MSD(T* arr, int d, int sizeOfArray);

template <class T>
void LSD(T* arr, int d, int sizeOfArray);

template<class T>
void mergeSort (T* arr, int l, int r, bool comp(T a, T b, int d), int d);

template <class T>
void merge(T *arr, int left, int half, int right, bool comp(T a, T b, int d), int d);

bool compLSD(ll a, ll b, ll d) {
    for( int i = 1; i < d; ++i ) {
        a /= 10;
        b /= 10;
    }
    a %= 10;
    b %= 10;
    return a <= b;
}

int strLen(char* str) {
    int len = 0;
    while(str[len++] != '\0') {
        
    }
    return len - 1;
}

bool compMSD(char* str1, char* str2, int d) {
    //    int str1Len = strLen(str1);
    //    int str2Len = strLen(str2);
    //    if (str1Len > d && str2Len < d) return false;
    //    if (str1Len < d || str2Len < d) return true;
    if( str1[d] > str2[d] ) return false;
    return true;
}

bool compInt(int a, int b, int d) {
    return a < b;
}

bool isSorted(int* arr, int n) {
    for( int i = 0; i < n - 1; ++i ) {
        if( arr[i] > arr[i + 1] ) {
            return false;
        }
    }
    return true;
}


int main(int argc, const char * argv[]) {
    int n = 10000;
    int sz = 0;
    int maxLen = 0;
    char** foo = (char**) malloc(n * sizeof(char*));
    //    char* foo = (char*) malloc(257 * sizeof(char));
    for( int i = 0; i < n; ++i ) {
        foo[i] = (char*) malloc(257 * sizeof(char));
    }
    
    char** arr = (char**) malloc(sizeof(char*));
    while( gets(foo[sz]) ) {
        int sLen = strLen(foo[sz]);
        if( sLen > maxLen ) {
            maxLen = sLen;
        }
        arr = (char**) realloc(arr, (sz + 1) * sizeof(char*));
        arr[sz] = (char*) malloc((sLen + 1)* sizeof(char));
        memmove(arr[sz], foo[sz], sLen);
        arr[sz][sLen] = '\0';
        free(foo[sz]);
        if( foo[sz++][0] == '\0' ) {
            break;
        }
    }
    free(foo);
    --sz;
    
    MSD(arr, maxLen, sz);
    show(arr, sz);
    return 0;
}

template<class T>
void mergeSort (T* arr, int l, int r, bool comp(T a, T b, int d), int d) {
    if (r - l <= 1) {
        return;
    }
    int half = (l + ((r - l) >> 1));
    mergeSort (arr, l, half, comp, d);
    mergeSort (arr, half, r, comp, d);
    merge(arr, l, half, r, comp, d);
}

template <class T>
void merge(T *arr, int left, int half, int right, bool comp(T a, T b, int d), int d) {
    assert(left < right);
    assert(left >= 0);
    assert(half < right);
    assert(left < half);
    int l_size = half - left;
    int r_size = right - half;
    int l = 0;
    int r = 0;
    int sz = left;
    T* l_half = (T*) malloc (l_size * sizeof(T));
    T* r_half = (T*) malloc (r_size * sizeof(T));
    
    for( int i = left; i < half; ++i, ++l) {
        l_half[l] = arr[i];
    }
    for (int i = half; i < right; ++i, ++r) {
        r_half[r] = arr[i];
    }
    assert(l == l_size);
    assert(r == r_size);
    l = 0;
    r = 0;
    while (l < l_size && r < r_size) {
        if (comp( l_half[l], r_half[r], d)) {
            arr[sz++] = l_half[l++];
        } else {
            arr[sz++] = r_half[r++];
        }
    }
    for( ; l < l_size; ++l, ++sz) {
        arr[sz] = l_half[l];
    }
    for( ; r < r_size; ++r, ++sz) {
        arr[sz] = r_half[r];
    }
}

template <class T>
void LSD(T* arr, int d, int sizeOfArray) {
    for( int i = 1; i <= d; ++i) {
        mergeSort(arr, 0, sizeOfArray, compLSD, i);
    }
}

template <class T>
void MSD(T* arr, int d, int sizeOfArray) {
    for (int i = d - 1; i >= 0; --i) {
        mergeSort(arr, 0, sizeOfArray, compMSD, i);
    }
}