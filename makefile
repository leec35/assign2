#Makefile

#Variables
INCLUDES=-I.
CC=gcc
CFLAGS= -I. -g -Wall -c $(INCLUDES)
LINKARGS=-lm -g
LIBS=-lm

#FILES
DEPS = header.h
OBJECT_FILES= cmsc257-assign2.o cmsc257-assign2-main.o

#Productions

all: assign2

assign2: $(OBJECT_FILES)
	$(CC) $(LINKARGS) $(OBJECT_FILES) -o $@ $(LIBS)

cmsc257-assign2-main.o : driver.c header.h
	$(CC) $(CFLAGS) -o $@ $<

cmsc257-assign2.o : functions.c header.h
	$(CC) $(CFLAGS) -o $@ $<
clean :
	rm -f assign2 $(OBJECT_FILES)

