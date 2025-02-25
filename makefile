all: indice
indice: trie.o main.o
	gcc -Wall -Wextra -g -o indice trie.o main.o
trie.o: trie.c trie.h
	gcc -Wall -Wextra -g -c trie.c
main.o: main.c trie.h
	gcc -Wall -Wextra -g -c main.c