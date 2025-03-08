#include "comum.h"
#include "processamento.h"
#include "trie.h"

TrieNode *OpenTrie(wchar_t **palavras, int num)
{
    TrieNode *root = createTrieNode();
    for (int i = 0; i < num; i++)
    {
        insertKeyword(root, palavras[i]);
    }
    return root;
}

TrieNode *processText(char buscaType[5], const char *filename)
{
    DadosProcessados *dados = processar_arquivos("palavras_chave.txt",filename);
    if(!dados){
        return NULL;
    }
    wchar_t *text = dados->texto;
    size_t textLen = wcslen(text);
    int logicalPosition = 0;
    int trie = 0;
    TrieNode *root;
    if (strcmp(buscaType, "trie") == 0){
        trie = 1;
        root = OpenTrie(dados->palavras_chave,dados->num_palavras);
    }else if(strcmp(buscaType, "hash") == 0){
        return NULL;
    }
    for (size_t i = 0; i < textLen;)
    {
        wchar_t word[256];
        int wordLen = 0;
        size_t j;

        for (j = i; j < textLen && iswalpha(text[j]) && wordLen < 255; j++)
        {
            word[wordLen++] = towlower(text[j]);
        }
        word[wordLen] = L'\0';
        if(wordLen>0){
            if(trie == 1){
                processTextTrie(root, word, logicalPosition, wordLen);
                i = i+wordLen-1;
                logicalPosition = logicalPosition+wordLen-1;
            }
        }
        logicalPosition++;
        i++;
    }
    return root;
}
