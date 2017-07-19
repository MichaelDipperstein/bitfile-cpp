############################################################################
# Makefile for bitfile class library and sample
#
############################################################################

CPP = g++
LD = g++
CPPFLAGS = -O2 -Wall -Wextra -pedantic -c
LDFLAGS = -O2 -o

# libraries
LIBS = -L. -lbitfile

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

sample$(EXE):	sample.o libbitfile.a
		$(LD) $< $(LIBS) $(LDFLAGS) $@

sample.o:	sample.cpp bitfile.h
		$(CPP) $(CPPFLAGS) $<

libbitfile.a:	bitfile.o
		ar crv libbitfile.a bitfile.o
		ranlib libbitfile.a

bitfile.o:	bitfile.cpp bitfile.h
		$(CPP) $(CPPFLAGS) $<

clean:
		$(DEL) *.o
		$(DEL) *.a
		$(DEL) sample$(EXE)
		$(DEL) testfile
