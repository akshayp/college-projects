# Makefile for Collision Avoidance
#
#  (c) 2009  Scott & Linda Wills

# The compiler being used

CC= gcc

# compiler options

#CFLAGS= -O2
CFLAGS= -g -Wall

# linker

LN= $(CC)

# linker options

LNFLAGS= -g

# extra libraries used in linking (use -l command)

LDLIBS= /usr/local/lib/libjpeg.a

#LDLIBS= /usr/lib/libjpeg.so.62

# source files

SOURCES= utils.c chroma.c rollers.c report.c P2-4.c

# include files

INCLUDES= utils.h chroma.h rollers.h report.h

# object files

OBJECTS= utils.o chroma.o rollers.o report.o P2-4.o

all: P2-4

.c.o:	$*.c
	$(CC) $(CFLAGS) -c $*.c

P2-4:	$(OBJECTS)
	$(LN) $(LNFLAGS) -o $@ $(OBJECTS) $(LDLIBS)

clean:
	rm -f *.o
	rm -f *~

s01:	P2-4
	./P2-4 "20090902-04e" 1 100 33
	
utils.o:	utils.c utils.h
chroma.o:   chroma.c chroma.h utils.h
roller.o:	rollers.c rollers.h utils.h
report.o:	report.c report.h utils.h
P2-4.o:		P2-4.c utils.h chroma.o rollers.h report.h
