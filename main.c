#include "trie.h"
#include "hash.h"
#include "processamento.h"

TrieNode *OpenTrie(wchar_t **palavras, int numPalavrasChave)
{
    TrieNode *raiz = createTrieNode();
    for (int i = 0; i < numPalavrasChave; i++)
    {
        insertKeyword(raiz, palavras[i]);
    }
    return raiz;
}

HashTable *OpenHash(wchar_t **palavras, int numPalavrasChave)
{
    HashTable *table = createHashTable(numPalavrasChave);
    for (int i = 0; i < numPalavrasChave; ++i)
    {
        insertKeywordHash(table, palavras[i]);
    }
    return table;
}

void ProcessText(TrieNode *trie, HashTable *table, DadosProcessados *dados)
{
    wchar_t *texto = dados->texto;
    size_t tamanhoTexto = wcslen(texto);
    int posicaoLogica = 0;
    for (size_t i = 0; i < tamanhoTexto;)
    {
        wchar_t palavra[256];
        int tamanhoPalavra = 0;
        size_t j;

        for (j = i; j < tamanhoTexto && iswalpha(texto[j]) && tamanhoPalavra < 255; j++)
        {
            palavra[tamanhoPalavra++] = towlower(texto[j]);
        }
        palavra[tamanhoPalavra] = L'\0';
        if (tamanhoPalavra > 0)
        {
            if (trie != NULL)
            {
                processTextTrie(trie, palavra, posicaoLogica, tamanhoPalavra);
                i = i + tamanhoPalavra - 1;
                posicaoLogica = posicaoLogica + tamanhoPalavra - 1;
            }
            if (table != NULL)
            {
                processTextHash(table, palavra, posicaoLogica);
            }
        }
        posicaoLogica++;
        i++;
    }
}

void exibirMenu()
{
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

int main()
{
    setlocale(LC_ALL, "pt_BR.UTF-8");
    int opcao = -1;
    char arquivoTexto[256];
    char arquivoPalavrasChave[256];
    wchar_t palavraChave[256];
    DadosProcessados *dados = NULL;
    TrieNode *raiz = NULL;
    HashTable *table = NULL;

    while (opcao != 0)
    {
        exibirMenu();
        scanf("%d", &opcao);
        switch (opcao)
        {
        case 1:
            printf("\nDigite o nome do arquivo de texto: ");
            scanf("%s", arquivoTexto);
            printf("\nDigite o nome do aruivo de palavras chave: ");
            scanf("%s", arquivoPalavrasChave);
            dados = processar_arquivos(arquivoPalavrasChave, arquivoTexto);
            if (!dados)
            {
                printf("\nErro nos dados dos arquivos.\n\n");
            }
            else
            {
                printf("\nArquivos carregados com sucesso.\n");
            }
            break;
        case 2:
            if (dados)
            {
                if (table)
                {
                    freeHashTable(table);
                    table = NULL;
                }
                table = OpenHash(dados->palavras_chave, dados->num_palavras);
                ProcessText(NULL, table, dados);
                if (!table)
                {
                    printf("\nErro no indice remissivo com hash.\n");
                }
            }
            else
            {
                printf("\nPor favor carregue nomes dos arquivos...\n\n");
            }
            break;
        case 3:
            if (dados)
            {
                if (raiz)
                {
                    freeTrie(raiz);
                    raiz = NULL;
                }
                raiz = OpenTrie(dados->palavras_chave, dados->num_palavras);
                ProcessText(raiz, NULL, dados);
                if (!raiz)
                {
                    printf("\nErro no indice remissivo com arvore digital.\n");
                }
            }
            else
            {
                printf("\nPor favor carregue nomes dos arquivos.\n");
            }
            break;
        case 4:
            if (table)
            {
                printf("\nIndice remissivo com Tabela Hash:\n");
                printIndexHash(table);
                printf("\n\n");
            }
            else
            {
                printf("\nNão há tabela hash.\n");
            }
            if (raiz)
            {
                printf("\nIndice remissivo com arvore digital:\n");
                printIndex(raiz);
                printf("\n\n");
            }
            else
            {
                printf("\nNão há Arvore Digital.\n");
            }
            break;
        case 5:
            if (table)
            {
                printHashTable(table);
            }
            else
            {
                printf("\nNão há tabela hash.\n");
            }
            break;
        case 6:
            if (raiz)
            {
                printTrie(raiz, palavraChave, 0, 1);
            }
            else
            {
                printf("\nNão há Arvore Digital.\n");
            }
            break;
        case 7:
            if (table)
            {
                freeHashTable(table);
                table = NULL;
            }
            if (raiz)
            {
                freeTrie(raiz);
                raiz = NULL;
            }
            break;
        case 0:
            printf("\nFinalizando programa...\n");
            break;
        default:
            printf("\nOpção invalida.\n");
            break;
        }
    }

    return 0;
}
