SRCS = $(shell find -name '*.c')
OBJS = $(addsuffix .o,$(basename $(SRCS)))

CC = gcc
LD = ld

ASFLAGS = -m32
CFLAGS	= -m32 -Wall -g -fno-stack-protector -nostdlib -nostdinc -static -I../../Libraries/libkernel -L../../Libraries/libkernel -TMakefile_ld

shell.bin: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lkernel

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^ 

clean:
	rm $(OBJS)

.PHONY: clean