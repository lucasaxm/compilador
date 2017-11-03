$DEPURA=1

compilador: lex.yy.c y.tab.c compilador.o compilador.h pilha.o pilha.h tabela.o tabela.h
	gcc lex.yy.c compilador.tab.c compilador.o pilha.o tabela.o -o compilador -ll -ly -lc

lex.yy.c: compilador.l compilador.h
	flex compilador.l

y.tab.c: compilador.y compilador.h
	bison compilador.y -d -v --report=all

compilador.o : compilador.h compiladorF.c
	gcc -c compiladorF.c -o compilador.o

pilha.o : pilha.h pilha.c
	gcc -c pilha.c -o pilha.o

tabela.o : tabela.h tabela.c
	gcc -c tabela.c -o tabela.o

clean : 
	rm -f compilador.tab.* lex.yy.c *.o
