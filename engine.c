#include "include/ttt.h"
#include "include/engine.h"


static void handle_finish(struct game_state *game, int who)
{
	switch(who){
		case X_WON:
			dbg("X Won\n");
			wbkgd(game->win_status, COLOR_PAIR(1));
			mvwprintw(game->win_status, 0, 0, "X Won\n");
			break;
		case O_WON:
			dbg("O Won\n");
			mvwprintw(game->win_status, 0, 0, "O Won\n");
			break;
		case DRAW:
			dbg("DRAW \n");
			mvwprintw(game->win_status, 0, 0, "Draw ... \n");
		default:
			dbg("handle_finish() got incorrect parameter\n");
	}

	game->status = 0;	
	wrefresh(game->win_status);
	wbkgd(game->win_status, COLOR_PAIR(0));
}

/* w_turn: 1 = X's turn; -1 = Y's turn */
static void win_or_draw(struct game_state *game, int w_turn)
{
	dbg("x = %d; y = %d; left = %d; right = %d\n----\n",
			SUM_X, SUM_Y, SUM_LEFT, SUM_RIGHT);

	if(w_turn == 1){
		if(SUM_X == game->size || 
			SUM_Y == game->size || 
			SUM_LEFT == game->size || 
			SUM_RIGHT == game->size){

				handle_finish(game, X_WON);
		}

	} else {

		if(SUM_X == game->size * -1 ||
			SUM_Y == game->size * -1 ||
			SUM_LEFT == game->size * -1 ||
			SUM_RIGHT == game->size * -1){

				handle_finish(game, O_WON);
		}
	}

	if(game->turn == game->size * game->size && game->status){
		handle_finish(game, DRAW);
	}

}


void main_loop(struct game_state *game)
{
	char key;
	signed int l_diag = 0, r_diag = 0, add = 0; 
	for(;;){
	key = wgetch(game->win_board);
		switch(key){

		case 's':
		case 'j':
			if(game->y < (game->size - 1) && game->status)
				game->y += 1;
			break;
		case 'w':
		case 'k':
			if(game->y > 0 && game->status)
				game->y -= 1;
			break;
		case 'a':
		case 'h':
			if(game->x > 0 && game->status)
				game->x -= 1;
			break;
		case 'd':
		case 'l':
			if(game->x < (game->size -1) && game->status)
				game->x += 1;
			break;
		case '\n':
		case ' ':
			/* game logic merges with presentiation. not good. */
			if(winch(game->win_board) == ' ' && game->status){
				if(game->x - game->y == 0)
					l_diag = 1;

				if(game->x + game->y == game->size - 1)
					r_diag = 1;

				if(game->turn % 2 == 0){
					waddch(game->win_board, 'X');
					add = 1;
				} else {
					waddch(game->win_board, 'O');
					add = -1;
				}

				SUM_X += add;				
				SUM_Y += add;

				if(l_diag)
					SUM_LEFT += add;
				if(r_diag)
					SUM_RIGHT += add;

				game->turn++;
				win_or_draw(game, add);
			}
			break;
		case 'q':
			wbkgd(game->win_status, COLOR_PAIR(2));
			mvwprintw(game->win_status, 0, 0, 
					"Do you really want to quit [Y/N] ?\n");
			char ch = wgetch(game->win_status);
			switch(ch){
				case 'Y':
				case 'y':
					free_game_struct(game);
					quit(EXIT_SUCCESS);
				default:
					wmove(game->win_status, 0, 0);
					wclrtoeol(game->win_status);
					wbkgd(game->win_status, COLOR_PAIR(0));
					wrefresh(game->win_status);
					
			}


		}

		wmove(game->win_board, game->win_y + game->y * 2,
				game->win_x + game->x * 4 );

		wrefresh(game->win_board);
		/* reset variables */
		l_diag = 0;
		r_diag = 0;
	}
}


void draw_board(struct game_state *game)
{
	int i,j;
	for(i = 0; i < game->size - 1; i++){
		/* draw horizontal lines */
		for(j = 0; j < game->size * 4 - 1; j++){ 
			if((j + 1) % 4 == 0){
				mvwaddch(game->win_board,
						game->win_y + 1 + 2 * i ,
						game->win_x + j, ACS_PLUS);
			} else {
				mvwaddch(game->win_board,
						game->win_y + 1 + 2 * i,
						game->win_x + j,ACS_HLINE);
			}
		}

		/* draw vertical lines */
		for(j = 0; j < game->size; j++){ 
			mvwaddch(game->win_board,
					game->win_y + 2 * j ,
					game->win_x + 3 + 4 * i, ACS_VLINE);
		} 
			
	}
}
