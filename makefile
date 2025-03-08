all: indice

indice: processamento.o trie.o hash.o main.o
	gcc -Wall -Wextra -g -o indice processamento.o trie.o hash.o main.o

processamento.o: processamento.c processamento.h
	gcc -Wall -Wextra -g -c processamento.c

trie.o: trie.c trie.h
	gcc -Wall -Wextra -g -c trie.c

hash.o: hash.c hash.h
	gcc -Wall -Wextra -g -c hash.c

main.o: main.c trie.h
	gcc -Wall -Wextra -g -c main.c

clean:
	rm -f *.o indice