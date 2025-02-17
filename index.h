#ifndef INDEX_H
#define INDEX_H

#include "trie.h"
#include "hash.h"

void create_index_trie(TrieNode *root, const char *text);
void create_index_hash(const char *text);
void print_index_trie(TrieNode *root);
void print_index_hash();

#endif // INDEX_H