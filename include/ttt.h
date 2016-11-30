#ifndef ttt_H
#define ttt_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>
#include <locale.h>

#include <ncurses.h>

#ifdef DEBUG
#define dbg(...)  fprintf(dbg_log, __VA_ARGS__)
extern FILE *dbg_log;
#else
#define dbg(...)
#endif


#define INIT_GAME_STRUCT(game, a) \
	game = malloc(sizeof(struct game_state)); \
	memset(game, 0, sizeof(struct game_state)); \
	game->status = 1; \
	game->sum[0] = malloc(a * sizeof(int)); \
	game->sum[1] = malloc(a * sizeof(int)); \
	game->sum[2] = malloc(sizeof(int)); \
	game->sum[3] = malloc(sizeof(int)); \
	memset((void *)game->sum[0], 0, a * sizeof(sizeof(int))); \
	memset((void *)game->sum[1], 0, a * sizeof(sizeof(int))); \
	*game->sum[2] = 0; \
	*game->sum[3] = 0; \
	game->size = a; \
	game->win_board = newwin(LINES - 1, COLS, 0, 0); \
	game->win_status = newwin(1, COLS, LINES - 1, 0); \
	game->win_x = (COLS / 2) - (a * 4) / 2; \
	game->win_y = (int) LINES * 0.2; \





struct game_state {
	int status; /* 1 = game running, 0 = game over */
	int turn;
	int size; /* board size */
	int x, y; /* logical coordiantes */
	signed int *sum[4]; /* sum of columns, rows, left diag, right diag*/

	/* visual options */
	WINDOW *win_board; /* curses board window */
	WINDOW *win_status; /* curses status bar window */
	int win_x, win_y; /* initial board x and y location */
};

void quit(int status);

#endif
