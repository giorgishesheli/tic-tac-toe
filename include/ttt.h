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

struct game_state {
	int status; /* 1 = game running, 0 = game over */
	int size; /* board size */
	int turn_num;
	int result;
	/*
	   logical board is a two dimensional array with size + 1 columns and
	   size + 2 rows. from 0 to size it serves as a logical matrix. 
	   additional column holds sum of corresponding rows. first additional
	   row holds sum of corresponding columns. second additional row holds
	   only two values, sum of left diagonal and sum of right diagonal.
	   there is no
	*/
	int **logical_board;
	int x, y; /* logical coordiantes */
	struct interface *visual;
	/* For online pvp */
	int turn;
};

#define INTERFACE_CURSES

#ifdef INTERFACE_CURSES

#define INIT_INTERFACE(game) curses_init(game)
#define INTERFACE_MENU(game) curses_menu(game)
#define PLAY_LOCAL(game) curses_play_local(game)
#include "interface/curses.h"

#endif

#define INIT_GAME_STRUCT(game, a) \
	game->status = 1; \
	game->logical_board = malloc((a + 2) * sizeof(void *)); \
	for(i = 0; i <= a; i++){ \
		game->logical_board[i] = malloc((a + 1) * sizeof(int)); \
		memset(game->logical_board[i], 0, (a + 1) * sizeof(int)); \
	} \
	game->logical_board[a + 1] = malloc(2 * sizeof(int)); \
	memset(game->logical_board[a + 1], 0, 2 * sizeof(int)); \

#define LOCAL_GAME 1
#define INET_PVP 2
#define INET_SERV 3
#define QUIT_GAME 4

void quit(int status);
void free_game_struct(struct game_state *game);
void null_game_struct(struct game_state *game);

#endif
