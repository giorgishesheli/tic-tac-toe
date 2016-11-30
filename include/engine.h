#ifndef engine_H
#define engine_H


#define X_WON 0
#define O_WON 1
#define DRAW 2


#define SUM_X game->sum[0][game->x]
#define SUM_Y game->sum[1][game->y]
#define SUM_LEFT *game->sum[2]
#define SUM_RIGHT *game->sum[3]


void free_game_struct(struct game_state *game);
void draw_board(struct game_state *game);
void main_loop();

#endif
