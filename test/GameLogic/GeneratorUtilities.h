#ifndef GENERATOR_UTILITIES_H
#define GENERATOR_UTILITIES_H

#include <stdint.h>
#include <gtest/gtest.h>

extern "C" {
#include "GameLogic/Board.h"
}

// Shared fields for all move test cases
struct MoveTestCase {
    short pieceSquare{};
    uint64_t blockers{};
    uint64_t captures{};
    uint64_t expectedMoves{};
};

// Pawn-specific case
struct PawnMoveTestCase : public MoveTestCase {
    short enPassantSquare = -1; // -1 means "no en passant"
};

// King-specific case
struct KingMoveTestCase : public MoveTestCase {
    uint8_t castling_rights = 0; // default: none
};

void setOccupied(Board *board, MoveTestCase param, Players player);
std::string bitboard_to_string(uint64_t bb);
uint64_t string_to_bitboard(const char* bb_string, const char to_search);
::testing::AssertionResult BitboardEq(uint64_t actual, uint64_t expected);

#endif