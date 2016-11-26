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

int cursor_x, cursor_y = 0; // logical cursor location
int board_x, board_y;  // Board location

int board[BOARD_SIZE][BOARD_SIZE];

int turn_num = 0; 



void draw_board(int init_x, int init_y){
	//top horizontal line
	mvaddch(init_y + 1, init_x, ACS_HLINE);
	mvaddch(init_y + 1, init_x + 1, ACS_HLINE);
	mvaddch(init_y + 1, init_x + 2, ACS_HLINE);
	mvaddch(init_y + 1, init_x + 3, ACS_PLUS);
	mvaddch(init_y + 1, init_x + 4, ACS_HLINE);
	mvaddch(init_y + 1, init_x + 5, ACS_HLINE);
	mvaddch(init_y + 1, init_x + 6, ACS_HLINE);
	mvaddch(init_y + 1, init_x + 7, ACS_PLUS);
	mvaddch(init_y + 1, init_x + 8, ACS_HLINE);
	mvaddch(init_y + 1, init_x + 9, ACS_HLINE);
	mvaddch(init_y + 1, init_x + 10, ACS_HLINE);
	//bottom horizontal line
	mvaddch(init_y + 3, init_x, ACS_HLINE);
	mvaddch(init_y + 3, init_x + 1, ACS_HLINE);
	mvaddch(init_y + 3, init_x + 2, ACS_HLINE);
	mvaddch(init_y + 3, init_x + 3, ACS_PLUS);
	mvaddch(init_y + 3, init_x + 4, ACS_HLINE);
	mvaddch(init_y + 3, init_x + 5, ACS_HLINE);
	mvaddch(init_y + 3, init_x + 6, ACS_HLINE);
	mvaddch(init_y + 3, init_x + 7, ACS_PLUS);
	mvaddch(init_y + 3, init_x + 8, ACS_HLINE);
	mvaddch(init_y + 3, init_x + 9, ACS_HLINE);
	mvaddch(init_y + 3, init_x + 10, ACS_HLINE);
	//left vertical line
	mvaddch(init_y, init_x + 3, ACS_VLINE);
	mvaddch(init_y + 2, init_x + 3, ACS_VLINE);
	mvaddch(init_y + 4, init_x + 3, ACS_VLINE);
	//right vertical line
	mvaddch(init_y, init_x + 7, ACS_VLINE);
	mvaddch(init_y + 2, init_x + 7, ACS_VLINE);
	mvaddch(init_y + 4, init_x + 7, ACS_VLINE);
}

int sum(int *array){
	int i;
	int result = 0;
	for(i = 0; i < 3; i++){
		result += array[i];
	}
	return result;
}

void win_or_draw(){

	int hor_array[] = { board[0][cursor_y], board[1][cursor_y], board[2][cursor_y] };
	if(sum(board[cursor_x]) == 12 || sum(hor_array) == 12){
		dbg("O Won\n");
	}
	if(sum(board[cursor_x]) == 3 || sum(hor_array) == 3){
		dbg("X Won \n");
	}

	if(turn_num == 9){
		dbg("Draw \n");
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
				if(turn_num % 2 == 0){
					addch('X');
					board[cursor_x][cursor_y] = 1;				
				} else {
					addch('O');
					board[cursor_x][cursor_y] = 4;				
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
	int x = (COLS / 2) - 8;
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
