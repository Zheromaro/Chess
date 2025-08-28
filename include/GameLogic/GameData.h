#ifndef GAME_DATA_H
#define GAME_DATA_H

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600
#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)
#define FONTSIZE 500

#define NUM_ROW_COL 8
#define BOARD_WIDTH (WINDOW_WIDTH * 0.97)
#define BOARD_HEIGHT (WINDOW_WIDTH * 0.97)
#define CELL_WIDTH (BOARD_WIDTH / NUM_ROW_COL)
#define CELL_HEIGHT (BOARD_HEIGHT / NUM_ROW_COL)

typedef enum ChessGameStates {
    STILL_PLAYING = 1,
    BLACK_PLAYER_WON = 2,
    WHITE_PLAYER_WON = -2,
    DRAW = 0
} ChessGameStates;


// structs
typedef struct GameData
{
    ChessGameStates GameState;
} GameData;

#endif