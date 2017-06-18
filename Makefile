main:	floodit_trabalho_2017.o
	gcc floodit_trabalho_2017.o -o main

floodit_trabalho_2017.o:	floodit_trabalho_2017.c
	gcc -c floodit_trabalho_2017.c

clean:
	rm *.o