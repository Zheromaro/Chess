#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>
#include <stdbool.h>

#define SQUARE(row, col) ((row) * 8 + (col)) // row=0 (rank1), col=0 (file a)
typedef enum {
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8
} Square;

typedef uint64_t Bitboard;

typedef enum Players {
    WHITE_PLAYER,
    BLACK_PLAYER
} Players;

typedef enum PieceType{
    PAWN = 0,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
    NONE
} PieceType;

typedef struct Board
{
    Bitboard pieces[2][6]; // [color][piece_type]

    // Other
    short cliked_square_index;
    Bitboard occupied_squares;
    Bitboard white_occupied;
    Bitboard black_occupied;
    Bitboard available_moves;
} Board;

void init_board(Board* board);
void init_board_empty(Board* board);
void place_piece(Bitboard *board, short square);
void remove_piece(Bitboard *board, short square);
bool is_occupied(Bitboard board, short square);
void update_board(Board* board);
void print_bitboard(Bitboard board);

#endif