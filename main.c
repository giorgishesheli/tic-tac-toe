/*
*	Add exit method
*	Add scoreboard
*	Add Start Menu
*	Add Quitting
*/
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <locale.h>

#include <ncurses.h>

#ifndef BOARD_SIZE
#define BOARD_SIZE 3
#endif

#ifdef DEBUG
#define dbg(...)  fprintf(stderr, __VA_ARGS__)
#else
#define dbg(...)
#endif

#define X_WON 0
#define Y_WON 1
#define DRAW 2


WINDOW *w_board;
WINDOW *w_status;

int cursor_x, cursor_y = 0; // logical cursor location
int board_x, board_y;  // Initial board location

int game_state = 1; /* 1 game is on, 0 game is over */
int turn_num = 0; 

signed int board[2][BOARD_SIZE] = {0}; /* TODO: research if zeroing out this way is standard or not */
signed int diagonal[2] = {0, 0};





void draw_board(int init_x, int init_y){
	int i,j;
	for(i = 0; i < BOARD_SIZE - 1; i++){
		for(j = 0; j < BOARD_SIZE * 4 - 1; j++){ /* draw horizontal lines */
			if((j + 1) % 4 == 0){
				mvwaddch(w_board, init_y + 1 + 2 * i , init_x + j, ACS_PLUS);
			} else {
				mvwaddch(w_board, init_y + 1 + 2 * i , init_x + j, ACS_HLINE);
			}
		}

		for(j = 0; j < BOARD_SIZE; j++){ /* draw vertical lines */
			mvwaddch(w_board, init_y + 2 * j , init_x + 3 + 4 * i, ACS_VLINE);
		} 
			
	}
}

void handle_finish(int who){
	switch(who){
		case X_WON:
			dbg("X Won\n");
			mvwprintw(w_status, 0, 0, "X Won\n");
			break;
		case Y_WON:
			dbg("O Won\n");
			mvwprintw(w_status, 0, 0, "O Won\n");
			break;
		case DRAW:
			dbg("DRAW \n");
			mvwprintw(w_status, 0, 0, "Draw ... \n");
		default:
			dbg("handle_finish() got incorrect parameter\n");
	}

	game_state = 0;	
	wrefresh(w_status);
}


void win_or_draw(int w_turn){ // w_turn: 1 = X's turn; -1 = Y's turn
	dbg("board[0][cursor_x] = %d; board[1][cursor_y] = %d; diagonal[0] = %d; diagonal[1] = %d\n--------\n",
			board[0][cursor_x], board[1][cursor_y], diagonal[0], diagonal[1]);

	if(w_turn == 1){
		if(board[0][cursor_x] == BOARD_SIZE || board[1][cursor_y] == BOARD_SIZE || 
			diagonal[0] == BOARD_SIZE || diagonal[1] == BOARD_SIZE){
				handle_finish(X_WON);
		}

	} else {

		if(board[0][cursor_x] == BOARD_SIZE * -1 || board[1][cursor_y] == BOARD_SIZE * -1 ||
			diagonal[0] == BOARD_SIZE * -1 || diagonal[1] == BOARD_SIZE * -1){
				handle_finish(Y_WON);
		}
	}

	if(turn_num == BOARD_SIZE * BOARD_SIZE && game_state){
		handle_finish(DRAW);
	}

}


void main_loop(){
	char key;
	signed int l_diag = 0, r_diag = 0, add = 0; 
	/* Lesson Learned
	   signed int l_diag, r_diag, add = 0; <-- only initializes add variable as zero
	*/
	for(;;){
	key = wgetch(w_board);
		switch(key){
			case 'j':
				if(cursor_y < (BOARD_SIZE - 1) && game_state)
					cursor_y += 1;
				break;
			case 'k':
				if(cursor_y > 0 && game_state)
					cursor_y -= 1;
				break;
			case 'h':
				if(cursor_x > 0 && game_state)
					cursor_x -= 1;
				break;
			case 'l':
				if(cursor_x < (BOARD_SIZE -1) && game_state)
					cursor_x += 1;
				break;
			case ' ':
				if(inch() == ' ' && game_state){ // game logic merges with presentiation. not good.
					if(cursor_x - cursor_y == 0)
						l_diag = 1;

					if(cursor_x + cursor_y == BOARD_SIZE - 1)
						r_diag = 1;

					if(turn_num % 2 == 0){
						waddch(w_board, 'X');
						add = 1;
					} else {
						waddch(w_board, 'O');
						add = -1;
					}

					board[0][cursor_x] += add;				
					board[1][cursor_y] += add;
					if(l_diag)
						diagonal[0] += add;
					if(r_diag)
						diagonal[1] += add;
					dbg("l_diag = %d; r_diag = %d\n", l_diag, r_diag);
					turn_num++;
					win_or_draw(add);
				}
			case 'r':
				if(!game_state){
				}

				

		}

		wmove(w_board, board_y + cursor_y * 2, board_x + cursor_x * 4 );
		wrefresh(w_board);
		/* reset variables */
		l_diag = 0;
		r_diag = 0;
	}
}


void init_game(){
	board_x = (COLS / 2) - (BOARD_SIZE * 4) / 2; /* Middle of the screen*/
	board_y = (int) LINES * 0.2; /* twenty percent padding top*/
	draw_board(board_x, board_y); 
	wmove(w_board, board_y, ++board_x);
	wrefresh(w_board);
}

void init_curses(){
	setlocale(LC_ALL, "");
	initscr();
	cbreak();
	noecho();
	w_board = newwin(LINES - 1, COLS, 0, 0);
	w_status = newwin(1, COLS, LINES - 1, 0);
}

int main(int argc, char **argv){
	init_curses();
	init_game();
	main_loop();
	exit(EXIT_SUCCESS);
}
