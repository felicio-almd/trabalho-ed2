CC=gcc
CFLAGS=-Wall -Wextra -g
LDFLAGS=

SRCS=main.c trie.c
OBJS=$(SRCS:.c=.o)
TARGET=indice

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c trie.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean