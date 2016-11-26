#include <stdlib.h>

#include <unistd.h>
#include <locale.h>

#include <ncurses.h>

#ifndef BOARD_SIZE
#define BOARD_SIZE 9
#endif

#ifdef DEBUG
#define dbg(...)  fprintf(stderr, __VA_ARGS__)
#else
#define dbg(...)
#endif

int cursor_x, cursor_y = 0; // logical cursor location
int board_x, board_y;  // Initial board location

signed int board[2][BOARD_SIZE];

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


void win_or_draw(){

	if(board[0][cursor_x] == BOARD_SIZE * -1 || board[1][cursor_y] == BOARD_SIZE * -1){
		dbg("O Won\n");
	}
	if(board[0][cursor_x] == BOARD_SIZE || board[1][cursor_y] == BOARD_SIZE){
		dbg("X Won\n");
	}

	if(turn_num == BOARD_SIZE * BOARD_SIZE){
		dbg("DRAW \n");
	}

}


void loop(){
	char key;
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
					if(turn_num % 2 == 0){
						addch('X');
						board[0][cursor_x] += 1;				
						board[1][cursor_y] += 1;
					} else {
						addch('O');
						board[0][cursor_x] -= 1;				
						board[1][cursor_y] -= 1;
					}
				}

				turn_num++;
				win_or_draw();
		}

		move(board_y + cursor_y * 2, board_x + cursor_x * 4 );
		refresh();
	}
}

void zero_board(){
	int i, j;
	for(i = 0; i < BOARD_SIZE; i++){
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
	loop();
}


int main(int argc, char **argv){
	setlocale(LC_ALL, "");
	initscr();
	cbreak();
	noecho();
	start_game();
	exit(EXIT_SUCCESS);
}
