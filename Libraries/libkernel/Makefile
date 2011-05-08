SRCS = $(shell find -name '*.c')
OBJS = $(addsuffix .o, $(basename $(SRCS)))

CC  = gcc
AR	= ar

CFLAGS 	 = -m32 -Wall -g -fno-stack-protector -std=c99 -nostdlib -fno-builtin -fno-exceptions -fno-leading-underscore -Wall -Wextra -pedantic-errors
LDFLAGS  = -melf_i386 -Lstatic


libkernel.a: $(OBJS)
	$(AR) rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^



clean:
	rm $(OBJS)

.PHONY: clean

