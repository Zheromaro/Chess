#include <stdio.h>
#include "GameLogic/Board.h"
#include "GameLogic/MoveGenerator.h"
#include "GameLogic/AttackGenerator.h"
#include "GameLogic/MoveAction.h"

bool move_piece(Board *board, short from, short to, Players owner, PieceType piece);

// Special Moves
void apply_capture(Board *board, short from, short to, Players owner, PieceType piece);
void apply_en_passant(Board *board, short from, short to, Players owner);
void apply_castling(Board *board, short from, short to, Players owner);
void apply_promotion(Board *board, short square, Players owner);

// Rule Updates
void update_en_passant_rights(Board *board, short from, short to, Players owner, PieceType piece);
void update_castling_rights(Board *board, short from, short to, Players owner, PieceType piece);
void update_attaked_squares(Board *board);

// Utilities
PieceType get_piece_type(Board board, short square, Players owner);
Bitboard get_piece_moves(Board board, short square, Players owner, PieceType piece);
Bitboard get_pieces_attacks(Board board, Players owner);
Players switch_player(Players current);


void click_on_square(Board* board, Players *player, short square_index)
{
    PieceType oldPieceType = get_piece_type(*board, board->cliked_square_index, *player);
    if (move_piece(board, board->cliked_square_index, square_index, *player, oldPieceType)) 
    {
        *player = switch_player(*player);
        return;
    }

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

// ---------- private ------------
bool move_piece(Board *board, short from, short to, Players owner, PieceType piece)
{
    if (!(board->available_moves & (1ULL << to))) return false;
    
    remove_piece(&(board->pieces[owner][piece]), from);
    place_piece (&(board->pieces[owner][piece]), to);
    
    apply_capture(board, from, to, owner, piece);
    if (piece == PAWN)
    {
        apply_promotion(board, to, owner);
        apply_en_passant(board, from, to, owner);
    }
    else if (piece == KING)
        apply_castling(board, from, to, owner);
    
    update_board(board);
    update_en_passant_rights(board, from, to, owner, piece);
    update_castling_rights(board, from, to, owner, piece);
    update_attaked_squares(board);

    board->cliked_square_index = 0;
    board->available_moves = 0;
    
    return true;
}

void apply_capture(Board *board, short from, short to, Players owner, PieceType piece)
{
    for (int i = 0; i < 6; i++)
    {
        if (is_occupied(board->pieces[!(owner)][i], to))
        {
            remove_piece(&(board->pieces[!(owner)][i]), to);
            break;
        }
    }
}
void apply_en_passant(Board *board, short from, short to, Players owner)
{   
    // capture en_passant
    if (to == board->en_passant_square)
    {
        if (owner == WHITE_PLAYER)
            remove_piece(&(board->pieces[BLACK_PLAYER][PAWN]), to - 8);
        else
            remove_piece(&(board->pieces[WHITE_PLAYER][PAWN]), to + 8);

        // Reset en passant square
        board->en_passant_square = -1;
    }
}
void apply_promotion(Board *board, short square, Players owner)
{
    // Check if the pawn is in the promotion rank for the current player
    if ((owner == WHITE_PLAYER && square / 8 != 7) ||
        (owner == BLACK_PLAYER && square / 8 != 0))
        return; // Not in promotion rank

    remove_piece(&(board->pieces[owner][PAWN]), square);
    place_piece(&(board->pieces[owner][QUEEN]), square);
}
void apply_castling(Board *board, short from, short to, Players owner)
{   
    // Kingside castling
    if (to == from + 2)
    {
        remove_piece(&(board->pieces[owner][ROOK]), from + 3);
        place_piece (&(board->pieces[owner][ROOK]), from + 1);
    }
    // Queenside castling
    else if (to == from - 2) 
    {
        remove_piece(&(board->pieces[owner][ROOK]), from - 4);
        place_piece (&(board->pieces[owner][ROOK]), from - 1);
    }
}

void update_en_passant_rights(Board *board, short from, short to, Players owner, PieceType piece)
{
    if (piece == PAWN && (owner == WHITE_PLAYER) && (from / 8 == 1) && (to / 8 == 3)) 
        board->en_passant_square = from + 8;
    else if (piece == PAWN && (owner == BLACK_PLAYER) && (from / 8 == 6) && (to / 8 == 4))
        board->en_passant_square = from - 8;
    else
        board->en_passant_square = -1;
} 
void update_castling_rights(Board *board, short from, short to, Players owner, PieceType piece)
{
    if (piece == KING)
    {
        if (owner == WHITE_PLAYER) // Remove white king castling right
            board->castling_rights &= ~(WKING_SIDE_CASTLE_RIGHT | WQUEEN_SIDE_CASTLE_RIGHT);
        else                        // Remove black king castling right
            board->castling_rights &= ~(BKING_SIDE_CASTLE_RIGHT | BQUEEN_SIDE_CASTLE_RIGHT);
    }
    else if (piece == ROOK)
    {
        if (owner == WHITE_PLAYER) 
        {
            if (from == 0)       // Remove white queenside castling right
                board->castling_rights &= ~WQUEEN_SIDE_CASTLE_RIGHT;
            else if (from == 7)  // Remove white kingside castling right
                board->castling_rights &= ~WKING_SIDE_CASTLE_RIGHT;
        } 
        else 
        {
            if (from == 56)      // Remove black queenside castling right
                board->castling_rights &= ~BQUEEN_SIDE_CASTLE_RIGHT;
            else if (from == 63) // Remove black kingside castling right
                board->castling_rights &= ~BKING_SIDE_CASTLE_RIGHT;
        }
    }
}
void update_attaked_squares(Board *board)
{
    board->attaked_squares[WHITE_PLAYER] = 0;
    board->attaked_squares[BLACK_PLAYER] = 0;
    
    board->attaked_squares[WHITE_PLAYER] |= pawns_attack(WHITE_PLAYER, *board);
    board->attaked_squares[WHITE_PLAYER] |= knights_attack(WHITE_PLAYER, *board);
    board->attaked_squares[WHITE_PLAYER] |= bishops_attack(WHITE_PLAYER, *board);
    board->attaked_squares[WHITE_PLAYER] |= rooks_attack(WHITE_PLAYER, *board);
    board->attaked_squares[WHITE_PLAYER] |= queens_attack(WHITE_PLAYER, *board);
    board->attaked_squares[WHITE_PLAYER] |= king_attack(WHITE_PLAYER, *board);

    board->attaked_squares[BLACK_PLAYER] |= pawns_attack(BLACK_PLAYER, *board);
    board->attaked_squares[BLACK_PLAYER] |= knights_attack(BLACK_PLAYER, *board);
    board->attaked_squares[BLACK_PLAYER] |= bishops_attack(BLACK_PLAYER, *board);
    board->attaked_squares[BLACK_PLAYER] |= rooks_attack(BLACK_PLAYER, *board);
    board->attaked_squares[BLACK_PLAYER] |= queens_attack(BLACK_PLAYER, *board);
    board->attaked_squares[BLACK_PLAYER] |= king_attack(BLACK_PLAYER, *board);

    print_bitboard(board->attaked_squares[WHITE_PLAYER]);
    print_bitboard(board->attaked_squares[BLACK_PLAYER]);
}

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
PieceType get_piece_type(Board board, short square, Players owner)
{
    uint64_t mask = 1ULL << square;

    if (board.pieces[owner][PAWN]   & mask) return PAWN;
    if (board.pieces[owner][KNIGHT] & mask) return KNIGHT;
    if (board.pieces[owner][BISHOP] & mask) return BISHOP;
    if (board.pieces[owner][ROOK]   & mask) return ROOK;
    if (board.pieces[owner][QUEEN]  & mask) return QUEEN;
    if (board.pieces[owner][KING]   & mask) return KING;

    return NONE; // empty square
}
Players switch_player(Players current)
{
    if (current == WHITE_PLAYER)
        current = BLACK_PLAYER;
    else if (current == BLACK_PLAYER)
        current = WHITE_PLAYER;
    
    return current;
}
