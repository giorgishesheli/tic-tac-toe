all: ttt

ttt: ttt.o engine.o
	gcc -std=c11 -Wall -Wpedantic -g ttt.o engine.o -lncurses -o ttt

debug:
	gcc -std=c11 -Wall -Wpedantic -g -DDEBUG ttt.c engine.c -lncurses -o ttt

clean:
	rm ttt.o engine.o ttt
