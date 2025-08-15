#ifndef GAME_DATA_H
#define GAME_DATA_H

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600
#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)
#define FONTSIZE 500

#define GRID_SIZE 100
#define NUM_ROW_COL 8
#define CELL_HEIGHT ((WINDOW_HEIGHT / NUM_ROW_COL))
#define CELL_WIDTH ((WINDOW_WIDTH / NUM_ROW_COL))

typedef enum ChessGameStates {
    STILL_PLAYING = 1,
    BLACK_PLAYER_WON = 2,
    WHITE_PLAYER_WON = -2,
    DRAW = 0
} ChessGameStates;
typedef enum Players {
    WHITE_PLAYER,
    BLACK_PLAYER
} Players;
typedef enum Pieces {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    KING,
    QUEEN
} Pieces;
typedef enum PieceColor {
    BLACK,
    WHITE
} PieceColor;

// structs
typedef struct SquareStates
{
    bool isEmpty;
    PieceColor pieceColor;
    Pieces piece;
} SquareStates;
typedef struct GameData
{
    ChessGameStates GameState;
    SquareStates board[NUM_ROW_COL][NUM_ROW_COL];
    Players currentPlayer;
} GameData;

#endif