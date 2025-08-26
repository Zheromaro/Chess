#include <stdio.h>
#include "GameLogic/Board.h"

void init_board(Board* board)
{
    // White pieces
    board->white_pawns   = 0x000000000000FF00ULL;
    board->white_knights = 0x0000000000000042ULL;
    board->white_bishops = 0x0000000000000024ULL;
    board->white_rooks   = 0x0000000000000081ULL;
    board->white_queen   = 0x0000000000000008ULL;
    board->white_king    = 0x0000000000000010ULL;
    // Black pieces
    board->black_pawns   = 0x00FF000000000000ULL;
    board->black_knights = 0x4200000000000000ULL;
    board->black_bishops = 0x2400000000000000ULL;
    board->black_rooks   = 0x8100000000000000ULL;
    board->black_queen   = 0x0800000000000000ULL;
    board->black_king    = 0x1000000000000000ULL;
    // Other
    board->cliked_square = 0x0000000000000000ULL;
    board->abailable_moves = 0x0000000000000000ULL;
}

void init_board_empty(Board* board)
{
    // White pieces
    board->white_pawns   = 0x0000000000000000ULL;
    board->white_knights = 0x0000000000000000ULL;
    board->white_bishops = 0x0000000000000000ULL;
    board->white_rooks   = 0x0000000000000000ULL;
    board->white_queen   = 0x0000000000000000ULL;
    board->white_king    = 0x0000000000000000ULL;
    // Black pieces
    board->black_pawns   = 0x0000000000000000ULL;
    board->black_knights = 0x0000000000000000ULL;
    board->black_bishops = 0x0000000000000000ULL;
    board->black_rooks   = 0x0000000000000000ULL;
    board->black_queen   = 0x0000000000000000ULL;
    board->black_king    = 0x0000000000000000ULL;
    // Other
    board->cliked_square = 0x0000000000000000ULL;
    board->abailable_moves = 0x0000000000000000ULL;
}

void click_on_square(Board* board, uint64_t square) {
    if (board->cliked_square == square)
        board->cliked_square = 0;
    else
        board->cliked_square = square;
}

void place_piece(Bitboard *board, uint64_t square) {
    *board |= (1ULL << square);
}

void remove_piece(Bitboard *board, uint64_t square) {
    *board &= ~(1ULL << square);
}

bool is_occupied(Bitboard board, uint64_t square) {
    return (board >> square) & 1ULL;
}

void print_bitboard(Bitboard board) {
    for (int row = 7; row >= 0; row--) {
        for (int col = 0; col < 8; col++) {
            int square = row * 8 + col;       // 0 = a1, 63 = h8
            printf("%c ", (board & (1ULL << square)) ? '1' : '.');
        }
        printf("\n");
    }
    printf("\n");
}

void update_board(Board* board) 
{
    board->white_occupied = board->white_pawns | board->white_knights |
                            board->white_bishops | board->white_rooks |
                            board->white_queen | board->white_king;

    board->black_occupied = board->black_pawns | board->black_knights |
                            board->black_bishops | board->black_rooks |
                            board->black_queen | board->black_king;

    board->occupied_squares = board->white_occupied | board->black_occupied;
}
