#ifndef COMUM_H
#define COMUM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>
#include "trie.h"
#include "processamento.h"

void ProcessamentoInicial(int type, const char *filename);
TrieNode *processText(char buscaType[5], const char *filename);
TrieNode *OpenTrie(wchar_t **palavras, int num);

#endif