#include "include/ttt.h"

void free_game_struct(struct game_state *game){
	free(game->board[0]);
	free(game->board[1]);
	free(game);
}

void draw_board(struct game_state *game){
	int i,j;
	for(i = 0; i < game->vs->board_size - 1; i++){
		for(j = 0; j < game->vs->board_size * 4 - 1; j++){ /* draw horizontal lines */
			if((j + 1) % 4 == 0){
				mvwaddch(game->vs->w_board, game->vs->board_y + 1 + 2 * i , game->vs->board_x + j, ACS_PLUS);
			} else {
				mvwaddch(game->vs->w_board, game->vs->board_y + 1 + 2 * i , game->vs->board_x + j, ACS_HLINE);
			}
		}

		for(j = 0; j < game->vs->board_size; j++){ /* draw vertical lines */
			mvwaddch(game->vs->w_board, game->vs->board_y + 2 * j , game->vs->board_x + 3 + 4 * i, ACS_VLINE);
		} 
			
	}
}

static void handle_finish(struct game_state *game, int who){
	switch(who){
		case X_WON:
			dbg("X Won\n");
			mvwprintw(game->vs->w_status, 0, 0, "X Won\n");
			break;
		case Y_WON:
			dbg("O Won\n");
			mvwprintw(game->vs->w_status, 0, 0, "O Won\n");
			break;
		case DRAW:
			dbg("DRAW \n");
			mvwprintw(game->vs->w_status, 0, 0, "Draw ... \n");
		default:
			dbg("handle_finish() got incorrect parameter\n");
	}

	game->game_status = 0;	
	wrefresh(game->vs->w_status);
}


static void win_or_draw(struct game_state *game, int w_turn){ // w_turn: 1 = X's turn; -1 = Y's turn
	dbg("board[0][game->cursor_x] = %d; board[1][game->cursor_y] = %d; diagonal[0] = %d; diagonal[1] = %d\n--------\n",
			game->board[0][game->cursor_x], game->board[1][game->cursor_y], game->diagonal[0], game->diagonal[1]);

	if(w_turn == 1){
		if(game->board[0][game->cursor_x] == game->vs->board_size || game->board[1][game->cursor_y] == game->vs->board_size || 
			game->diagonal[0] == game->vs->board_size || game->diagonal[1] == game->vs->board_size){
				handle_finish(game, X_WON);
		}

	} else {

		if(game->board[0][game->cursor_x] == game->vs->board_size * -1 || game->board[1][game->cursor_y] == game->vs->board_size * -1 ||
			game->diagonal[0] == game->vs->board_size * -1 || game->diagonal[1] == game->vs->board_size * -1){
				handle_finish(game, Y_WON);
		}
	}

	if(game->turn_num == game->vs->board_size * game->vs->board_size && game->game_status){
		handle_finish(game, DRAW);
	}

}


void main_loop(struct game_state *game){
	char key;
	signed int l_diag = 0, r_diag = 0, add = 0; 
	/* Lesson Learned
	   signed int l_diag, r_diag, add = 0; <-- only initializes add variable as zero
	*/
	for(;;){
	key = wgetch(game->vs->w_board);
		switch(key){
			case 'j':
				if(game->cursor_y < (game->vs->board_size - 1) && game->game_status)
					game->cursor_y += 1;
				break;
			case 'k':
				if(game->cursor_y > 0 && game->game_status)
					game->cursor_y -= 1;
				break;
			case 'h':
				if(game->cursor_x > 0 && game->game_status)
					game->cursor_x -= 1;
				break;
			case 'l':
				if(game->cursor_x < (game->vs->board_size -1) && game->game_status)
					game->cursor_x += 1;
				break;
			case ' ':
				if(inch() == ' ' && game->game_status){ // game logic merges with presentiation. not good.
					if(game->cursor_x - game->cursor_y == 0)
						l_diag = 1;

					if(game->cursor_x + game->cursor_y == game->vs->board_size - 1)
						r_diag = 1;

					if(game->turn_num % 2 == 0){
						waddch(game->vs->w_board, 'X');
						add = 1;
					} else {
						waddch(game->vs->w_board, 'O');
						add = -1;
					}

					game->board[0][game->cursor_x] += add;				
					game->board[1][game->cursor_y] += add;
					if(l_diag)
						game->diagonal[0] += add;
					if(r_diag)
						game->diagonal[1] += add;
					dbg("l_diag = %d; r_diag = %d\n", l_diag, r_diag);
					game->turn_num++;
					win_or_draw(game, add);
				}
				break;
			case 'q':
				mvwprintw(game->vs->w_status, 0, 0, "Do you really want to quit [Y/N] ?\n");
				char ch = wgetch(game->vs->w_status);
				switch(ch){
					case 'Y':
					case 'y':
						free_game_struct(game);
						quit(EXIT_SUCCESS);
					default:
						wmove(game->vs->w_status, 0, 0);
						wclrtoeol(game->vs->w_status);
						wrefresh(game->vs->w_status);
				}


		}

		wmove(game->vs->w_board, game->vs->board_y + game->cursor_y * 2, game->vs->board_x + game->cursor_x * 4 );
		wrefresh(game->vs->w_board);
		/* reset variables */
		l_diag = 0;
		r_diag = 0;
	}
}
