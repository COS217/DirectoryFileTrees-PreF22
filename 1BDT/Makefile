#--------------------------------------------------------------------
# Makefile for Assignment 4, Part 1
# Author: Christopher Moretti
#--------------------------------------------------------------------

TARGETS=bdtGood bdtBad1 bdtBad2 bdtBad3 bdtBad4 bdtBad5

all: $(TARGETS)

clean:
	rm -f $(TARGETS) *~

clobber: clean
	rm -f  dynarray.o bdt_client.o

bdt_client.o: bdt_client.c bdt.h
	gcc217 -g -c $<

dynarray.o: dynarray.c dynarray.h
	gcc217 -g -c $<

bdt%: dynarray.o bdt%.o bdt_client.o
	gcc217 -g $^ -o $@



