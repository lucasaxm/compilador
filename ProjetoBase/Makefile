$DEPURA=1

compilador: lex.yy.c y.tab.c compilador.o compilador.h pilha.o pilha.h fila.o fila.h tabela.o tabela.h
	gcc lex.yy.c compilador.tab.c compilador.o pilha.o fila.o tabela.o -o compilador -ll -ly -lc -lm -g

lex.yy.c: compilador.l compilador.h
	flex compilador.l

y.tab.c: compilador.y compilador.h
	bison compilador.y -d -v --report=all

compilador.o : compilador.h compiladorF.c
	gcc -c compiladorF.c -o compilador.o

pilha.o : pilha.h pilha.c
	gcc -c pilha.c -o pilha.o -g

fila.o : fila.h fila.c
	gcc -c fila.c -o fila.o -g

tabela.o : tabela.h tabela.c
	gcc -c tabela.c -o tabela.o -g

clean : 
	rm -f compilador.tab.* lex.yy.c *.o
