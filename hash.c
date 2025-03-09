#include "hash.h"

int ehPrimo(int num)
{
    if (num < 2)
        return 0;
    if (num == 2 || num == 3)
        return 1;
    if (num % 2 == 0 || num % 3 == 0)
        return 0;

    for (int i = 5; i * i <= num; i += 6)
    {
        if (num % i == 0 || num % (i + 2) == 0)
            return 0;
    }
    return 1;
}

// Encontra o próximo número primo maior ou igual a "n"
int proximoPrimoMaior(int n)
{
    while (!ehPrimo(n))
    {
        n++;
    }
    return n;
}

// Função para criar uma nova tabela hash com tamanho primo
TabelaHash *criarTabelaHash(int tamanho)
{
    int tamanhoPrimo = proximoPrimoMaior(tamanho); // Garante que o tamanho seja primo

    TabelaHash *tabela = (TabelaHash *)malloc(sizeof(TabelaHash));
    tabela->tamanho = tamanhoPrimo;
    tabela->entradas = (EntradaHash *)calloc(tamanhoPrimo, sizeof(EntradaHash));

    // Inicializa todas as entradas como vazias
    for (int i = 0; i < tamanhoPrimo; i++)
    {
        tabela->entradas[i].palavraChave = NULL;
        tabela->entradas[i].posicoesHash = NULL;
        tabela->entradas[i].ocupado = 0;
    }

    printf("Criada tabela hash com tamanho primo: %d\n", tamanhoPrimo);
    return tabela;
}

// Função de hash para strings wide
/*
    Usa unsigned int evita ter valores negativos, assim so vai índices sempre válidos na tabela hash.
    Melhora a compatibilidade com operações bitwise e % tamanhoTabela, prevenindo erros.
    Amplia o intervalo de valores possíveis, ajudando na distribuição uniforme dos hashes.
    Assegura eficiência e segurança ao acessar a tabela hash.
*/
unsigned int funcaoHash(const wchar_t *string, int tamanhoTabela)
{
    // unsigned para usar so valores positivos
    unsigned int hash = 0;

    while (*string)
    {
        // unsigned para usar so valores positivos
        hash = (hash * 31) + (unsigned int)(*string);
        string++;
    }

    return hash % tamanhoTabela;
}

// Função para normalizar uma palavra (converter para minúsculo)
wchar_t *normalizeWord(const wchar_t *palavra)
{
    size_t tamanho = wcslen(palavra);
    wchar_t *normalizada = (wchar_t *)malloc((tamanho + 1) * sizeof(wchar_t));

    for (size_t i = 0; i < tamanho; i++)
    {
        normalizada[i] = towlower(palavra[i]);
    }
    normalizada[tamanho] = L'\0';

    return normalizada;
}

void imprimirTabelaHash(TabelaHash *tabela)
{
    if (tabela == NULL)
    {
        printf("Tabela hash não existe.\n");
        return;
    }

    printf("Tabela Hash (Tamanho: %d):\n", tabela->tamanho);
    for (int i = 0; i < tabela->tamanho; i++)
    {
        EntradaHash *entry = &tabela->entradas[i];
        printf("[%4d] ", i); // Formatação para índices de até 4 dígitos

        if (entry->ocupado == 1)
        {
            printf("Ocupado -> Palavra: \"%ls\"", entry->palavraChave);
            printf("\n");
        }
        else if (entry->ocupado == -1)
        {
            printf("Removido\n");
        }
        else
        {
            printf("Livre\n");
        }
    }
}

// Função para inserir uma palavra-chave na tabela hash
void inserirPalavraChaveHash(TabelaHash *table, const wchar_t *word)
{
    wchar_t *normalized = normalizeWord(word);
    unsigned int index = funcaoHash(normalized, table->tamanho);
    int originalIndex = index;

    // Procura uma posição livre usando hashing linear
    while (table->entradas[index].ocupado == 1 &&
           (table->entradas[index].palavraChave == NULL ||
            wcscmp(table->entradas[index].palavraChave, normalized) != 0))
    {
        // Próxima posição (hashing linear)
        index = (index + 1) % table->tamanho;

        // Se der a volta completa, a tabela está cheia
        if (index == originalIndex)
        {
            fprintf(stderr, "Tabela hash cheia!\n");
            free(normalized);
            return;
        }
    }

    // Se encontrou uma posição com a mesma palavra ou uma posição livre
    if (table->entradas[index].ocupado == 0 ||
        table->entradas[index].ocupado == -1 ||
        (table->entradas[index].palavraChave != NULL &&
         wcscmp(table->entradas[index].palavraChave, normalized) == 0))
    {

        // Libera a palavra anterior se estiver ocupada
        if (table->entradas[index].ocupado == 1 && table->entradas[index].palavraChave != NULL)
        {
            free(table->entradas[index].palavraChave);
        }

        table->entradas[index].palavraChave = wcsdup(normalized);
        table->entradas[index].ocupado = 1;
    }
    free(normalized);
}

