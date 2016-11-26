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

#define X_PLAYER 0
#define Y_PLAYER 1

int cursor_x, cursor_y = 0; // logical cursor location
int board_x, board_y;  // Initial board location

signed int board[2][BOARD_SIZE];
signed int diagonal[2] = {0, 0};

int turn_num = 0; 



void draw_board(int init_x, int init_y){
	int i,j;
	for(i = 0; i < BOARD_SIZE - 1; i++){
		for(j = 0; j < BOARD_SIZE * 4 - 1; j++){
			if((j + 1) % 4 == 0){
				mvaddch(init_y + 1 + 2 * i , init_x + j, ACS_PLUS);
			} else {
				mvaddch(init_y + 1 + 2 * i , init_x + j, ACS_HLINE);
			}
		}

		for(j = 0; j < BOARD_SIZE; j++){
			mvaddch(init_y + 2 * j , init_x + 3 + 4 * i, ACS_VLINE);
		} 
			
	}
}


void win_or_draw(int w_turn){ // w_turn: -1 means Y's turn, 1 means X's turn
	dbg("diagonal[0] = %d, diagonal[1] = %d\n", diagonal[0], diagonal[1]);

	if(w_turn == 1){
		if(board[0][cursor_x] == BOARD_SIZE || board[1][cursor_y] == BOARD_SIZE || 
			diagonal[0] == BOARD_SIZE || diagonal[1] == BOARD_SIZE){

				dbg("X Won\n");
		}

	} else {

		if(board[0][cursor_x] == BOARD_SIZE * -1 || board[1][cursor_y] == BOARD_SIZE * -1 ||
			diagonal[0] == BOARD_SIZE * -1 || diagonal[1] == BOARD_SIZE * -1){

				dbg("O Won\n");
		}
	}

	if(turn_num == BOARD_SIZE * BOARD_SIZE){
		dbg("DRAW \n");
	}

}


void main_loop(){
	char key;
	signed int l_diag, r_diag, add = 0;
	for(;;){
	key = getch();
		switch(key){
			case 'j':
				cursor_y += 1;
				break;
			case 'k':
				cursor_y -= 1;
				break;
			case 'h':
				cursor_x -= 1;
				break;
			case 'l':
				cursor_x += 1;
				break;
			case ' ':
				if(inch() == ' '){ // logic merges with presentiation. not good.
					if(cursor_x - cursor_y == 0)
						dbg("l_diag nigga\n");

					if(cursor_x + cursor_y == BOARD_SIZE - 1)
						r_diag = 1;

					if(turn_num % 2 == 0){
						addch('X');
						add = 1;
					} else {
						addch('O');
						add = -1;
					}

					board[0][cursor_x] += add;				
					board[1][cursor_y] += add;
					if(l_diag)
						diagonal[0] += add;
					if(r_diag)
						diagonal[1] += add;
					turn_num++;
					win_or_draw(add);
				}

		}

		move(board_y + cursor_y * 2, board_x + cursor_x * 4 );
		refresh();
		/* reset variables */
		l_diag = 0;
		r_diag = 0;
	}
}

void zero_board(){
	int i, j;
	for(i = 0; i < 2; i++){
		for(j = 0; j < BOARD_SIZE; j++){
			board[i][j] = 0;
		}
	}
}
	       			       

void start_game(){
	int x = (COLS / 2) - (BOARD_SIZE * 4) / 2;
	int y = (int) LINES * 0.2;
	draw_board(x,y);
	board_x = x + 1;
	board_y = y;
	zero_board(); // Safely zero out board array
	wmove(stdscr, board_y, board_x);
	refresh();
	main_loop();
}


int main(int argc, char **argv){
	setlocale(LC_ALL, "");
	initscr();
	cbreak();
	noecho();
	start_game();
	exit(EXIT_SUCCESS);
}
