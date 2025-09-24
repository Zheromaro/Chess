#include "GameLogic/MoveGenerator.h"

static inline Bitboard shift(Bitboard bb, int shift);

// Precomputed file masks to prevent wrapping
const uint64_t notA  = 0xfefefefefefefefeULL; // ~file A
const uint64_t notAB = 0xfcfcfcfcfcfcfcfcULL; // ~files A,B
const uint64_t notH  = 0x7f7f7f7f7f7f7f7fULL; // ~file H
const uint64_t notGH = 0x3f3f3f3f3f3f3f3fULL; // ~files G,H

Bitboard pawn_moves(Players player, Board board, short square) {
    Bitboard pawnPlace = 1ULL << square;
    Bitboard moves = 0ULL;

    // Direction offsets
    bool goingUp = (player == WHITE_PLAYER);
    int forwardShift  = goingUp ? +8 : -8;
    int leftCapShift  = goingUp ? +7 : -7;
    int rightCapShift = goingUp ? +9 : -9;

    Bitboard opponentOccupied = (player == WHITE_PLAYER) ? board.black_occupied : board.white_occupied;

    // Single push
    Bitboard singlePush = shift(pawnPlace, forwardShift) & ~board.occupied_squares;
    moves |= singlePush;

    // Double push (only if pawn on starting rank)
    Bitboard startRankMask = goingUp ? 0x000000000000FF00ULL   // rank 2
                                     : 0x00FF000000000000ULL;  // rank 7

    if (pawnPlace & startRankMask) {
        Bitboard doublePush = shift(singlePush, forwardShift) & ~board.occupied_squares;
        moves |= doublePush;
    }

    // Captures
    Bitboard leftCapture  = shift(pawnPlace & notA, leftCapShift)  & opponentOccupied;
    Bitboard rightCapture = shift(pawnPlace & notH, rightCapShift) & opponentOccupied;
    moves |= leftCapture | rightCapture;

    // En passant
    if (board.en_passant_square != -1) {
        Bitboard enPassantTarget = 1ULL << board.en_passant_square;

        Bitboard leftEP = shift(pawnPlace & notA, leftCapShift)  & enPassantTarget;            
        Bitboard rightEP = shift(pawnPlace & notH, rightCapShift) & enPassantTarget;
        moves |= leftEP | rightEP;
    }

    return moves;
}
Bitboard rook_moves(Players player, Board board, short square)
{
    Bitboard moves = 0ULL;
    int rank = square / 8;
    int file = square % 8;

    // Up
    for (int r = rank + 1; r < 8; r++) {
        int sq = r * 8 + file;

        if (board.occupied_squares & (1ULL << sq)) // if blocked
        {
            if (board.black_occupied & (1ULL << sq) && player == WHITE_PLAYER) {
                moves |= (1ULL << sq); // capture
            } else if (board.white_occupied & (1ULL << sq) && player == BLACK_PLAYER) {
                moves |= (1ULL << sq); // capture
            }
            break; // stop at first occupied square
        }
        else // empty square
        {
            moves |= (1ULL << sq);
        }
    }

    // Down
    for (int r = rank - 1; r >= 0; r--) {
        int sq = r * 8 + file;
        
        if (board.occupied_squares & (1ULL << sq))
        {
            if (board.black_occupied & (1ULL << sq) && player == WHITE_PLAYER) {
                moves |= (1ULL << sq);
            } else if (board.white_occupied & (1ULL << sq) && player == BLACK_PLAYER) {
                moves |= (1ULL << sq);
            }
            break;
        }
        else // empty square
        {
            moves |= (1ULL << sq);
        }
    }

    // Right
    for (int f = file + 1; f < 8; f++) {
        int sq = rank * 8 + f;
        
        if (board.occupied_squares & (1ULL << sq))
        {
            if (board.black_occupied & (1ULL << sq) && player == WHITE_PLAYER) {
                moves |= (1ULL << sq);
            } else if (board.white_occupied & (1ULL << sq) && player == BLACK_PLAYER) {
                moves |= (1ULL << sq);
            }
            break;
        }
        else // empty square
        {
            moves |= (1ULL << sq);
        }
    }

    // Left
    for (int f = file - 1; f >= 0; f--) {
        int sq = rank * 8 + f;
        
        if (board.occupied_squares & (1ULL << sq))
        {
            if (board.black_occupied & (1ULL << sq) && player == WHITE_PLAYER) {
                moves |= (1ULL << sq);
            } else if (board.white_occupied & (1ULL << sq) && player == BLACK_PLAYER) {
                moves |= (1ULL << sq);
            }
            break;
        }
        else // empty square
        {
            moves |= (1ULL << sq);
        }
    }

    return moves;
}
Bitboard bishop_moves(Players player, Board board, short square)
{
    Bitboard moves = 0ULL;
    int rank = square / 8;
    int file = square % 8;

    // Up-right
    for (int r = rank + 1, f = file + 1; r < 8 && f < 8; r++, f++)
    {
        int sq = (r * 8 + f);
        
        if (board.occupied_squares & (1ULL << sq))
        {
            if (board.black_occupied & (1ULL << sq) && player == WHITE_PLAYER ||
                board.white_occupied & (1ULL << sq) && player == BLACK_PLAYER) 
                moves |= (1ULL << sq);
            
            break;
        }
        else // empty square
        {
            moves |= (1ULL << sq);
        }
    }

    // Up-left
    for (int r = rank + 1, f = file - 1; r < 8 && f >= 0; r++, f--)
    {
        int sq = (r * 8 + f);
        
        if (board.occupied_squares & (1ULL << sq))
        {
            if (board.black_occupied & (1ULL << sq) && player == WHITE_PLAYER ||
                board.white_occupied & (1ULL << sq) && player == BLACK_PLAYER) 
                moves |= (1ULL << sq);
            
            break;
        }
        else // empty square
        {
            moves |= (1ULL << sq);
        }
    }

    // Down-right
    for (int r = rank - 1, f = file + 1; r >= 0 && f < 8; r--, f++)
    {
        int sq = (r * 8 + f);
        
        if (board.occupied_squares & (1ULL << sq))
        {
            if (board.black_occupied & (1ULL << sq) && player == WHITE_PLAYER ||
                board.white_occupied & (1ULL << sq) && player == BLACK_PLAYER) 
                moves |= (1ULL << sq);

            break;
        }
        else // empty square
        {
            moves |= (1ULL << sq);
        }
    }

    // Down-left
    for (int r = rank - 1, f = file - 1; r >= 0 && f >= 0; r--, f--)
    {
        int sq = (r * 8 + f);
        
        if (board.occupied_squares & (1ULL << sq))
        {
            if (board.black_occupied & (1ULL << sq) && player == WHITE_PLAYER ||
                board.white_occupied & (1ULL << sq) && player == BLACK_PLAYER) 
                moves |= (1ULL << sq);

            break;
        }
        else // empty square
        {
            moves |= (1ULL << sq);
        }
    }

    return moves;
}
Bitboard knight_moves(Players player, Board board, short square)
{
    Bitboard bitboard = 1ULL << square;
    Bitboard moves = 0ULL;

    moves |= (bitboard & notH)  << 17; // 2 up, 1 right
    moves |= (bitboard & notA)  << 15; // 2 up, 1 left
    moves |= (bitboard & notGH) << 10; // 1 up, 2 right
    moves |= (bitboard & notAB) << 6;  // 1 up, 2 left
    moves |= (bitboard & notH)  >> 15; // 2 down, 1 right
    moves |= (bitboard & notA)  >> 17; // 2 down, 1 left
    moves |= (bitboard & notGH) >> 6;  // 1 down, 2 right
    moves |= (bitboard & notAB) >> 10; // 1 down, 2 left

    if (board.black_occupied & moves && player == BLACK_PLAYER)
        moves &= ~board.black_occupied; // Remove moves that land on ally pieces
    else if (board.white_occupied & moves && player == WHITE_PLAYER)
        moves &= ~board.white_occupied;
    
    return moves;
}
Bitboard queen_moves(Players player, Board board, short square)
{
    return rook_moves(player, board, square) | bishop_moves(player, board, square);
}
Bitboard king_moves(Players player, Board board, short square)
{
    Bitboard bitboard = 1ULL << square;
    Bitboard moves = 0ULL;

    moves |= (bitboard & notA) << 7;   // up-left
    moves |= bitboard << 8;            // up
    moves |= (bitboard & notH) << 9;   // up-right
    moves |= (bitboard & notA) >> 1;   // left
    moves |= (bitboard & notH) << 1;   // right
    moves |= (bitboard & notA) >> 9;   // down-left
    moves |= bitboard >> 8;            // down
    moves |= (bitboard & notH) >> 7;   // down-right
    
    // Remove moves that land on attacked squares
    Players opponent = (player == BLACK_PLAYER) ? WHITE_PLAYER : BLACK_PLAYER;
    moves &= ~board.attaked_squares[opponent];

    // Remove moves that land on ally pieces
    if (board.black_occupied & moves && player == BLACK_PLAYER)
        moves &= ~board.black_occupied;
    else if (board.white_occupied & moves && player == WHITE_PLAYER)
        moves &= ~board.white_occupied;

    // Castling
    if (player == WHITE_PLAYER && square == 4) { // e1
        // King-side (e1 → g1)
        if (!(board.occupied_squares & ((1ULL<<F1) | (1ULL<<G1))) && // f1,g1 empty
            !(board.attaked_squares[BLACK_PLAYER] & ((1ULL<<F1) | (1ULL<<G1))) && // f1,g1 not attacked
            board.castling_rights & WKING_SIDE_CASTLE_RIGHT)
        {
            moves |= (1ULL << 6); // g1
        }

        // Queen-side (e1 → c1)
        if (!(board.occupied_squares & ((1ULL<<B1) | (1ULL<<C1) | (1ULL<<D1))) && // b1,c1,d1 empty 
            !(board.attaked_squares[BLACK_PLAYER] & ((1ULL<<B1) | (1ULL<<C1) | (1ULL<<D1))) && // b1,c1,d1 not attacked 
            board.castling_rights & WQUEEN_SIDE_CASTLE_RIGHT)
        {
            moves |= (1ULL << 2); // c1
        }
    }

    if (player == BLACK_PLAYER && square == 60) { // e8
        // King-side (e8 → g8)
        if (!(board.occupied_squares & ((1ULL<<F8) | (1ULL<<G8))) && // f8,g8 empty
            !(board.attaked_squares[WHITE_PLAYER] & ((1ULL<<F8) | (1ULL<<G8))) && // f8,g8 not attacked
            board.castling_rights & BKING_SIDE_CASTLE_RIGHT)
        {
            moves |= (1ULL << 62); // g8
        }

        // Queen-side (e8 → c8)
        if (!(board.occupied_squares & ((1ULL<<B8) | (1ULL<<C8) | (1ULL<<D8))) && // b8,c8,d8 empty
            !(board.attaked_squares[WHITE_PLAYER] & ((1ULL<<B8) | (1ULL<<C8) | (1ULL<<D8))) && // b8,c8,d8 not attacked
            board.castling_rights & BQUEEN_SIDE_CASTLE_RIGHT)
        {
            moves |= (1ULL << 58); // c8
        }
    }

    return moves;
}

// private
static inline Bitboard shift(Bitboard bb, int shift)
{
    return (shift > 0) ? (bb << shift) : (bb >> -shift);
}
