#include "trie.h"
#include "hash.h"
#include "processamento.h"
#include <time.h>

NoTrie *iniciarTrie(wchar_t **palavras, int numPalavrasChave)
{
    NoTrie *raiz = criarNoTrie();
    for (int i = 0; i < numPalavrasChave; i++)
    {
        inserirPalavraChaveTrie(raiz, palavras[i]);
    }
    return raiz;
}

TabelaHash *iniciarTabelaHash(wchar_t **palavras, int numPalavrasChave)
{
    TabelaHash *tabela = criarTabelaHash(numPalavrasChave);
    for (int i = 0; i < numPalavrasChave; ++i)
    {
        inserirPalavraChaveHash(tabela, palavras[i]);
    }
    return tabela;
}

void processarTexto(NoTrie *trie, TabelaHash *tabela, DadosProcessados *dados)
{
    wchar_t *texto = dados->texto;
    size_t tamanhoTexto = wcslen(texto);
    int posicaoLogica = 0;
    for (size_t i = 0; i < tamanhoTexto;)
    {
        wchar_t palavra[1000];
        int tamanhoPalavra = 0;
        size_t j;

        // aceita caracteres, numero e alguns especiais, verifica caractere por caractere, depois deixa todos minusculos
        for (j = i; j < tamanhoTexto && (iswalpha(texto[j]) || texto[j] == L'@' || texto[j] == L'-') && tamanhoPalavra < 255; j++)
        {
            // case insensitive
            palavra[tamanhoPalavra++] = towlower(texto[j]);
        }
        palavra[tamanhoPalavra] = L'\0';
        if (tamanhoPalavra > 0)
        {
            if (trie != NULL)
            {
                processarTextoTrie(trie, palavra, posicaoLogica, tamanhoPalavra);
                i = i + tamanhoPalavra - 1;
                posicaoLogica = posicaoLogica + tamanhoPalavra - 1;
            }
            if (tabela != NULL)
            {
                processarTextoHash(tabela, palavra, posicaoLogica);
                i = i + tamanhoPalavra - 1;
                posicaoLogica = posicaoLogica + tamanhoPalavra - 1;
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
    wchar_t palavraChave[1000];
    DadosProcessados *dados = NULL;
    NoTrie *raiz = NULL;
    TabelaHash *tabela = NULL;

    // variaveis para tempo de execução do código
    clock_t inicio, fim;
    double tempoHash, tempoArvore;

    while (opcao != 0)
    {
        exibirMenu();
        if (scanf("%d", &opcao) != 1)
        {
            while (getchar() != '\n')
                ;
            printf("\nEntrada inválida. Por favor, digite um número.\n");
            continue;
        }
        switch (opcao)
        {
        case 1:
            printf("\nDigite o nome do arquivo de texto: ");
            scanf("%s", arquivoTexto);
            printf("\nDigite o nome do arquivo de palavras chave: ");
            scanf("%s", arquivoPalavrasChave);
            dados = processarArquivos(arquivoPalavrasChave, arquivoTexto);
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
                if (tabela)
                {
                    freeTabelaHash(tabela);
                    tabela = NULL;
                }
                inicio = clock();
                tabela = iniciarTabelaHash(dados->palavrasChave, dados->numPalavras);
                processarTexto(NULL, tabela, dados);
                fim = clock();
                tempoHash = ((double)fim - inicio) / CLOCKS_PER_SEC;
                if (!tabela)
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
                inicio = clock();
                raiz = iniciarTrie(dados->palavrasChave, dados->numPalavras);
                processarTexto(raiz, NULL, dados);
                fim = clock();
                tempoArvore = ((double)fim - inicio) / CLOCKS_PER_SEC;
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
            if (tabela)
            {
                printf("\nIndice remissivo com Tabela Hash:\n");
                imprimirIndiceHash(tabela);
                printf("\n");
                printf("Tempo gasto com a Hash: %f segundos", tempoHash);
                printf("\n\n");
            }
            else
            {
                printf("\nNão há tabela hash.\n");
            }
            if (raiz)
            {
                printf("\nIndice remissivo com arvore digital:\n");
                imprimirIndiceTrie(raiz);
                printf("\n");
                printf("Tempo gasto com a Trie: %f segundos", tempoArvore);
                printf("\n\n");
            }
            else
            {
                printf("\nNão há Arvore Digital.\n");
            }
            break;
        case 5:
            if (tabela)
            {
                imprimirTabelaHash(tabela);
            }
            else
            {
                printf("\nNão há tabela hash.\n");
            }
            break;
        case 6:
            if (raiz)
            {
                imprimirTrie(raiz, palavraChave, 0, 1);
            }
            else
            {
                printf("\nNão há Arvore Digital.\n");
            }
            break;
        case 7:
            if (tabela)
            {
                freeTabelaHash(tabela);
                tabela = NULL;
            }
            if (raiz)
            {
                freeTrie(raiz);
                raiz = NULL;
            }
            break;
        case 0:
            if (dados)
            {
                liberarDados(dados);
            }
            if (tabela)
            {
                freeTabelaHash(tabela);
            }
            if (raiz)
            {
                freeTrie(raiz);
            }
            printf("\nFinalizando programa...\n");
            break;
        default:
            printf("\nOpção invalida.\n");
            break;
        }
    }
    return 0;
}
