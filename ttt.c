#include "include/ttt.h"
#include "include/engine.h"

#include <string.h>

#ifdef DEBUG
FILE *dbg_log;
#endif


void quit(int status){
	endwin();
	exit(status);
}

struct game_state *init_game(){
	struct game_state *game;

	/* game struct pointer, size of board */
	INIT_GAME_STRUCT(game, 3);

	draw_board(game); 
	wmove(game->win_board, game->win_y, ++(game->win_x));
	wrefresh(game->win_board);

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

#ifdef DEBUG
	dbg_log = fopen("./log", "a");
	setbuf(dbg_log, NULL);
#endif

	init_curses();
	game = init_game();
	main_loop(game);
	exit(EXIT_SUCCESS);
}
