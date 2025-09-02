#include <stdio.h>
#include "GameLogic/Board.h"
#include "GameLogic/MoveGenerator.h"
#include "GameLogic/MoveAction.h"

PieceType get_piece_type(Board board, short square, Players owner);
Bitboard get_piece_moves(Board board, short square, Players owner, PieceType piece);
bool move_piece(Board *board, short from, short to, Players *owner, PieceType piece);
void switch_player(Players* current);


void click_on_square(Board* board, Players* player, short square_index) {
    update_board(board);
    PieceType oldPieceType = get_piece_type(*board, board->cliked_square_index, *player);
    if (move_piece(board, board->cliked_square_index, square_index, player, oldPieceType)) return;

    board->available_moves = 0;
    if (board->cliked_square_index == square_index)
    board->cliked_square_index = 0;
    else
    {
        board->cliked_square_index = square_index;
        PieceType pieceType = get_piece_type(*board, square_index, *player);
        board->available_moves = get_piece_moves(*board, square_index, *player, pieceType);
    }
    update_board(board);
}

// private
Bitboard get_piece_moves(Board board, short square, Players owner, PieceType piece)
{
    switch (piece) {
        case NONE:   return 0; break;
        case PAWN:   return pawn_moves(owner, board, square);   break;
        case KNIGHT: return knight_moves(owner, board, square); break;
        case BISHOP: return bishop_moves(owner, board, square); break;
        case ROOK:   return rook_moves(owner, board, square);   break;
        case QUEEN:  return queen_moves(owner, board, square);  break;
        case KING:   return king_moves(owner, board, square);   break;
        default: return 0;
    }  
}
PieceType get_piece_type(Board board, short square, Players owner) {
    uint64_t mask = 1ULL << square;

    if (board.pieces[owner][PAWN]   & mask) return PAWN;
    if (board.pieces[owner][KNIGHT] & mask) return KNIGHT;
    if (board.pieces[owner][BISHOP] & mask) return BISHOP;
    if (board.pieces[owner][ROOK]   & mask) return ROOK;
    if (board.pieces[owner][QUEEN]  & mask) return QUEEN;
    if (board.pieces[owner][KING]   & mask) return KING;

    return NONE; // empty square
}
bool move_piece(Board *board, short from, short to, Players *owner, PieceType piece) {
    if (!(board->available_moves & (1ULL << to))) return false;
    
    // Capture opponent piece if exists
    for (int i = 0; i < 6; i++)
    {
        if (is_occupied(board->pieces[!(*owner)][i], to))
        {
            remove_piece(&(board->pieces[!(*owner)][i]), to);
            break;
        }
    }
    remove_piece(&(board->pieces[*owner][piece]), from);
    place_piece (&(board->pieces[*owner][piece]), to);
    switch_player(owner);
    board->cliked_square_index = 0;
    board->available_moves = 0;
    
    return true;
}
void switch_player(Players* current)
{
    if (*current == WHITE_PLAYER)
        *current = BLACK_PLAYER;
    else if (*current == BLACK_PLAYER)
        *current = WHITE_PLAYER;
}