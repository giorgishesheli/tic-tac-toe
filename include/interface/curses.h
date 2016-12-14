#ifndef INTERFACE_CURSES_H
#define INTERFACE_CURSES_H

#include "../ttt.h"

struct interface {
	WINDOW *win_board;
	WINDOW *win_status;
	int board_x;
	int board_y;
};


void init_curses();
void draw_board(struct game_state *game);
void curses_x_won(struct game_state *game);
void curses_o_won(struct game_state *game);
void curses_draw(struct game_state *game);
void main_loop(struct game_state *game);

#endif
