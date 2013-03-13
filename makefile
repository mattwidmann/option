CFLAGS = -std=c11 -Wextra -Wno-unused-parameter -g -O3
LDLIBS =
CC = clang

.PHONY: all
all: option.o

.PHONY: clean
clean:
	rm -f *.o option_test
	rm -rf *.dSYM

.PHONY: check
check: test
	# valgrind
	# clang --static-analysis

.PHONY: profile
profile: benchmark
	# gprof
	# callgrind

.PHONY: test
test: option.o
	$(CC) $(CFLAGS) option.o test/option_test.c -o option_test

%.o: src/%.c src/%.h
	$(CC) $(CFLAGS) -c src/$*.c
