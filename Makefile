DEBUG = -g -DDEBUG
	
all: ttt

ttt: ttt.o engine.o curses/curses.o
	gcc -std=c11 -Wall -Wpedantic $(DEBUG) $^ -lncurses -o $@

curses/curses.o: curses/curses.c include/interface/curses.h
	gcc -std=c11 -Wall -Wpedantic -c $(DEBUG) $< -o $@

%.o: %.c include/%.h
	gcc -std=c11 -Wall -Wpedantic -c $(DEBUG) $< -o $@

clean:
	rm ttt *.o curses/*.o
