#ifndef HASH_H
#define HASH_H

#include <stdbool.h>

#define TABLE_SIZE 100
#define MAX_KEYWORD_LENGTH 50

typedef struct HashNode
{
    char word[MAX_KEYWORD_LENGTH];
    int positions[100];
    int position_count;
} HashNode;

extern HashNode hash_table[TABLE_SIZE];

int hash_function(const char *word);
void insert_hash(const char *word, int position);
HashNode *search_hash(const char *word);
void delete_index_hash();

#endif 