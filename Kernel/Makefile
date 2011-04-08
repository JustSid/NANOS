SRCS = $(shell find -name '*.c')
SRCS += $(shell find -name '*.S')
OBJS = $(addsuffix .o, $(basename $(SRCS)))

CC  = gcc
LD  = ld


ASFLAGS  = -m32
CFLAGS 	 = -m32 -Wall -g -fno-stack-protector -std=c99 -nostdlib -fno-builtin -fno-exceptions -fno-leading-underscore -Wall -Wextra -pedantic-errors -I.
LDFLAGS  = -melf_i386 -Ttext=0x100000 -TMakefile_ld



nanos: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

%.o: %.S
	$(CC) $(ASFLAGS) -c -o $@ $^



clean:
	rm $(OBJS)

.PHONY: clean

