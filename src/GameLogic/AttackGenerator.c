#include "GameLogic/AttackGenerator.h"

static inline Bitboard shift(Bitboard bb, int shift);

// Precomputed file masks to prevent wrapping
const uint64_t AnotA  = 0xfefefefefefefefeULL; // ~file A
const uint64_t AnotAB = 0xfcfcfcfcfcfcfcfcULL; // ~files A,B
const uint64_t AnotH  = 0x7f7f7f7f7f7f7f7fULL; // ~file H
const uint64_t AnotGH = 0x3f3f3f3f3f3f3f3fULL; // ~files G,H

Bitboard pawns_attack(Players player, Board board)
{
    Bitboard pawns_squares = board.pieces[player][PAWN];
    Bitboard attacks = 0ULL;

    while (pawns_squares)
    {
        short square = __builtin_ctzll(pawns_squares);
        Bitboard bitboard = 1ULL << square;

        // Direction offsets
        bool goingUp = (player == WHITE_PLAYER);
        int leftCapShift  = goingUp ? +7 : -7;
        int rightCapShift = goingUp ? +9 : -9;

        // Captures
        Bitboard leftCapture  = shift(bitboard & AnotA, leftCapShift);
        Bitboard rightCapture = shift(bitboard & AnotH, rightCapShift);
        attacks |= leftCapture | rightCapture;

        pawns_squares &= pawns_squares - 1;
    }
    return attacks;
}
Bitboard rooks_attack(Players player, Board board)
{
    Bitboard rooks_squares = board.pieces[player][ROOK];
    Bitboard attacks = 0ULL;

    while (rooks_squares)
    {
        short square = __builtin_ctzll(rooks_squares);
        Bitboard bitboard = 1ULL << square;
        int rank = square / 8;
        int file = square % 8;

        // Up
        for (int r = rank + 1; r < 8; r++) {
            int sq = r * 8 + file;
            attacks |= (1ULL << sq); // attaked or defendent
            if (board.occupied_squares & (1ULL << sq))
                break; // stop at first occupied square
        }

        // Down
        for (int r = rank - 1; r >= 0; r--) {
            int sq = r * 8 + file;
            attacks |= (1ULL << sq);
            if (board.occupied_squares & (1ULL << sq))
                break;
        }

        // Right
        for (int f = file + 1; f < 8; f++) {
            int sq = rank * 8 + f;
            attacks |= (1ULL << sq);
            if (board.occupied_squares & (1ULL << sq))
                break;
        }

        // Left
        for (int f = file - 1; f >= 0; f--) {
            int sq = rank * 8 + f;
            attacks |= (1ULL << sq);
            if (board.occupied_squares & (1ULL << sq))
                break;
        }

        rooks_squares &= rooks_squares - 1;
    }

    return attacks;
}
Bitboard bishops_attack(Players player, Board board)
{
    Bitboard bishops_squares = board.pieces[player][BISHOP];
    Bitboard attacks = 0ULL;

    while (bishops_squares)
    {
        short square = __builtin_ctzll(bishops_squares);
        Bitboard bitboard = 1ULL << square;
        int rank = square / 8;
        int file = square % 8;
        
        // Up-right
        for (int r = rank + 1, f = file + 1; r < 8 && f < 8; r++, f++)
        {
            int sq = (r * 8 + f);
            attacks |= (1ULL << sq);
            if (board.occupied_squares & (1ULL << sq))
                break;
        }

        // Up-left
        for (int r = rank + 1, f = file - 1; r < 8 && f >= 0; r++, f--)
        {
            int sq = (r * 8 + f);
            attacks |= (1ULL << sq);
            if (board.occupied_squares & (1ULL << sq))
                break;
        }

        // Down-right
        for (int r = rank - 1, f = file + 1; r >= 0 && f < 8; r--, f++)
        {
            int sq = (r * 8 + f);
            attacks |= (1ULL << sq);
            if (board.occupied_squares & (1ULL << sq))
                break;
        }

        // Down-left
        for (int r = rank - 1, f = file - 1; r >= 0 && f >= 0; r--, f--)
        {
            int sq = (r * 8 + f);
            attacks |= (1ULL << sq);
            if (board.occupied_squares & (1ULL << sq))
                break;
        }

        bishops_squares &= bishops_squares - 1;
    }
    return attacks;
}
Bitboard knights_attack(Players player, Board board)
{
    Bitboard knights_squares = board.pieces[player][KNIGHT];
    Bitboard attacks = 0ULL;

    while (knights_squares)
    {
        short square = __builtin_ctzll(knights_squares);
        Bitboard bitboard = 1ULL << square;

        attacks |= (bitboard & AnotH)  << 17; // 2 up, 1 right
        attacks |= (bitboard & AnotA)  << 15; // 2 up, 1 left
        attacks |= (bitboard & AnotGH) << 10; // 1 up, 2 right
        attacks |= (bitboard & AnotAB) << 6;  // 1 up, 2 left
        attacks |= (bitboard & AnotH)  >> 15; // 2 down, 1 right
        attacks |= (bitboard & AnotA)  >> 17; // 2 down, 1 left
        attacks |= (bitboard & AnotGH) >> 6;  // 1 down, 2 right
        attacks |= (bitboard & AnotAB) >> 10; // 1 down, 2 left

        knights_squares &= knights_squares - 1;
    }
    return attacks;
}
Bitboard queens_attack(Players player, Board board)
{
    return rooks_attack(player, board) | bishops_attack(player, board);
}
Bitboard king_attack(Players player, Board board)
{
    short square = __builtin_ctzll(board.pieces[player][KING]);
    Bitboard bitboard = 1ULL << square;
    Bitboard attacks = 0ULL;

    attacks |= (bitboard & AnotA) << 7;   // up-left
    attacks |= bitboard << 8;            // up
    attacks |= (bitboard & AnotH) << 9;   // up-right
    attacks |= (bitboard & AnotA) >> 1;   // left
    attacks |= (bitboard & AnotH) << 1;   // right
    attacks |= (bitboard & AnotA) >> 9;   // down-left
    attacks |= bitboard >> 8;            // down
    attacks |= (bitboard & AnotH) >> 7;   // down-right

    return attacks;
}

// private
static inline Bitboard shift(Bitboard bb, int shift)
{
    return (shift > 0) ? (bb << shift) : (bb >> -shift);
}
