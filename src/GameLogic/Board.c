#include <stdio.h>
#include "GameLogic/Board.h"
#include "GameLogic/PieceMoveGenerator.h"

PieceType get_piece_type(Board board, short square, Players owner);
Bitboard get_piece_moves(Board board, short square, Players currentP, PieceType type);

void init_board(Board* board)
{
    // White pieces
    board->pieces[WHITE_PLAYER][PAWN]   = 0x000000000000FF00ULL;
    board->pieces[WHITE_PLAYER][KNIGHT] = 0x0000000000000042ULL;
    board->pieces[WHITE_PLAYER][BISHOP] = 0x0000000000000024ULL;
    board->pieces[WHITE_PLAYER][ROOK]   = 0x0000000000000081ULL;
    board->pieces[WHITE_PLAYER][QUEEN]  = 0x0000000000000008ULL;
    board->pieces[WHITE_PLAYER][KING]   = 0x0000000000000010ULL;
    // Black pieces
    board->pieces[BLACK_PLAYER][PAWN]   = 0x00FF000000000000ULL;
    board->pieces[BLACK_PLAYER][KNIGHT] = 0x4200000000000000ULL;
    board->pieces[BLACK_PLAYER][BISHOP] = 0x2400000000000000ULL;
    board->pieces[BLACK_PLAYER][ROOK]   = 0x8100000000000000ULL;
    board->pieces[BLACK_PLAYER][QUEEN]  = 0x0800000000000000ULL;
    board->pieces[BLACK_PLAYER][KING]   = 0x1000000000000000ULL;

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

void click_on_square(Board* board, Players player, short square_index) {
    board->available_moves = 0;
    if (board->cliked_square_index == square_index)
        board->cliked_square_index = 0;
    else
    {
        board->cliked_square_index = square_index;
        PieceType pieceType = get_piece_type(*board, square_index, player);
        board->available_moves = get_piece_moves(*board, square_index, player, pieceType);
    }
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
    for (int i = 0; i < 6; i++)
    {
        board->white_occupied |= board->pieces[WHITE_PLAYER][i];
        board->black_occupied |= board->pieces[BLACK_PLAYER][i];
    }

    board->occupied_squares = board->white_occupied | board->black_occupied;
}

// private
Bitboard get_piece_moves(Board board, short square, Players currentP, PieceType type)
{
    if (type == NONE)
        return 0;
    
    if (type == PAWN)
    {
        if (currentP == WHITE_PLAYER)
            return wpawn_moves(board, square);
        else if (currentP == BLACK_PLAYER)
            return bpawn_moves(board, square);
    }
    if (type == KNIGHT)
        return knight_moves(currentP, board, square);
    if (type == BISHOP)
        return bishop_moves(currentP, board, square);
    if (type == ROOK)
        return rook_moves(currentP, board, square);
    if (type == QUEEN)
        return queen_moves(currentP, board, square);
    if (type == KING)
        return king_moves(currentP, board, square);
}
PieceType get_piece_type(Board board, short square, Players owner) {
    uint64_t mask = 1ULL << square;

    if (board.pieces[owner][PAWN] & mask)   return PAWN;
    if (board.pieces[owner][KNIGHT] & mask) return KNIGHT;
    if (board.pieces[owner][BISHOP] & mask) return BISHOP;
    if (board.pieces[owner][ROOK] & mask)   return ROOK;
    if (board.pieces[owner][QUEEN] & mask)  return QUEEN;
    if (board.pieces[owner][KING] & mask)   return KING;

    return NONE; // empty square
}
