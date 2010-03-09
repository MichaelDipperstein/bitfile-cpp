############################################################################
# Makefile for file class library and sample
#
#   $Id: Makefile,v 1.2 2005/06/23 04:39:06 michael Exp $
#   $Log: Makefile,v $
#   Revision 1.2  2005/06/23 04:39:06  michael
#   Convert from DOS end of line to Unix end of line
#
#   Revision 1.1.1.1  2004/08/04 13:45:38  michael
#   bitfile class
#
############################################################################

CPP = g++
LD = g++
CPPFLAGS = -O2 -Wall -c
LDFLAGS = -O2 -o

# Treat NT and non-NT windows the same
ifeq ($(OS),Windows_NT)
	OS = Windows
endif

ifeq ($(OS),Windows)
	EXE = .exe
	DEL = del
else	#assume Linux/Unix
	EXE =
	DEL = rm
endif

all:		sample$(EXE)

sample$(EXE):	sample.o bitfile.o
		$(LD) $^ $(LDFLAGS) $@

sample.o:	sample.cpp bitfile.h
		$(CPP) $(CPPFLAGS) $<

bitfile.o:	bitfile.cpp bitfile.h
		$(CPP) $(CPPFLAGS) $<

clean:
		$(DEL) *.o
		$(DEL) sample$(EXE)
		$(DEL) testfile
