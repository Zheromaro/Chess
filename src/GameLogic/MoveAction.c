#include <stdio.h>
#include <SDL2/SDL.h>
#include "GameLogic/Board.h"
#include "GameLogic/MoveGenerator.h"
#include "GameLogic/AttackGenerator.h"
#include "GameLogic/MoveAction.h"

// Moves
bool move_piece(Board *board, short from, short to, Players owner, PieceType piece);
void filter_moves_in_check(Board *board, short from, Players owner, PieceType piece, Bitboard *moves);
bool is_legal_move(Board board, short from, short to, Players owner, PieceType piece);
bool has_legal_moves(Board board, Players current);

// Special Moves
void try_capture(Board *board, short from, short to, Players owner, PieceType piece);
void try_en_passant(Board *board, short from, short to, Players owner);
void try_castling(Board *board, short from, short to, Players owner);
bool try_promotion(Board *board, short choice, Players owner);

// Rule Updates
void update_en_passant_rights(Board *board, short from, short to, Players owner, PieceType piece);
void update_castling_rights(Board *board, short from, short to, Players owner, PieceType piece);
void update_attaked_squares(Board *board);
void update_game_state(Board *board, Players current);
void update_promotion_status(Board *board, short square, Players owner);

// Utilities
PieceType get_piece_type(Board board, short square, Players owner);
Bitboard get_piece_moves(Board board, short square, Players owner, PieceType piece);
Bitboard get_pieces_attacks(Board board, Players owner);
Players switch_player(Board* board, Players current);

void click_on_square(Board* board, Players *player, short square_index, SDL_Keycode key)
{    
    PieceType pieceType = get_piece_type(*board, board->cliked_square_index, *player);
    if (move_piece(board, board->cliked_square_index, square_index, *player, pieceType) &&
        board->promotion_square_index == -1) 
    {
        *player = switch_player(board, *player);
        return;
    }

    board->available_moves = 0;
    if (board->cliked_square_index == square_index)
        board->cliked_square_index = -1;
    else
    {
        board->cliked_square_index = square_index;
        PieceType newPieceType = get_piece_type(*board, square_index, *player);
        Bitboard newPieceMoves = get_piece_moves(*board, square_index, *player, newPieceType);
        filter_moves_in_check(board, square_index, *player, newPieceType, &newPieceMoves);
        board->available_moves = newPieceMoves;
    }
    update_board(board);
}

// ---------- private ------------
bool move_piece(Board *board, short from, short to, Players owner, PieceType piece)
{
    if (board->promotion_square_index != -1)
    {
        if (try_promotion(board, to, owner)) return true;
        else return false;
    }
    if ((to < 0 || to > 64) ||
        !(board->available_moves & (1ULL << to)))
        return false;
    printf("pppp\n");
    
    remove_piece(&(board->pieces[owner][piece]), from);
    place_piece (&(board->pieces[owner][piece]), to);
    
    try_capture(board, from, to, owner, piece);
    if (piece == PAWN)
    try_en_passant(board, from, to, owner);
    else if (piece == KING)
    try_castling(board, from, to, owner);
    
    update_board(board);
    update_en_passant_rights(board, from, to, owner, piece);
    update_promotion_status(board, to, owner);
    update_castling_rights(board, from, to, owner, piece);
    update_attaked_squares(board);
    update_game_state(board, owner);

    board->cliked_square_index = -1;
    board->available_moves = 0;
    
    return true;
}
void filter_moves_in_check(Board *board, short from, Players owner, PieceType piece, Bitboard *moves)
{
    Bitboard legal_moves = 0ULL;
    Bitboard pseudo_moves = *moves;

    while (pseudo_moves)
    {
        short to = __builtin_ctzll(pseudo_moves); // get lowest set bit
        pseudo_moves &= pseudo_moves - 1;         // pop bit
        Board temp = *board;
        if (is_legal_move(*board, from, to, owner, piece))
        {
            legal_moves |= (1ULL << to);
        }
    }

    *moves = legal_moves;
}
bool is_legal_move(Board board, short from, short to, Players owner, PieceType piece)
{
    Board temp = board;

    remove_piece(&temp.pieces[owner][piece], from);
    place_piece(&temp.pieces[owner][piece], to);
    try_capture(&temp, from, to, owner, piece);
    if (piece == PAWN) try_en_passant(&temp, from, to, owner);


    update_board(&temp);
    update_attaked_squares(&temp);

    // check if king safe
    Bitboard king = temp.pieces[owner][KING];
    bool king_in_check = (owner == WHITE_PLAYER)
                         ? (temp.attaked_squares[BLACK_PLAYER] & king)
                         : (temp.attaked_squares[WHITE_PLAYER] & king);

    return !king_in_check;
}
bool has_legal_moves(Board board, Players owner)
{
    for (int pieceType = PAWN; pieceType <= KING; pieceType++) {
        Bitboard pieces = board.pieces[owner][pieceType];
        while (pieces) {
            short from = __builtin_ctzll(pieces);
            pieces &= pieces - 1;

            Bitboard moves = get_piece_moves(board, from, owner, pieceType);
            filter_moves_in_check(&board, from, owner, pieceType, &moves);

            if (moves) return true; // found at least one legal move
        }
    }
    return false;
}

