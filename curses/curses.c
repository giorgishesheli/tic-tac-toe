#include "../include/interface/curses.h"
#include "../include/engine.h"
#include <string.h>
#include <errno.h>

void erase_status(WINDOW *status_bar)
{
	wmove(status_bar, 0, 0);
	wclrtoeol(status_bar);
	wbkgd(status_bar, COLOR_PAIR(0));
	wrefresh(status_bar);
}

void write_status(WINDOW *status_bar, char *message, int color)
{
	wbkgd(status_bar, COLOR_PAIR(color));
	mvwprintw(status_bar, 0, 0, message);
	wrefresh(status_bar);
}

void curses_init(struct game_state *game)
{
	setlocale(LC_ALL, "");
	initscr();
	start_color();
	use_default_colors();
	init_pair(1, COLOR_BLACK, COLOR_YELLOW);
	init_pair(2, COLOR_BLACK, COLOR_RED);
	cbreak();
	noecho();

	struct interface *visual = malloc(sizeof(struct interface));
	visual->win_board = newwin(LINES - 1, COLS, 0, 0); \
	visual->win_status = newwin(1, COLS + 1, LINES - 1, 0); \
	visual->board_y = (int) LINES * 0.2; \
	game->visual = visual;
}

static void init_game(struct game_state *game)
{
	int i; /* needed for INIT_GAME_STRUCT macro */

	INIT_GAME_STRUCT(game, game->size);
	game->turn = 1;



	game->visual->board_x = (COLS / 2) - (game->size * 4) / 2; \
	draw_board(game); 
	wmove(game->visual->win_board, game->visual->board_y, ++(game->visual->board_x));
	wrefresh(game->visual->win_board);
}

void draw_board(struct game_state *game)
{
	wclear(game->visual->win_board);
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

static void handle_finish(struct game_state *game)
{
	switch(game->result){
		case _X_:
			dbg("X Won\n");
			write_status(game->visual->win_status, "X Won\n", 1);
			break;
		case _O_:
			dbg("O Won\n");
			write_status(game->visual->win_status, "O Won\n", 1);
			break;
		case _DRAW_:
			dbg("DRAW \n");
			write_status(game->visual->win_status, "Draw..\n", 1);
			break;
		default:
			dbg("handle_finish() got incorrect parameter\n");
	}

}

static int game_loop(struct game_state *game)
{
	int key;
	while(game->status){
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
					waddch(game->visual->win_board, 'X');
					break;
				case _O_:
					waddch(game->visual->win_board, 'O');
					break;
				case _PASS_:
					//pass
					break;
			}
			if(game->status == 0){
				handle_finish(game);
			}
			break;
		case 'q':
			write_status(game->visual->win_status,
				"Do you really want to quit [Y/N]?\n", 2);
			char ch = wgetch(game->visual->win_status);
			switch(ch){
				case 'Y':
				case 'y':
				free_game_struct(game);
				quit(EXIT_SUCCESS);

				default:
				erase_status(game->visual->win_status);
					
			}


		}

		wmove(game->visual->win_board, game->visual->board_y + game->y * 2,
				game->visual->board_x + game->x * 4 );

		wrefresh(game->visual->win_board);
	}
	return RESTART;
}

static void pre_game(struct game_state *game, char (*player)[15], int *score)
{
	int board_size;
	char ch_board_size[5];
	wclear(game->visual->win_board);
	wrefresh(game->visual->win_board);
	echo();
	mvwprintw(game->visual->win_board,
		      LINES / 2 - 1, 
	      	COLS / 2 - 10, "Board size (default 3): ");
	wgetnstr(game->visual->win_board, ch_board_size, 5);
	errno = 0;
	board_size = (int) strtol(ch_board_size, NULL, 0);
	if(errno){
		dbg("strtol errno fired %s \n", strerror(errno));
	} else {
		game->size = (board_size) ? board_size : 3;
	}

	mvwprintw(game->visual->win_board,
		      LINES / 2, 
	      	COLS / 2 - 15, "Player1 Name (default Player1): ");
	wgetnstr(game->visual->win_board, player[0], 15);
	

	mvwprintw(game->visual->win_board,
		      LINES / 2 + 1, 
	      	COLS / 2 - 15, "Player2 Name (default Player2): ");
	wgetnstr(game->visual->win_board, player[1], 15);
	noecho();

}

void curses_play_local(struct game_state *game)
{

	int action;
	char player[2][15] = { "Player1", "Player2" };
	int score[2];
	pre_game(game, player, score);
	init_game(game);
	for(;;){
		action = game_loop(game); // game loop returns value 
		null_game_struct(game);
	}
}

static void paint_logo(WINDOW *window, int x, int y)
{
	mvwprintw(window, y++, x - 15, " XX   XX  |    XX    |  XX   XX"); 	
	mvwprintw(window, y++, x - 15, "  XX XX   |  XX  XX  |   XX XX "); 	
	mvwprintw(window, y++, x - 15, "   XXX    | X      X |    XXX  "); 	
	mvwprintw(window, y++, x - 15, "  XX XX   |  XX  XX  |   XX XX "); 	
	mvwprintw(window, y++, x - 15, " XX   XX  |    XX    |  XX   XX"); 	
	mvwprintw(window, y + 2, x - 5, "TIC TAC TOE"); 	
}
static void paint_menu(WINDOW *window, char **menu,  int active)
{
	int x_bgin = COLS / 2 - 7;
	int y_bgin = LINES / 2 -1;
	int i = 0;
	wclear(window);
	paint_logo(window, x_bgin + 7, y_bgin - 10);
	while(*(menu + i) != NULL){
		if(active == i)
			wattron(window, A_STANDOUT);

		mvwprintw(window, y_bgin + i, x_bgin, *(menu + i));

		if(active == i)
			wattroff(window, A_STANDOUT);

		wmove(window, y_bgin + active, x_bgin);
		i++;
	}
	wrefresh(window);
}

int curses_menu(struct interface *visual)
{
	int i = 0, menu_status = 1, choice = 0;
	char *menu[15] = { 
		" PLAY LOCAL  ",
		" PLAY ONLINE ",
		" QUIT        ",
		NULL 
	};

	paint_menu(visual->win_board, menu,  i);
	while(menu_status){
		switch(wgetch(visual->win_board)){
			case 'j':
				i = (++i > 2) ? 2 : i;
				break;
			case 'k':
				i =(--i < 0) ? 0 : i;
				break;
			case '\n':
				switch(i){
					case 0:
					choice = LOCAL_GAME;
					menu_status = 0;
					break;

					case 1:
					choice = INET_PVP;
					break;
					
					case 2:
					choice = QUIT_GAME;
					menu_status = 0;
					break;
				}
				break;


		}
		paint_menu(visual->win_board, menu,  i);
	}
	return choice;
}
