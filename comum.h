#ifndef COMUM_H
#define COMUM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>

void ProcessamentoInicial(int type, const char *filename);
void ProcessTextG(int type, const char *filename);
TrieNode OpenTrie();

#endif