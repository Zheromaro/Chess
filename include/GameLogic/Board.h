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

typedef struct Board
{
    // White pieces
    Bitboard white_pawns;
    Bitboard white_knights;
    Bitboard white_bishops;
    Bitboard white_rooks;
    Bitboard white_queen;
    Bitboard white_king;

    // Black pieces
    Bitboard black_pawns;
    Bitboard black_knights;
    Bitboard black_bishops;
    Bitboard black_rooks;
    Bitboard black_queen;
    Bitboard black_king;

    // Other
    Bitboard cliked_square;
    Bitboard occupied_squares;
    Bitboard white_occupied;
    Bitboard black_occupied;
    Bitboard abailable_moves;
} Board;

void init_board(Board* board);
void click_on_square(Board* board, uint64_t square);
void place_piece(Bitboard *board, uint64_t square);
void remove_piece(Bitboard *board, uint64_t square);
bool is_occupied(Bitboard board, uint64_t square);
void update_board(Board* board);
void print_bitboard(Bitboard board);

#endif