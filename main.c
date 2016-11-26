#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <locale.h>

#include <ncurses.h>

int wind_cursor[2];
int log_cursor[2] = {0, 0};
int board[3][3] = {
	{0, 0, 0},
	{0, 0, 0},
	{0, 0, 0}
};

int turn = 1; // 1 X's turn, 0 means it's O's turn
int turn_num = 0;

#ifdef DEBUG
#define dbg(...)  fprintf(stderr, __VA_ARGS__)
#else
#define dbg(...)
#endif


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
	/*
	dbg(" {0, 0} = %d, {0, 1} = %d, {0, 2} = %d, {1,0} = %d, {1, 1} = %d, {1, 2} = %d, {2,0} = %d, {2,1} = %d, {2,2} = %d\n",
			board[0][0], board[0][1], board[0][2],
			board[1][0], board[1][1], board[1][2],
			board[2][0], board[2][1], board[2][2] );
			*/
	if(turn_num == 9){
		dbg("Draw \n");
	}

	int hor_array[] = { board[0][log_cursor[1]], board[1][log_cursor[1]], board[2][log_cursor[1]] };
	if(sum(board[log_cursor[0]]) == 12 || sum(hor_array) == 12){
		dbg("O Won\n");
	}
	if(sum(board[log_cursor[0]]) == 3 || sum(hor_array) == 3){
		dbg("X Won \n");
	}

}


void loop(){
	char key;

	for(;;){
	key = getch();
		switch(key){
			case 'j':
				move(wind_cursor[1] + 2, wind_cursor[0]);
				refresh();
				wind_cursor[1] += 2;
				log_cursor[1]++;
				break;
			case 'k':
				move(wind_cursor[1] - 2, wind_cursor[0]);
				refresh();
				wind_cursor[1] -= 2;
				log_cursor[1]--;
				break;
			case 'h':
				move(wind_cursor[1], wind_cursor[0] - 4);
				refresh();
				wind_cursor[0] -= 4;
				log_cursor[0]--;
				break;
				break;
			case 'l':
				move(wind_cursor[1], wind_cursor[0] + 4);
				refresh();
				wind_cursor[0] += 4;
				log_cursor[0]++;
				break;
			case ' ':
				if(turn){
					addch('X');
					board[log_cursor[0]][log_cursor[1]] = 1;				
					turn = 0;
				} else {
					addch('O');
					board[log_cursor[0]][log_cursor[1]] = 4;				
					turn = 1;
				}
				move(wind_cursor[1], wind_cursor[0]);
				turn_num++;
				win_or_draw();
				dbg("log_cursor[0] = %d, log_cursor[1] = %d\n", log_cursor[0], log_cursor[1]);
				refresh();
				break;
			default:
				break;
		}
	}
}

void start_game(){
	int x = (COLS / 2) - 8;
	int y = (int) LINES * 0.2;
	draw_board(x,y);
	wind_cursor[0] = x + 1;
	wind_cursor[1] = y;
	wmove(stdscr, wind_cursor[1], wind_cursor[0]);
	refresh();
	loop();
}


int main(int argc, char **argv){
	setlocale(LC_ALL, "");
	initscr();
	cbreak();
	//curs_set(0);
	noecho();
	start_game();
	//mvaddch(11, 12, ACS_VLINE);
	//mvaddch(12, 11, ACS_HLINE);



	pause();

	exit(EXIT_SUCCESS);
}
