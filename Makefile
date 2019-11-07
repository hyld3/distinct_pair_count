CC=clang

default: all

all: main.c hashmap.c
	$(CC) $(CFLAGS) -o pair_count $^

clean:
	$(RM) pair_count
