all: indice
indice: trie.o hash.o main.o
	gcc -Wall -Wextra -g -o indice trie.o hash.o main.o
trie.o: trie.c trie.h
	gcc -Wall -Wextra -g -c trie.c
hash.o: hash.c hash.h
	gcc -Wall -Wextra -g -c hash.c
main.o: main.c trie.h hash.h
	gcc -Wall -Wextra -g -c main.c