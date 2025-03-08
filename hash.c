#include "hash.h"

int isPrime(int num)
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
int nextPrime(int n)
{
    while (!isPrime(n))
    {
        n++;
    }
    return n;
}

// Função para criar uma nova tabela hash com tamanho primo
HashTable *createHashTable(int size)
{
    int primeSize = nextPrime(size); // Garante que o tamanho seja primo

    HashTable *table = (HashTable *)malloc(sizeof(HashTable));
    table->size = primeSize;
    table->entries = (HashEntry *)calloc(primeSize, sizeof(HashEntry));

    // Inicializa todas as entradas como vazias
    for (int i = 0; i < primeSize; i++)
    {
        table->entries[i].keyword = NULL;
        table->entries[i].positions = NULL;
        table->entries[i].isOccupied = 0;
    }

    printf("Criada tabela hash com tamanho primo: %d\n", primeSize);
    return table;
}

// Função de hash para strings wide
unsigned int hashFunction(const wchar_t *str, int tableSize)
{
    unsigned int hash = 0;

    while (*str)
    {
        hash = (hash * 31) + (unsigned int)(*str);
        str++;
    }

    return hash % tableSize;
}

// Função para normalizar uma palavra (converter para minúsculo)
wchar_t *normalizeWord(const wchar_t *word)
{
    size_t len = wcslen(word);
    wchar_t *normalized = (wchar_t *)malloc((len + 1) * sizeof(wchar_t));

    for (size_t i = 0; i < len; i++)
    {
        normalized[i] = towlower(word[i]);
    }
    normalized[len] = L'\0';

    return normalized;
}

