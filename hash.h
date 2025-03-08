#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>
#include <string.h>

// Estrutura para armazenar as posições em que a palavra aparece
typedef struct PositionNodeHash
{
    int position;
    struct PositionNodeHash *next;
} PositionNodeHash;

// Estrutura para um elemento da tabela hash
typedef struct HashEntry
{
    wchar_t *keyword;
    PositionNodeHash *positions;
    int isOccupied; // 0: vazio, 1: ocupado, -1: deletado
} HashEntry;

// Estrutura para a tabela hash
typedef struct HashTable
{
    HashEntry *entries;
    int size;
} HashTable;

// Funções da tabela hash
HashTable *createHashTable(int size);
void printHashTable(HashTable *table);
unsigned int hashFunction(const wchar_t *str, int tableSize);
void insertKeywordHash(HashTable *table, const wchar_t *word);
void addPositionHash(PositionNodeHash **head, int position);
void processTextHash(HashTable *table, wchar_t word[256], int logicalPosition);
void printPositionsHash(PositionNodeHash *node);
void printIndexHash(HashTable *table);
void freeHashTable(HashTable *table);
int compareHashEntries(const void *a, const void *b);

#endif /* HASH_H */