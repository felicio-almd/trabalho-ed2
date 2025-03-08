#ifndef TRIE_H
#define TRIE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>

// Estruturas

typedef struct PositionNode
{
    int position;
    struct PositionNode *next;
} PositionNode;

typedef struct TrieNode
{
    struct TrieNode *children[128]; // Aumentado para acomodar caracteres acentuados e é 128 porque?
    wchar_t *keyword;
    PositionNode *positions;
} TrieNode;

// Funções

TrieNode *createTrieNode();
int get_char_index(wchar_t c);
void insertKeyword(TrieNode *root, const wchar_t *word);
void addPosition(PositionNode **head, int position);
void processText(TrieNode *root, const char *filename);
void printPositions(PositionNode *node);
void printIndex(TrieNode *node);
void printTrie(TrieNode *root, wchar_t *buffer, int depth, int indentation);
void freeTrie(TrieNode *node);

#endif // TRIE_H