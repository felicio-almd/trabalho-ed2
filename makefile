all: indice

indice: processamento.o trie.o comum.o main.o
	gcc -Wall -Wextra -g -o indice processamento.o trie.o comum.o main.o

processamento.o: processamento.c processamento.h
	gcc -Wall -Wextra -g -c processamento.c

trie.o: trie.c trie.h
	gcc -Wall -Wextra -g -c trie.c

comum.o: comum.c comum.h trie.h processamento.h
	gcc -Wall -Wextra -g -c comum.c

main.o: main.c trie.h
	gcc -Wall -Wextra -g -c main.c

clean:
	rm -f *.o indice