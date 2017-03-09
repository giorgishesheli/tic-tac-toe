#ifndef engine_H
#define engine_H


#define _X_ 1 /* SINGAL X */
#define _O_ 2 /* SIGNAL O */
#define _DRAW_ 3 /* SIGNAL DRAW */
#define _PASS_ 4 /* SIGNAL THAT NOTHING HAPPENED*/
#define _END_GAME_ 5 /* SIGNAL TO END GAME */


#define SUM_X game->logical_board[game->x][game->size]
#define SUM_Y game->logical_board[game->size][game->y]
#define SUM_LEFT game->logical_board[game->size + 1][0]
#define SUM_RIGHT game->logical_board[game->size + 1][1]


signed int process_input(struct game_state *game);

#endif
