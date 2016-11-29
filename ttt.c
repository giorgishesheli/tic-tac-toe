#include "include/ttt.h"
#include "include/engine.h"

#include <string.h>


int board_size = BOARD_SIZE;



void quit(int status){
	endwin();
	exit(status);
}

struct game_state *init_game(){
#ifdef DEBUG
	dbg_log = fopen("./log", "a");
	setbuf(dbg_log, NULL);
#endif

	struct game_state *game = malloc(sizeof(struct game_state));
	memset(game, 0, sizeof(struct game_state));
	game->board[0] = malloc(sizeof(BOARD_SIZE * sizeof(int)));
	game->board[1] = malloc(sizeof(BOARD_SIZE * sizeof(int)));
	game->vs = malloc(sizeof(struct v_options));	
	game->game_status = 1;

	game->vs->board_size = 3;
	game->vs->w_board = newwin(LINES - 1, COLS, 0, 0);
	game->vs->w_status = newwin(1, COLS, LINES - 1, 0);

	game->vs->board_x = (COLS / 2) - (board_size * 4) / 2; /* Middle of the screen*/
	game->vs->board_y = (int) LINES * 0.2; /* twenty percent padding top*/
	draw_board(game); 
	wmove(game->vs->w_board, game->vs->board_y, ++(game->vs->board_x));
	wrefresh(game->vs->w_board);
	return game;
}

void init_curses(){
	setlocale(LC_ALL, "");
	initscr();
	cbreak();
	noecho();
}

int main(int argc, char **argv){
	struct game_state *game;

	init_curses();
	game = init_game();
	main_loop(game);
	exit(EXIT_SUCCESS);
}
