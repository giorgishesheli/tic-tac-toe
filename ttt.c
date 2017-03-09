#include "include/ttt.h"
#include "include/engine.h"

#include <string.h>

#ifdef DEBUG
FILE *dbg_log;
#endif

void null_game_struct(struct game_state *game){
	int i;
	game->turn_num = 0;
	game->result = 0;
	game->x = 0;
	game->y = 0;
	game->turn = 0;

	for(i = 0; i <= game->size; i++){
		memset(game->logical_board[i], 0,
				(game->size + 1) * sizeof(void *));
	}
	memset(game->logical_board[game->size + 1], 0, 2 * sizeof(int));
}

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



int main(int argc, char **argv)
{
	struct game_state *game;
	game = malloc(sizeof(struct game_state));
	memset(game, 0, sizeof(struct game_state));

#ifdef DEBUG
	dbg_log = fopen("./log", "a");
	setbuf(dbg_log, NULL);
#endif

	INIT_INTERFACE(game);
	while(1){
		switch(INTERFACE_MENU(game->visual)){
			case LOCAL_GAME:
				PLAY_LOCAL(game);
				break;
			case INET_PVP:
				break;
			case INET_SERV:
				break;
			case QUIT_GAME:
				free(game); // TODO Memory leak
				quit(EXIT_SUCCESS);
		}
	}
	exit(EXIT_SUCCESS);
}
