#include "include/ttt.h"
#include "include/engine.h"

#include <string.h>

#ifdef DEBUG
FILE *dbg_log;
#endif

void free_game_struct(struct game_state *game)
{
	int i;
	for(i = 0; i <= game->size; i++){
		free(game->logical_board[i]);
	}
	free(game->logical_board[game->size + 1]);
	free(game->logical_board);
	free(game->visual);
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
	int i; /* needed for INIT_GAME_STRUCT macro */

	/* game struct pointer, size of board */
	INIT_GAME_STRUCT(game, 3);
	game->turn = 1;

	struct interface *visual = malloc(sizeof(struct interface));
	visual->win_board = newwin(LINES - 1, COLS, 0, 0); \
	visual->win_status = newwin(1, COLS + 1, LINES - 1, 0); \
	visual->board_x = (COLS / 2) - (3 * 4) / 2; \
	visual->board_y = (int) LINES * 0.2; \
	game->visual = visual;


	draw_board(game); 
	wmove(visual->win_board, visual->board_y, ++(visual->board_x));
	wrefresh(visual->win_board);

	return game;
}


int main(int argc, char **argv)
{
	struct game_state *game;

#ifdef DEBUG
	dbg_log = fopen("./log", "a");
	setbuf(dbg_log, NULL);
#endif

	INIT_INTERFACE();
	game = init_game();
	main_loop(game);
	exit(EXIT_SUCCESS);
}
