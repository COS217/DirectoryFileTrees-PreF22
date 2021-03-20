#--------------------------------------------------------------------
# Makefile for Assignment 4, Part 2
# dt* targets are built using checkerDT
# rules to build dt{Bad,Good}*.o and node*.o from source will fail
# Author: Christopher Moretti
#--------------------------------------------------------------------

TARGETS = dtGood dtBad1a dtBad1b  dtBad2 dtBad3 dtBad4 dtBad5 

.PRECIOUS: %.o

all: $(TARGETS)

clean:
	rm -f $(TARGETS) *~

clobber: clean
	rm -f nodeGood.o dtGood.o dynarray.o checkerDT.o dt_client.o

dt%: dynarray.o node%.o checkerDT.o dt%.o dt_client.o
	gcc217 -g $^ -o $@

checkerDT.o: checkerDT.c dynarray.h checkerDT.h node.h a4def.h
	gcc217 -g -c $<

dynarray.o: dynarray.c dynarray.h
	gcc217 -g -c $<

dt_client.o: dt_client.c dt.h a4def.h
	gcc217 -g -c $<

dtGood.o: dtGood.c dynarray.h dt.h a4def.h node.h checkerDT.h
	gcc217 -g -c $<

nodeGood.o: nodeGood.c dynarray.h node.h a4def.h checkerDT.h
	gcc217 -g -c $<

dt%.o: dt%.c dynarray.h dt.h a4def.h node.h checkerDT.h
	$(error "You can't re-build" $<)

node%.o: node%.c dynarray.h node.h a4def.h checkerDT.h
	$(error "You can't re-build" $<)
