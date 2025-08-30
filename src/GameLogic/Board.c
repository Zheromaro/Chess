#include <stdio.h>
#include "GameLogic/Board.h"
#include "GameLogic/MoveGenerator.h"

void init_board(Board* board)
{
    // White pieces
    board->pieces[WHITE_PLAYER][PAWN]   = 0x00FF000000000000ULL;
    board->pieces[WHITE_PLAYER][KNIGHT] = 0x4200000000000000ULL;
    board->pieces[WHITE_PLAYER][BISHOP] = 0x2400000000000000ULL;
    board->pieces[WHITE_PLAYER][ROOK]   = 0x8100000000000000ULL;
    board->pieces[WHITE_PLAYER][QUEEN]  = 0x0800000000000000ULL;
    board->pieces[WHITE_PLAYER][KING]   = 0x1000000000000000ULL;
    // Black pieces
    board->pieces[BLACK_PLAYER][PAWN]   = 0x000000000000FF00ULL;
    board->pieces[BLACK_PLAYER][KNIGHT] = 0x0000000000000042ULL;
    board->pieces[BLACK_PLAYER][BISHOP] = 0x0000000000000024ULL;
    board->pieces[BLACK_PLAYER][ROOK]   = 0x0000000000000081ULL;
    board->pieces[BLACK_PLAYER][QUEEN]  = 0x0000000000000008ULL;
    board->pieces[BLACK_PLAYER][KING]   = 0x0000000000000010ULL;

    // Other
    board->cliked_square_index = 0;
    board->available_moves  = 0x0000000000000000ULL;
    board->occupied_squares = 0x0000000000000000ULL;
    board->white_occupied   = 0x0000000000000000ULL;
    board->black_occupied   = 0x0000000000000000ULL;

    update_board(board);
}

void init_board_empty(Board* board)
{
    // White pieces
    board->pieces[WHITE_PLAYER][PAWN]   = 0x0000000000000000ULL;
    board->pieces[WHITE_PLAYER][KNIGHT] = 0x0000000000000000ULL;
    board->pieces[WHITE_PLAYER][BISHOP] = 0x0000000000000000ULL;
    board->pieces[WHITE_PLAYER][ROOK]   = 0x0000000000000000ULL;
    board->pieces[WHITE_PLAYER][QUEEN]  = 0x0000000000000000ULL;
    board->pieces[WHITE_PLAYER][KING]   = 0x0000000000000000ULL;
    // Black pieces
    board->pieces[BLACK_PLAYER][PAWN]   = 0x0000000000000000ULL;
    board->pieces[BLACK_PLAYER][KNIGHT] = 0x0000000000000000ULL;
    board->pieces[BLACK_PLAYER][BISHOP] = 0x0000000000000000ULL;
    board->pieces[BLACK_PLAYER][ROOK]   = 0x0000000000000000ULL;
    board->pieces[BLACK_PLAYER][QUEEN]  = 0x0000000000000000ULL;
    board->pieces[BLACK_PLAYER][KING]   = 0x0000000000000000ULL;
    // Other
    board->cliked_square_index = 0;
    board->available_moves  = 0x0000000000000000ULL;
    board->occupied_squares = 0x0000000000000000ULL;
    board->white_occupied   = 0x0000000000000000ULL;
    board->black_occupied   = 0x0000000000000000ULL;

    update_board(board);
}

void place_piece(Bitboard *board, short square) {
    *board |= (1ULL << square);
}

void remove_piece(Bitboard *board, short square) {
    *board &= ~(1ULL << square);
}

bool is_occupied(Bitboard board, short square) {
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
    board->white_occupied = 0;
    board->black_occupied = 0;

    for (int i = 0; i < 6; i++)
    {
        board->white_occupied |= board->pieces[WHITE_PLAYER][i];
        board->black_occupied |= board->pieces[BLACK_PLAYER][i];
    }

    board->occupied_squares = board->white_occupied | board->black_occupied;
}
