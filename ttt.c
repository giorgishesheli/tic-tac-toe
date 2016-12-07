#include "include/ttt.h"
#include "include/engine.h"

#include <string.h>

#ifdef DEBUG
FILE *dbg_log;
#endif

void free_game_struct(struct game_state *game)
{
	free(game->sum[0]);
	free(game->sum[1]);
	free(game->sum[2]);
	free(game->sum[3]);
	free(game);
}

void quit(int status)
{
	endwin();
	exit(status);
}

struct game_state *init_game()
{
	struct game_state *game;

	/* game struct pointer, size of board */
	INIT_GAME_STRUCT(game, 3);

	draw_board(game); 
	wmove(game->win_board, game->win_y, ++(game->win_x));
	wrefresh(game->win_board);

	return game;
}

void init_curses()
{
	setlocale(LC_ALL, "");
	initscr();
	start_color();
	use_default_colors();
	init_pair(1, COLOR_BLACK, COLOR_YELLOW);
	init_pair(2, COLOR_BLACK, COLOR_RED);
	cbreak();
	noecho();
}

int main(int argc, char **argv)
{
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