void printHashTable(HashTable *table)
{
    if (table == NULL)
    {
        printf("Tabela hash não existe.\n");
        return;
    }

    printf("Tabela Hash (Tamanho: %d):\n", table->size);
    for (int i = 0; i < table->size; i++)
    {
        HashEntry *entry = &table->entries[i];
        printf("[%4d] ", i); // Formatação para índices de até 4 dígitos

        if (entry->isOccupied == 1)
        {
            printf("Ocupado -> Palavra: \"%ls\"", entry->keyword);
            printf("\n");
        }
        else if (entry->isOccupied == -1)
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
void insertKeywordHash(HashTable *table, const wchar_t *word)
{
    wchar_t *normalized = normalizeWord(word);
    unsigned int index = hashFunction(normalized, table->size);
    int originalIndex = index;

    // Procura uma posição livre usando hashing linear
    while (table->entries[index].isOccupied == 1 &&
           (table->entries[index].keyword == NULL ||
            wcscmp(table->entries[index].keyword, normalized) != 0))
    {
        // Próxima posição (hashing linear)
        index = (index + 1) % table->size;

        // Se der a volta completa, a tabela está cheia
        if (index == originalIndex)
        {
            fprintf(stderr, "Tabela hash cheia!\n");
            free(normalized);
            return;
        }
    }

    // Se encontrou uma posição com a mesma palavra ou uma posição livre
    if (table->entries[index].isOccupied == 0 ||
        table->entries[index].isOccupied == -1 ||
        (table->entries[index].keyword != NULL &&
         wcscmp(table->entries[index].keyword, normalized) == 0))
    {

        // Libera a palavra anterior se estiver ocupada
        if (table->entries[index].isOccupied == 1 && table->entries[index].keyword != NULL)
        {
            free(table->entries[index].keyword);
        }

        table->entries[index].keyword = wcsdup(normalized);
        table->entries[index].isOccupied = 1;
    }
    free(normalized);
}

// Função para adicionar uma posição à lista de posições
void addPositionHash(PositionNodeHash **head, int position)
{
    position++; // Começa do 1

    PositionNodeHash *newNode = (PositionNodeHash *)malloc(sizeof(PositionNodeHash));
    newNode->position = position;

    if (*head == NULL || (*head)->position > position)
    {
        newNode->next = *head;
        *head = newNode;
        return;
    }

    PositionNodeHash *current = *head;
    while (current->next != NULL && current->next->position < position)
    {
        current = current->next;
    }

    // Verifica se a posição já existe
    if (current->position == position)
    {
        free(newNode);
        return;
    }

    // Insere ordenadamente
    newNode->next = current->next;
    current->next = newNode;
}

// Função para buscar uma palavra na tabela hash
HashEntry *findKeywordHash(HashTable *table, const wchar_t *word)
{
    wchar_t *normalized = normalizeWord(word);
    unsigned int index = hashFunction(normalized, table->size);
    int originalIndex = index;

    // Procura a palavra usando hashing linear
    while (table->entries[index].isOccupied != 0)
    {
        if (table->entries[index].isOccupied == 1 &&
            table->entries[index].keyword != NULL &&
            wcscmp(normalizeWord(table->entries[index].keyword), normalized) == 0)
        {
            free(normalized);
            return &table->entries[index];
        }

        // Próxima posição (hashing linear)
        index = (index + 1) % table->size;

        // Se der a volta completa, a palavra não está na tabela
        if (index == originalIndex)
        {
            break;
        }
    }

    free(normalized);
    return NULL;
}

// Função para processar o texto e adicionar as posições das palavras-chave
void processTextHash(HashTable *table, wchar_t word[256], int logicalPosition)
{
    HashEntry *entry = findKeywordHash(table, word);
    if (entry)
    {
        addPositionHash(&entry->positions, logicalPosition);
    }
}

// Imprime as posições de uma palavra
void printPositionsHash(PositionNodeHash *node)
{
    while (node)
    {
        printf("%d", node->position);
        node = node->next;
        if (node)
            printf(" ");
    }
    printf("\n");
}

// Função auxiliar para comparar duas entradas da tabela hash
int compareHashEntries(const void *a, const void *b)
{
    HashEntry *entryA = *(HashEntry **)a;
    HashEntry *entryB = *(HashEntry **)b;

    return wcscmp(entryA->keyword, entryB->keyword);
}

// Imprime todas as palavras-chave e suas posições
void printIndexHash(HashTable *table)
{
    // Primeiro, contamos quantas entradas ocupadas existem
    int count = 0;
    for (int i = 0; i < table->size; i++)
    {
        if (table->entries[i].isOccupied == 1 && table->entries[i].keyword)
        {
            count++;
        }
    }

    // Criamos um array temporário para armazenar ponteiros para as entradas ocupadas
    HashEntry **occupied = (HashEntry **)malloc(count * sizeof(HashEntry *));
    if (occupied == NULL)
    {
        printf("Erro de alocação de memória\n");
        return;
    }

    // Preenchemos o array com ponteiros para as entradas ocupadas
    int index = 0;
    for (int i = 0; i < table->size; i++)
    {
        if (table->entries[i].isOccupied == 1 && table->entries[i].keyword)
        {
            occupied[index++] = &(table->entries[i]);
        }
    }

    // Ordenamos o array usando qsort e uma função de comparação
    qsort(occupied, count, sizeof(HashEntry *), compareHashEntries);

    // Imprimimos as entradas em ordem alfabética
    for (int i = 0; i < count; i++)
    {
        printf("%ls: ", occupied[i]->keyword);
        printPositionsHash(occupied[i]->positions);
    }

    // Liberamos a memória alocada
    free(occupied);
}

// Libera a memória da tabela hash
void freeHashTable(HashTable *table)
{
    for (int i = 0; i < table->size; i++)
    {
        if (table->entries[i].keyword)
        {
            free(table->entries[i].keyword);
        }

        PositionNodeHash *curr = table->entries[i].positions;
        while (curr)
        {
            PositionNodeHash *temp = curr;
            curr = curr->next;
            free(temp);
        }
    }

    free(table->entries);
    free(table);
}