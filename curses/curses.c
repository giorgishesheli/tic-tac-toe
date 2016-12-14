#include "../include/interface/curses.h"
#include "../include/engine.h"

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

void draw_board(struct game_state *game)
{
	int i,j;
	for(i = 0; i < game->size - 1; i++){
		/* draw horizontal lines */
		for(j = 0; j < game->size * 4 - 1; j++){ 
			if((j + 1) % 4 == 0){
				mvwaddch(game->visual->win_board,
						game->visual->board_y + 1 + 2 * i ,
						game->visual->board_x + j, ACS_PLUS);
			} else {
				 mvwaddch(game->visual->win_board,
						game->visual->board_y + 1 + 2 * i,
						game->visual->board_x + j,ACS_HLINE);
			}
		}

		/* draw vertical lines */
		for(j = 0; j < game->size; j++){ 
			mvwaddch(game->visual->win_board,
					game->visual->board_y + 2 * j ,
					game->visual->board_x + 3 + 4 * i, ACS_VLINE);
		} 
			
	}
}

void curses_x_won(struct game_state *game)
{
		game->status = 0;	
		wbkgd(game->visual->win_status, COLOR_PAIR(1));
		mvwprintw(game->visual->win_status, 0, 0, "X Won\n");
		wrefresh(game->visual->win_status);
}

void curses_o_won(struct game_state *game)
{
		game->status = 0;	
		wbkgd(game->visual->win_status, COLOR_PAIR(1));
		mvwprintw(game->visual->win_status, 0, 0, "O Won\n");
		wrefresh(game->visual->win_status);
}

void curses_draw(struct game_state *game)
{
		game->status = 0;	
		wbkgd(game->visual->win_status, COLOR_PAIR(1));
		mvwprintw(game->visual->win_status, 0, 0, "Draw..\n");
		wrefresh(game->visual->win_status);
}

void main_loop(struct game_state *game)
{
	int key;
	for(;;){
	key = wgetch(game->visual->win_board);
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
			switch(process_input(game)){
				case _X_:
					waddch(game->visual->win_board, 'X'); // !!
					break;
				case _O_:
					waddch(game->visual->win_board, 'O'); // !!
					break;
			}
			break;
		case 'q':
			wbkgd(game->visual->win_status, COLOR_PAIR(2));
			mvwprintw(game->visual->win_status, 0, 0, 
					"Do you really want to quit [Y/N] ?\n");
			char ch = wgetch(game->visual->win_status);
			switch(ch){
				case 'Y':
				case 'y':
					free_game_struct(game);
					quit(EXIT_SUCCESS);
				default:
					wmove(game->visual->win_status, 0, 0);
					wclrtoeol(game->visual->win_status);
					wbkgd(game->visual->win_status, COLOR_PAIR(0));
					wrefresh(game->visual->win_status);
					
			}


		}

		wmove(game->visual->win_board, game->visual->board_y + game->y * 2,
				game->visual->board_x + game->x * 4 );

		wrefresh(game->visual->win_board);
	}
}
