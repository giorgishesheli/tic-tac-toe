#ifndef engine_H
#define engine_H


#define _X_ 0 /* SINGAL X */
#define _O_ 1 /* SIGNAL O */
#define _DRAW_ 2 /* SIGNAL DRAW */
#define _PASS_ 3 /* SIGNAL NOTHING HAPPENED */


#define SUM_X game->logical_board[game->x][game->size]
#define SUM_Y game->logical_board[game->size][game->y]
#define SUM_LEFT game->logical_board[game->size + 1][0]
#define SUM_RIGHT game->logical_board[game->size + 1][1]


signed int process_input(struct game_state *game);

#endif
