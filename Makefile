CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = index_remissivo
SRCS = main.c trie.c hash.c file_utils.c index.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)