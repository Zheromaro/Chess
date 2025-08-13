#ifndef GAME_DATA_H
#define GAME_DATA_H

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600
#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)
#define FONTSIZE 500

#define GRID_SIZE 100
#define NUM_ROW_COL 3
#define CELL_HEIGHT ((WINDOW_HEIGHT / NUM_ROW_COL))
#define CELL_WIDTH ((WINDOW_WIDTH / NUM_ROW_COL))

typedef enum XOGameStates {
    STILL_PLAYING = 1,
    PLAYER_X_WON = 2,
    PLAYER_O_WON = -2,
    DRAW = 0
} XOGameStates;
typedef enum XOCellStates {
    EMPTY_CELL,
    X_CELL,
    O_CELL
} XOCellStates;
typedef enum XOPlayers {
    PLAYER_X,
    PLAYER_O
} XOPlayers;

typedef struct GameData
{
    XOGameStates GameState;
    XOCellStates board[NUM_ROW_COL][NUM_ROW_COL];
    XOPlayers currentPlayer;
} GameData;


#endif