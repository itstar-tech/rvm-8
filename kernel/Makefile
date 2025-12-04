CC = gcc

CFLAGS = -Wall -O2 -fPIC

SOURCES = cpu.c opcodes.c
OBJS = $(SOURCES:.c=.o)

all: libkernel.a

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

libkernel.a: $(OBJS)
	ar rcs $@ $^

clean:
	rm -f $(OBJS) libkernel.a

.PHONY: tests

tests: libkernel.a
	$(CC) $(CFLAGS) -o test_cpu tests/test_cpu.c libkernel.a
	./test_cpu