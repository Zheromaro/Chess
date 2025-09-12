#include <gtest/gtest.h>
#include "GeneratorUtilities.h"

extern "C" {
#include "GameLogic/MoveGenerator.h"
#include "GameLogic/Board.h"
#include "jsonReading.h"
#include <stdio.h>
}

void setOccupied(Board *board, MoveTestCase param)
{
    board->white_occupied = param.blockers;
    board->black_occupied = param.captures;
    board->occupied_squares = param.blockers | param.captures;
}

std::string bitboard_to_string(uint64_t bb) {
    std::ostringstream oss;
    for (int rank = 7; rank >= 0; --rank) {
        for (int file = 0; file < 8; ++file) {
            int sq = rank * 8 + file;
            oss << ((bb >> sq) & 1ULL ? "1 " : ". ");
        }
        oss << "\n";
    }
    return oss.str();
}

Bitboard string_to_bitboard(const char* bb_string, const char to_search) {
    Bitboard result = 0;

    int idx = 0;
    for (int rank = 7; rank >= 0; rank--) {
        for (int file = 0; file < 8; file++) {
            char c = bb_string[idx];
            while (c == ' ' || c == '\n') { // skip spaces and newlines
                c = bb_string[++idx];
            }

            if (c == to_search) {
                int square = rank * 8 + file;
                result |= (1ULL << square);
            }
            idx++;
        }
    }

    return result;
}

::testing::AssertionResult BitboardEq(uint64_t actual, uint64_t expected) {
    if (expected == actual) {
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure()
           << "Expected:\n" << bitboard_to_string(expected)
           << "Got:\n" << bitboard_to_string(actual);
}
