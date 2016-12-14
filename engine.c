#include "include/ttt.h"
#include "include/engine.h"


static void handle_finish(struct game_state *game, int who)
{
	switch(who){
		case _X_:
			dbg("X Won\n");
			curses_x_won(game);
			break;
		case _O_:
			dbg("O Won\n");
			curses_o_won(game);
			break;
		case _DRAW_:
			dbg("DRAW \n");
			curses_draw(game);
			break;
		default:
			dbg("handle_finish() got incorrect parameter\n");
	}

}

static void win_or_draw(struct game_state *game, signed int player)
{
	dbg("x = %d; y = %d; left = %d; right = %d\n----\n",
			SUM_X, SUM_Y, SUM_LEFT, SUM_RIGHT);

	if(player == _X_){
		if(SUM_X == game->size || 
			SUM_Y == game->size || 
			SUM_LEFT == game->size || 
			SUM_RIGHT == game->size){

				handle_finish(game, _X_);
		}

	} else {

		if(SUM_X == game->size * -1 ||
			SUM_Y == game->size * -1 ||
			SUM_LEFT == game->size * -1 ||
			SUM_RIGHT == game->size * -1){

				handle_finish(game, _O_);
		}
	}

	if(game->turn_num == game->size * game->size && game->status){
		handle_finish(game, _DRAW_);
	}
}

signed int process_input(struct game_state *game){
	signed int l_diag = 0, r_diag = 0, add = 0, player = _PASS_; 
	if(!game->logical_board[game->x][game->y] && game->status && game->turn){
		if(game->x - game->y == 0)
			l_diag = 1;

		if(game->x + game->y == game->size - 1)
			r_diag = 1;

		if(game->turn_num % 2 == 0){
			player = _X_;
			add = 1;
		} else {
			player = _O_;
			add = -1;
		}

		SUM_X += add;				
		SUM_Y += add;

		if(l_diag)
			SUM_LEFT += add;
		if(r_diag)
			SUM_RIGHT += add;

		game->logical_board[game->x][game->y] = 1;
		game->turn_num++;
		win_or_draw(game, player);
	}

	return player;
}
