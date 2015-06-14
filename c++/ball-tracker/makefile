# Makefile for Ball Tracking
#
#  31 October 2008    Scott Wills

# The compiler being used

CC= gcc

# compiler options

CFLAGS= -g

# linker

LN= $(CC)

# linker options

LNFLAGS= 

# extra libraries used in linking (use -l command)

LDLIBS= /usr/local/lib/libjpeg.a

# source files

SOURCES= utils.c mmm.c regmark.c P2.4.c rollers.c

# include files

INCLUDES= utils.h mmm.h regmark.h rollers.h

# object files

OBJECTS= utils.o mmm.o regmark.o rollers.o P2-4.o

all: P2-4

.c.o:
	$(CC) $(CFLAGS) -c $*.c

P2-4: $(OBJECTS)
	$(LN) $(LNFLAGS) -o $@ $(OBJECTS) $(LDLIBS)

clean:
	rm -f *.o
	rm -f *~

test: P2-4
	./P2-4 "20070127-11" 1 58


P2-4.o:		P2-4.c utils.c utils.h mmm.c mmm.h regmark.c regmark.h rollers.c rollers.h
utils.o:	utils.c utils.h
mmm.o:		mmm.c mmm.h
regmark.o:      regmark.c regmark.h
rollers.o:	rollers.c rollers.h mmm.h
