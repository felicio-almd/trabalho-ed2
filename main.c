#include "trie.h"
#include "hash.h"
#include "processamento.h"

TrieNode *OpenTrie(wchar_t **palavras, int num)
{
    TrieNode *root = createTrieNode();
    for (int i = 0; i < num; i++)
    {
        insertKeyword(root, palavras[i]);
    }
    return root;
}

HashTable *OpenHash(wchar_t **palavras, int num){
    HashTable *table = createHashTable(num);
    for (int i = 0; i < num; ++i) {
        insertKeywordHash(table,palavras[i]);
    }
    return table;
}

void exibirMenu() {
    printf("\n===== SISTEMA DE ÍNDICE REMISSIVO =====\n");
    printf("1. Carregar arquivo de texto\n");
    printf("2. Criar índice remissivo (Tabela Hash)\n");
    printf("3. Criar índice remissivo (Árvore de Pesquisa Digital)\n");
    printf("4. Imprimir índice remissivo\n");
    printf("5. Imprimir tabela hash\n");
    printf("6. Imprimir árvore de pesquisa digital\n");
    printf("7. Excluir índice remissivo\n");
    printf("0. Sair\n");
    printf("Escolha uma opção: ");
}

void ProcessText(TrieNode *trie, HashTable *table, DadosProcessados *dados){
    wchar_t *text = dados->texto;
    size_t textLen = wcslen(text);
    int logicalPosition = 0;
    for (size_t i = 0; i < textLen;) {
        wchar_t word[256];
        int wordLen = 0;
        size_t j;

        for (j = i; j < textLen && iswalpha(text[j]) && wordLen < 255; j++) {
            word[wordLen++] = towlower(text[j]);
        }
        word[wordLen] = L'\0';
        if (wordLen > 0) {
            if (trie != NULL) {
                processTextTrie(trie, word, logicalPosition, wordLen);
                i = i + wordLen - 1;
                logicalPosition = logicalPosition + wordLen - 1;
            }
            if (table != NULL) {
                processTextHash(table, word, logicalPosition);
            }
        }
        logicalPosition++;
        i++;
    }
}



int main()
{
    setlocale(LC_ALL, "pt_BR.UTF-8");
    int opcao = -1;
    char fileName[256];
    char keyWordsName[256];
    wchar_t keyword[256];
    DadosProcessados *dados = NULL;
    TrieNode *root = NULL;
    HashTable *table = NULL;

    while (opcao != 0){
        exibirMenu();
        scanf("%d", &opcao);
        switch (opcao) {
            case 1:
                printf("Digite o nome do arquivo de texto: ");
                scanf("%s", fileName);
                printf("Digite o nome do aruivo de palavras chave: ");
                scanf("%s", keyWordsName);
                dados = processar_arquivos(keyWordsName,fileName);
                if(!dados){
                    printf("erro nos dados dos arquivos.");
                }else{
                    printf("arquivos carregados com sucesso.");
                }
                break;
            case 2:
                if(dados){
                    if(table){
                        freeHashTable(table);
                        table = NULL;
                    }
                    table = OpenHash(dados->palavras_chave,dados->num_palavras);
                    ProcessText(NULL, table, dados);
                    if(!table){
                        printf("erro no indice remissivo com hash");
                    }
                }else{
                    printf("por favor carregue nomes dos arquivos");
                }
                break;
            case 3:
                if(dados){
                    if(root){
                        freeTrie(root);
                        root = NULL;
                    }
                    root = OpenTrie(dados->palavras_chave,dados->num_palavras);
                    ProcessText(root, NULL, dados);
                    if(!root){
                        printf("erro no indice remissivo com arvore digital");
                    }
                }else{
                    printf("por favor carregue nomes dos arquivos");
                }
                break;
            case 4:
                if(table){
                    printf("Indice remissivo com Hash:\n");
                    printIndexHash(table);
                    printf("\n\n");
                }else{
                    printf("Não há tabela hash");
                }
                if(root){
                    printf("Indice remissivo com arvore digital:\n");
                    printIndex(root);
                    printf("\n\n");
                }else{
                    printf("Não há arvore digital");
                }
                break;
            case 5:
                if(table){
                    printHashTable(table);
                } else{
                    printf("Não há tabela hash");
                }
                break;
            case 6:
                if(root){
                    printTrie(root,keyword,0,1);
                }
                break;
            case 7:
                if(table){
                    freeHashTable(table);
                    table = NULL;
                }
                if(root){
                    freeTrie(root);
                    root = NULL;
                }
                break;
            case 0:
                printf("finalizando programa...");
                break;
            default:
                printf("opcao invalida");
                break;
        }
    }


    return 0;
}
