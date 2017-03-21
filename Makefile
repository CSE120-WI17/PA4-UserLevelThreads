# Makefile to compile Umix Programming Assignment 4 (pa4) [updated: 1/11/10]

LIBDIR = $(UMIXPUBDIR)/lib
# LIBDIR = $(UMIXROOTDIR)/sys

CC 	= cc 
FLAGS 	= -g -L$(LIBDIR) -lumix4

<<<<<<< HEAD
PA4 =	pa4a pa4b pa4c tc1 fifo piazza2 centihavoc
=======
PA4 =	pa4a pa4b pa4c
>>>>>>> 97b8df8770bce2c037f4c57d5b9683f90fc9537c

pa4:	$(PA4)

pa4a:	pa4a.c aux.h umix.h
	$(CC) $(FLAGS) -o pa4a pa4a.c

pa4b:	pa4b.c aux.h umix.h mythreads.h mythreads.o
	$(CC) $(FLAGS) -o pa4b pa4b.c mythreads.o

pa4c:	pa4c.c aux.h umix.h mythreads.h mythreads.o
	$(CC) $(FLAGS) -o pa4c pa4c.c mythreads.o

<<<<<<< HEAD
tc1:	tc1.c aux.h umix.h mythreads.h mythreads.o
	$(CC) $(FLAGS) -o tc1 tc1.c mythreads.o

fifo: 	advanced_fifo.c aux.h umix.h mythreads.h mythreads.o
	$(CC) $(FLAGS) -o fifo advanced_fifo.c mythreads.o

piazza2:piazza2.c aux.h umix.h mythreads.h mythreads.o
	$(CC) $(FLAGS) -o piazza2 piazza2.c mythreads.o

centihavoc: centihavoc.c aux.h umix.h mythreads.h mythreads.o
	$(CC) $(FLAGS) -o centihavoc centihavoc.c mythreads.o

=======
>>>>>>> 97b8df8770bce2c037f4c57d5b9683f90fc9537c
mythreads.o:	mythreads.c aux.h umix.h mythreads.h
	$(CC) $(FLAGS) -c mythreads.c

clean:
	rm -f *.o $(PA4)