// Função para adicionar uma posição à lista de posições
void adicionarPosicaoHash(NoPosicaoHash **cabeca, int posicaoHash)
{
    posicaoHash++; // Começa do 1

    NoPosicaoHash *newNode = (NoPosicaoHash *)malloc(sizeof(NoPosicaoHash));
    newNode->posicaoHash = posicaoHash;

    if (*cabeca == NULL || (*cabeca)->posicaoHash > posicaoHash)
    {
        newNode->proximo = *cabeca;
        *cabeca = newNode;
        return;
    }

    NoPosicaoHash *current = *cabeca;
    while (current->proximo != NULL && current->proximo->posicaoHash < posicaoHash)
    {
        current = current->proximo;
    }

    // Verifica se a posição já existe
    if (current->posicaoHash == posicaoHash)
    {
        free(newNode);
        return;
    }
    // Insere ordenadamente
    newNode->proximo = current->proximo;
    current->proximo = newNode;
}

// Função para buscar uma palavra na tabela hash
EntradaHash *findKeywordHash(TabelaHash *tabela, const wchar_t *palavra)
{
    wchar_t *palavraNormalizada = normalizeWord(palavra);
    unsigned int index = funcaoHash(palavraNormalizada, tabela->tamanho);
    int originalIndex = index;

    // Procura a palavra usando hashing linear
    while (tabela->entradas[index].ocupado != 0)
    {
        if (tabela->entradas[index].ocupado == 1 &&
            tabela->entradas[index].palavraChave != NULL &&
            wcscmp(normalizeWord(tabela->entradas[index].palavraChave), palavraNormalizada) == 0)
        {
            free(palavraNormalizada);
            return &tabela->entradas[index];
        }

        // Próxima posição (hashing linear)
        index = (index + 1) % tabela->tamanho;

        // Se der a volta completa, a palavra não está na tabela
        if (index == originalIndex)
        {
            break;
        }
    }

    free(palavraNormalizada);
    return NULL;
}

// Função para processar o texto e adicionar as posições das palavras-chave
void processarTextoHash(TabelaHash *tabela, wchar_t palavra[1000], int posicaoLogica)
{
    EntradaHash *entry = findKeywordHash(tabela, palavra);
    if (entry)
    {
        adicionarPosicaoHash(&entry->posicoesHash, posicaoLogica);
    }
}

// Imprime as posições de uma palavra
void imprimirPosicoesHash(NoPosicaoHash *no)
{
    while (no)
    {
        printf("%d", no->posicaoHash);
        no = no->proximo;
        if (no)
            printf(" ");
    }
    printf("\n");
}

// Função auxiliar para comparar duas entradas da tabela hash
int compararEntradasHash(const void *a, const void *b)
{
    EntradaHash *entryA = *(EntradaHash **)a;
    EntradaHash *entryB = *(EntradaHash **)b;

    return wcscmp(entryA->palavraChave, entryB->palavraChave);
}

// Imprime todas as palavras-chave e suas posições
void imprimirIndiceHash(TabelaHash *tabela)
{
    // Primeiro, contamos quantas entradas ocupadas existem
    int count = 0;
    for (int i = 0; i < tabela->tamanho; i++)
    {
        if (tabela->entradas[i].ocupado == 1 && tabela->entradas[i].palavraChave)
        {
            count++;
        }
    }
    // usa um array temporário para armazenar ponteiros para as entradas ocupadas
    EntradaHash **occupied = (EntradaHash **)malloc(count * sizeof(EntradaHash *));
    if (occupied == NULL)
    {
        printf("Erro de alocação de memória\n");
        return;
    }
    // Preenchemos o array com ponteiros para as entradas ocupadas
    int index = 0;
    for (int i = 0; i < tabela->tamanho; i++)
    {
        if (tabela->entradas[i].ocupado == 1 && tabela->entradas[i].palavraChave)
        {
            occupied[index++] = &(tabela->entradas[i]);
        }
    }
    // Ordenamos o array usando qsort e uma função de comparação
    qsort(occupied, count, sizeof(EntradaHash *), compararEntradasHash);
    // Imprimimos as entradas em ordem alfabética
    for (int i = 0; i < count; i++)
    {
        printf("%ls: ", occupied[i]->palavraChave);
        imprimirPosicoesHash(occupied[i]->posicoesHash);
    }
    // Liberamos a memória alocada
    free(occupied);
}

// Libera a memória da tabela hash
void freeTabelaHash(TabelaHash *tabela)
{
    for (int i = 0; i < tabela->tamanho; i++)
    {
        if (tabela->entradas[i].palavraChave)
        {
            free(tabela->entradas[i].palavraChave);
        }

        NoPosicaoHash *curr = tabela->entradas[i].posicoesHash;
        while (curr)
        {
            NoPosicaoHash *temp = curr;
            curr = curr->proximo;
            free(temp);
        }
    }

    free(tabela->entradas);
    free(tabela);
}