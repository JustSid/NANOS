SRCS = $(shell find -name '*.c')
OBJS = $(addsuffix .o,$(basename $(SRCS)))

CC = gcc
LD = ld

ASFLAGS = -m32
CFLAGS	= -m32 -Wall -g -fno-stack-protector -nostdinc -static -I../../Libraries/libkernel
LDFLAGS = -melf_i386 ../../Libraries/libkernel/libkernel.a -TMakefile_ld

test.bin: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^ 

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^ 

clean:
	rm $(OBJS)

.PHONY: clean