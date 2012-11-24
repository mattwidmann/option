CFLAGS = -std=c11 -Wextra -Wno-unused-parameter -g -O3
LDLIBS =
CC = clang

.PHONY: all
all: option.o

.PHONY: clean
clean:
	rm -f *.o

.PHONY: test
test: option.o

%.o: src/%.c src/%.h
	$(CC) $(CFLAGS) -c src/$*.c
