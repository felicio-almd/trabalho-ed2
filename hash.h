/* hash.h */
#ifndef HASH_H
#define HASH_H

#include <wchar.h>
#include <stddef.h>

#define HASH_LOAD_FACTOR 0.7

// Estrutura para lista de posições
typedef struct PositionNodeHash
{
    int position;
    struct PositionNodeHash *next;
} PositionNodeHash;

// Entrada da tabela hash
typedef struct HashEntry
{
    wchar_t *keyword;
    PositionNodeHash *positions;
    int is_active; // 0: vazio, 1: ativo, 2: removido
} HashEntry;

// Tabela hash
typedef struct HashTable
{
    HashEntry *entries;
    int size;
    int count;
} HashTable;

// Protótipos das funções
HashTable *createHashTable(int size);
void insertKeywordHash(HashTable *table, const wchar_t *word);
HashEntry *searchHashTable(const HashTable *table, const wchar_t *word);
void processTextHash(HashTable *table, const char *filename);
void printIndexHash(const HashTable *table);
void freeHashTable(HashTable *table);

#endif