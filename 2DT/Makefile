#--------------------------------------------------------------------
# Makefile for Assignment 4, Part 2
# dt* targets are built using checker
# rules to build dt*.o and node*.o from source will fail
# Author: Christopher Moretti
#--------------------------------------------------------------------

TARGETS = dtGood dtBad1a dtBad1b  dtBad2 dtBad3 dtBad4 dtBad5 

.PRECIOUS: %.o

all: $(TARGETS)

clean:
	rm -f $(TARGETS) *~

clobber: clean
	rm -f nodeGood.o dtGood.o dynarray.o checker.o

dt%: dynarray.o node%.o checker.o dt%.o dt_client.c
	gcc217 -g $^ -o $@

checker.o: checker.c dynarray.h checker.h node.h a4def.h
	gcc217 -g -c $<

dynarray.o: dynarray.c dynarray.h
	gcc217 -g -c $<

dtGood.o: dtGood.c dynarray.h dt.h a4def.h node.h checker.h
	gcc217 -g -c $<

nodeGood.o: nodeGood.c dynarray.h node.h a4def.h
	gcc217 -g -c $<

dt%.o: dt%.c dynarray.h dt.h a4def.h node.h checker.h
	$(error "You can't re-build" $<)

node%.o: node%.c dynarray.h node.h a4def.h
	$(error "You can't re-build" $<)
