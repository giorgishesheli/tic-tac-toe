#ifndef ttt_H
#define ttt_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>
#include <locale.h>

#include <ncurses.h>

#ifndef BOARD_SIZE
#define BOARD_SIZE 3
#endif

#ifdef DEBUG
#define dbg(...)  fprintf(dbg_log, __VA_ARGS__)
extern FILE *dbg_log;
#else
#define dbg(...)
#endif

#define X_WON 0
#define Y_WON 1
#define DRAW 2




struct game_state {
	int game_status; /* 1 game is on, 0 game is over */
	int turn_num;
	int cursor_x, cursor_y;
	signed int *board[2]; /* sum of rows and columns */
	signed int diagonal[2]; /* sum of diagonals */
	struct v_options *vs; /* visual options */
};

struct v_options {
	WINDOW *w_board;
	WINDOW *w_status;
	int board_size;
	int board_x, board_y;
};

void quit(int status);

#endif
