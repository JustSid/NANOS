SRCS = $(shell find -name '*.[cS]')
OBJS = $(addsuffix .o, $(basename $(SRCS)))

CC = gcc
LD = ld

ASFLAGS = -m32
CFLAGS 	= -m32 -Wall -g -fno-stack-protector -I Include
LDFLAGS = -melf_i386 -Ttext=0x100000 -TMakefile.ld

nanos: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

%.o: %.S
	$(CC) $(ASFLAGS) -c -o $@ $^

clean:
	rm $(OBJS)

.PHONY: clean