void try_capture(Board *board, short from, short to, Players owner, PieceType piece)
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
void try_en_passant(Board *board, short from, short to, Players owner)
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
bool try_promotion(Board *board, short choice, Players owner)
{
    if (!board->promotion_square_index == -1)
        return false;

    PieceType promotionPieceType = NONE;

    switch (choice)
    {
    case C5:
    case C6:
    case D5:
    case D6:
        promotionPieceType = 
            (owner == WHITE_PLAYER)
            ? QUEEN
            : BISHOP;
        break;
    case C3:
    case C4:
    case D3:
    case D4:
        promotionPieceType = 
            (owner == WHITE_PLAYER)
            ? BISHOP
            : QUEEN;
        break;
    case E5:
    case E6:
    case F5:
    case F6:
        promotionPieceType = 
            (owner == WHITE_PLAYER)
            ? KNIGHT
            : ROOK;
        break;
    case E3:
    case E4:
    case F3:
    case F4:
        promotionPieceType = 
            (owner == WHITE_PLAYER)
            ? ROOK
            : KNIGHT;
        break;
    default:
        return false;
    }

    remove_piece(&(board->pieces[owner][PAWN]), board->promotion_square_index);
    place_piece(&(board->pieces[owner][promotionPieceType]), board->promotion_square_index);
    board->promotion_square_index = -1;
    return true;
}
void try_castling(Board *board, short from, short to, Players owner)
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
}
void update_game_state(Board *board, Players current)
{
    bool white_in_check = board->attaked_squares[BLACK_PLAYER] & board->pieces[WHITE_PLAYER][KING];
    bool black_in_check = board->attaked_squares[WHITE_PLAYER] & board->pieces[BLACK_PLAYER][KING];

    if (white_in_check) {
        if (!has_legal_moves(*board, WHITE_PLAYER))
            board->state = WHITE_CHECKMATED;
        else
            board->state = WHITE_CHECKED;
    }
    else if (black_in_check) {
        if (!has_legal_moves(*board, BLACK_PLAYER))
            board->state = BLACK_CHECKMATED;
        else
            board->state = BLACK_CHECKED;
    }
    else {
        if (!has_legal_moves(*board, current)) // not in check but no moves
            board->state = STALEMATE;
        else
            board->state = STILL_PLAYING;
    }
}
void update_promotion_status(Board *board, short square, Players owner)
{
    if (owner == WHITE_PLAYER && square / 8 == 7)
        board->promotion_square_index = square;
    else if (owner == BLACK_PLAYER && square / 8 == 0)
        board->promotion_square_index = square;
    else
        board->promotion_square_index = -1;
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
    if (square < 0 || square > 64) return NONE;
    uint64_t mask = 1ULL << square;

    if (board.pieces[owner][PAWN]   & mask) return PAWN;
    if (board.pieces[owner][KNIGHT] & mask) return KNIGHT;
    if (board.pieces[owner][BISHOP] & mask) return BISHOP;
    if (board.pieces[owner][ROOK]   & mask) return ROOK;
    if (board.pieces[owner][QUEEN]  & mask) return QUEEN;
    if (board.pieces[owner][KING]   & mask) return KING;

    return NONE; // empty square
}
Players switch_player(Board* board, Players current)
{
    if (current == WHITE_PLAYER)
    {
        current = BLACK_PLAYER;
        board->fliped = false;
    }
    else if (current == BLACK_PLAYER)
    {
        current = WHITE_PLAYER;
        board->fliped = true;
    }
    
    return current;
}
