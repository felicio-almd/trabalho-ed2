#include "hash.h"
#include <string.h>

HashNode hash_table[TABLE_SIZE];

int hash_function(const char *word)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *word++))
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash % TABLE_SIZE;
}

void insert_hash(const char *word, int position)
{
    int index = hash_function(word);
    while (hash_table[index].position_count > 0 && strcmp(hash_table[index].word, word) != 0)
    {
        index = (index + 1) % TABLE_SIZE;
    }
    strcpy(hash_table[index].word, word);
    hash_table[index].positions[hash_table[index].position_count++] = position;
}

HashNode *search_hash(const char *word)
{
    int index = hash_function(word);
    while (hash_table[index].position_count > 0)
    {
        if (strcmp(hash_table[index].word, word) == 0)
        {
            return &hash_table[index];
        }
        index = (index + 1) % TABLE_SIZE;
    }
    return NULL;
}

void delete_index_hash()
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        hash_table[i].position_count = 0;
    }
}