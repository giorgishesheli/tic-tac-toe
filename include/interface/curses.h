#ifndef INTERFACE_CURSES_H
#define INTERFACE_CURSES_H

#include "../ttt.h"


#define ANOTHER_ROUND 0
#define RESTART 1
#define QUIT_MENU 2

struct interface {
	WINDOW *win_board;
	WINDOW *win_status;
	int board_x;
	int board_y;
};


void curses_init(struct game_state *game);
void draw_board(struct game_state *game);
void curses_main_loop(struct game_state *game);
int curses_menu(struct game_state *game);

#endif
