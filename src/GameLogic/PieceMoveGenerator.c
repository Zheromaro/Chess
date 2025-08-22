#include "GameLogic/PieceMoveGenerator.h"

// Precomputed file masks to prevent wrapping
const uint64_t notA  = 0xfefefefefefefefeULL; // ~file A
const uint64_t notAB = 0xfcfcfcfcfcfcfcfcULL; // ~files A,B
const uint64_t notH  = 0x7f7f7f7f7f7f7f7fULL; // ~file H
const uint64_t notGH = 0x3f3f3f3f3f3f3f3fULL; // ~files G,H

Bitboard wpawn_moves(Players player, Board board, uint64_t square)
{
    Bitboard pawnPlace = (1ULL << square);
    Bitboard moves = 0ULL;

    // Single push (forward 8)
    uint64_t singlePush = (pawnPlace << 8) & ~board.occupied_squares;
    moves |= singlePush;

    // Double push (only from rank 2 → rank 4)
    uint64_t rank2 = 0x000000000000FF00ULL;
    uint64_t doublePush = ((singlePush & rank2) << 8) & ~board.occupied_squares;
    moves |= doublePush;

    // Captures
    uint64_t leftCapture  = (pawnPlace << 7) & board.black_occupied & notA;
    uint64_t rightCapture = (pawnPlace << 9) & board.black_occupied & notH;
    moves |= leftCapture | rightCapture;

    return moves;
}
Bitboard bpawn_moves(Players player, Board board, uint64_t square)
{
    Bitboard pawnPlace = (1ULL << square);
    Bitboard moves = 0ULL;

    // Single push (forward 8)
    uint64_t singlePush = (pawnPlace >> 8) & ~board.occupied_squares;
    moves |= singlePush;

    // Double push (only from rank 2 → rank 4)
    uint64_t rank7 = 0x00FF000000000000ULL;
    uint64_t doublePush = ((singlePush & rank7) >> 8) & ~board.occupied_squares;
    moves |= doublePush;

    // Captures
    uint64_t leftCapture  = (pawnPlace >> 7) & board.black_occupied & notA;
    uint64_t rightCapture = (pawnPlace >> 9) & board.black_occupied & notH;
    moves |= leftCapture | rightCapture;

    return moves;
}
Bitboard rook_moves(Players player, Board board, uint64_t square)
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
    }

    return moves;
}
Bitboard bishop_moves(Players player, Board board, uint64_t square)
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
            if (board.black_occupied & (1ULL << sq) && player == WHITE_PLAYER) {
                moves |= (1ULL << sq);
            } else if (board.white_occupied & (1ULL << sq) && player == BLACK_PLAYER) {
                moves |= (1ULL << sq);
            }
            break;
        }
    }

    // Up-left
    for (int r = rank + 1, f = file - 1; r < 8 && f >= 0; r++, f--)
    {
        int sq = (r * 8 + f);
        
        if (board.occupied_squares & (1ULL << sq))
        {
            if (board.black_occupied & (1ULL << sq) && player == WHITE_PLAYER) {
                moves |= (1ULL << sq);
            } else if (board.white_occupied & (1ULL << sq) && player == BLACK_PLAYER) {
                moves |= (1ULL << sq);
            }
            break;
        }
    }

    // Down-right
    for (int r = rank - 1, f = file + 1; r >= 0 && f < 8; r--, f++)
    {
        int sq = (r * 8 + f);
        
        if (board.occupied_squares & (1ULL << sq))
        {
            if (board.black_occupied & (1ULL << sq) && player == WHITE_PLAYER) {
                moves |= (1ULL << sq);
            } else if (board.white_occupied & (1ULL << sq) && player == BLACK_PLAYER) {
                moves |= (1ULL << sq);
            }
            break;
        }
    }

    // Down-left
    for (int r = rank - 1, f = file - 1; r >= 0 && f >= 0; r--, f--)
    {
        int sq = (r * 8 + f);
        
        if (board.occupied_squares & (1ULL << sq))
        {
            if (board.black_occupied & (1ULL << sq) && player == WHITE_PLAYER) {
                moves |= (1ULL << sq);
            } else if (board.white_occupied & (1ULL << sq) && player == BLACK_PLAYER) {
                moves |= (1ULL << sq);
            }
            break;
        }
    }

    return moves;
}
Bitboard knight_moves(Players player, Board board, uint64_t square)
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
Bitboard queen_moves(Players player, Board board, uint64_t square)
{
    return rook_moves(player, board, square) | bishop_moves(player, board, square);
}
Bitboard king_moves(Players player, Board board, uint64_t square)
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

    if (board.black_occupied & moves && player == BLACK_PLAYER)
        moves &= ~board.black_occupied; // Remove moves that land on ally pieces
    else if (board.white_occupied & moves && player == WHITE_PLAYER)
        moves &= ~board.white_occupied;

    return moves;
}
