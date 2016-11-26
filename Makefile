all: main

main: main.c
	gcc -std=c11 -Wall -Wpedantic -g main.c -lncursesw -o main

debug: main.c
	gcc -std=c11 -Wall -Wpedantic -DDEBUG -g main.c -lncurses -o main
