#ifndef TRIE_H
#define TRIE_H

#include <stdbool.h>

#define ALPHABET_SIZE 26

typedef struct TrieNode
{
    struct TrieNode *children[ALPHABET_SIZE];
    bool is_end_of_word;
    int positions[100];
    int position_count;
} TrieNode;

TrieNode *create_node();
void insert_trie(TrieNode *root, const char *word, int position);
TrieNode *search_trie(TrieNode *root, const char *word);
void delete_index_trie(TrieNode *root);

#endif // TRIE_H