#include <gtest/gtest.h>

extern "C" {
#include "GameLogic/PieceMoveGenerator.h"
#include "GameLogic/Board.h"
#include <stdio.h>
}

#pragma region Test Setup
// -------- test functions ----------
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

::testing::AssertionResult BitboardEq(uint64_t actual, const std::string& expected) {
    std::string actualString = bitboard_to_string(actual);
    if (expected == actualString) {
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure()
           << "Expected:\n" << expected
           << "Got:\n" << actualString;
}

::testing::AssertionResult BitboardEq(uint64_t actual, uint64_t expected) {
    if (expected == actual) {
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure()
           << "Expected:\n" << bitboard_to_string(expected)
           << "Got:\n" << bitboard_to_string(actual);
}

// -------- for Test_F ---------
class PieceMoveGenerator : public ::testing::Test
{
public:
    // is available in all TESTs
    Board board;
    Board board2;
    Board board3;
    Board board4;
    Players playerW = WHITE_PLAYER;
    Players playerB = BLACK_PLAYER;
    
    void SetUp() override {
        init_board_empty(&board);
        init_board_empty(&board2);
        init_board_empty(&board3);
        init_board_empty(&board4);
    }
    
    void TearDown() override {

    }
};

// -------- for Test_P ---------
struct MoveTestCase {
    uint64_t pieceSquare;
    std::vector<uint64_t> blockers;
    std::vector<uint64_t> captures;
    uint64_t expectedMoves;
};
struct MoveTestCaseStr {
    uint64_t pieceSquare;
    std::vector<uint64_t> blockers;
    std::vector<uint64_t> captures;
    std::string expectedMoves;
};

class WPawnGenerator : public PieceMoveGenerator, 
                       public ::testing::WithParamInterface<MoveTestCase> {};

class BPawnGenerator : public PieceMoveGenerator, 
                       public ::testing::WithParamInterface<MoveTestCase> {};
                       
class RookGenerator : public PieceMoveGenerator, 
                       public ::testing::WithParamInterface<MoveTestCaseStr> {};
                       
class BishopGenerator : public PieceMoveGenerator, 
                       public ::testing::WithParamInterface<MoveTestCaseStr> {};
                       
class KnightGenerator : public PieceMoveGenerator, 
                       public ::testing::WithParamInterface<MoveTestCaseStr> {};
                       
class QueenGenerator : public PieceMoveGenerator, 
                       public ::testing::WithParamInterface<MoveTestCaseStr> {};
                       
class KingGenerator : public PieceMoveGenerator, 
                       public ::testing::WithParamInterface<MoveTestCaseStr> {};
#pragma endregion

TEST_P(WPawnGenerator, wpawn_moves) {
    // arrange
    const auto& param = GetParam();
    Bitboard moves = 0;
    place_piece(&(board.white_pawns), param.pieceSquare);
    for (auto b : param.blockers) {
        place_piece(&(board.white_pawns), b);
    }
    for (auto b : param.captures) {
        place_piece(&(board.black_pawns), b);
    }
    update_board(&board);

    // act
    moves = wpawn_moves(board, param.pieceSquare);
    
    // expect
    EXPECT_TRUE(BitboardEq(moves, param.expectedMoves));
}

INSTANTIATE_TEST_SUITE_P(DefaultCases, WPawnGenerator, ::testing::Values(
    // {pawnSquare, blockers, captures, expectedMoves}
    MoveTestCase{E2, {}, {}, (1ULL << E3) | (1ULL << E4)}, 
    MoveTestCase{E2, {E4}, {}, (1ULL << E3)},
    MoveTestCase{E2, {E3}, {}, 0},
    MoveTestCase{E2, {}, {D3, F3}, (1ULL << E3) | (1ULL << E4) | (1ULL << D3) | (1ULL << F3)},
    MoveTestCase{E2, {E3}, {D3, F3}, (1ULL << D3) | (1ULL << F3)},
    MoveTestCase{E2, {E4}, {D3, F3}, (1ULL << E3) | (1ULL << D3) | (1ULL << F3)},
    MoveTestCase{E2, {}, {D3}, (1ULL << E3) | (1ULL << E4) | (1ULL << D3)},
    MoveTestCase{E2, {}, {F3}, (1ULL << E3) | (1ULL << E4) | (1ULL << F3)},
    MoveTestCase{E2, {F3}, {D3}, (1ULL << E3) | (1ULL << E4) | (1ULL << D3)},
    MoveTestCase{E2, {D3}, {F3}, (1ULL << E3) | (1ULL << E4) | (1ULL << F3)},
    MoveTestCase{E3, {}, {}, (1ULL << E4)},
    MoveTestCase{E3, {E4}, {}, 0},
    MoveTestCase{E8, {}, {}, 0}
));

TEST_P(BPawnGenerator, bpawn_moves) {
    // arrange
    const auto& param = GetParam();
    Bitboard moves = 0;
    place_piece(&(board.black_pawns), param.pieceSquare);
    for (auto b : param.blockers) {
        place_piece(&(board.black_pawns), b);
    }
    for (auto b : param.captures) {
        place_piece(&(board.white_pawns), b);
    }
    update_board(&board);

    // act
    moves = bpawn_moves(board, param.pieceSquare);
    
    // expect
    EXPECT_TRUE(BitboardEq(moves, param.expectedMoves));
}

INSTANTIATE_TEST_SUITE_P(DefaultCases, BPawnGenerator, ::testing::Values(
    // {pawnSquare, blockers, captures, expectedMoves}
    MoveTestCase{E7, {}, {}, (1ULL << E6) | (1ULL << E5)}, 
    MoveTestCase{E7, {E5}, {}, (1ULL << E6)},
    MoveTestCase{E7, {E6}, {}, 0},
    MoveTestCase{E7, {}, {D6, F6}, (1ULL << E6) | (1ULL << E5) | (1ULL << D6) | (1ULL << F6)},
    MoveTestCase{E7, {E5}, {D6, F6}, (1ULL << E6) | (1ULL << D6) | (1ULL << F6)},
    MoveTestCase{E7, {E6}, {D6, F6}, (1ULL << D6) | (1ULL << F6)},
    MoveTestCase{E7, {}, {D6}, (1ULL << E6) | (1ULL << E5) | (1ULL << D6)},
    MoveTestCase{E7, {}, {F6}, (1ULL << E6) | (1ULL << E5) | (1ULL << F6)},
    MoveTestCase{E7, {F6}, {D6}, (1ULL << E6) | (1ULL << E5) | (1ULL << D6)},
    MoveTestCase{E7, {D6}, {F6}, (1ULL << E6) | (1ULL << E5) | (1ULL << F6)},
    MoveTestCase{E6, {}, {}, (1ULL << E5)},
    MoveTestCase{E6, {E5}, {}, 0},
    MoveTestCase{E1, {}, {}, 0}
));

TEST_P(RookGenerator, rook_moves) {
    // arrange
    const auto& param = GetParam();
    Bitboard movesW = 0;
    Bitboard movesB = 0;
    place_piece(&(board.white_rooks), param.pieceSquare);
    place_piece(&(board2.black_rooks), param.pieceSquare);
    for (auto b : param.blockers) {
        place_piece(&(board.white_pawns), b);
        place_piece(&(board2.black_pawns), b);
    }
    for (auto b : param.captures) {
        place_piece(&(board.black_pawns), b);
        place_piece(&(board2.white_pawns), b);
    }
    update_board(&board);
    update_board(&board2);

    // act
    movesW = rook_moves(playerW, board, param.pieceSquare);
    movesB = rook_moves(playerB, board2, param.pieceSquare);
    
    // expect
    EXPECT_TRUE(BitboardEq(movesW, param.expectedMoves));
    EXPECT_TRUE(BitboardEq(movesB, param.expectedMoves));
}

INSTANTIATE_TEST_SUITE_P(DefaultCases, RookGenerator, ::testing::Values(
    // {pawnSquare, blockers, captures, expectedMoves}
    MoveTestCaseStr{A1, {}, {}, 
                                "1 . . . . . . . \n"
                                "1 . . . . . . . \n"
                                "1 . . . . . . . \n"
                                "1 . . . . . . . \n"
                                "1 . . . . . . . \n"
                                "1 . . . . . . . \n"
                                "1 . . . . . . . \n"
                                ". 1 1 1 1 1 1 1 \n"},
    MoveTestCaseStr{H1, {}, {}, 
                                ". . . . . . . 1 \n"
                                ". . . . . . . 1 \n"
                                ". . . . . . . 1 \n"
                                ". . . . . . . 1 \n"
                                ". . . . . . . 1 \n"
                                ". . . . . . . 1 \n"
                                ". . . . . . . 1 \n"
                                "1 1 1 1 1 1 1 . \n"},
    MoveTestCaseStr{A8, {}, {}, 
                                ". 1 1 1 1 1 1 1 \n"
                                "1 . . . . . . . \n"
                                "1 . . . . . . . \n"
                                "1 . . . . . . . \n"
                                "1 . . . . . . . \n"
                                "1 . . . . . . . \n"
                                "1 . . . . . . . \n"
                                "1 . . . . . . . \n"},
    MoveTestCaseStr{H8, {}, {}, 
                                "1 1 1 1 1 1 1 . \n"
                                ". . . . . . . 1 \n"
                                ". . . . . . . 1 \n"
                                ". . . . . . . 1 \n"
                                ". . . . . . . 1 \n"
                                ". . . . . . . 1 \n"
                                ". . . . . . . 1 \n"
                                ". . . . . . . 1 \n"},
    MoveTestCaseStr{A5, {}, {}, 
                                "1 . . . . . . . \n"
                                "1 . . . . . . . \n"
                                "1 . . . . . . . \n"
                                ". 1 1 1 1 1 1 1 \n"
                                "1 . . . . . . . \n"
                                "1 . . . . . . . \n"
                                "1 . . . . . . . \n"
                                "1 . . . . . . . \n"},
    MoveTestCaseStr{E8, {}, {}, 
                                "1 1 1 1 . 1 1 1 \n"
                                ". . . . 1 . . . \n"
                                ". . . . 1 . . . \n"
                                ". . . . 1 . . . \n"
                                ". . . . 1 . . . \n"
                                ". . . . 1 . . . \n"
                                ". . . . 1 . . . \n"
                                ". . . . 1 . . . \n"},
    MoveTestCaseStr{H4, {}, {}, 
                                ". . . . . . . 1 \n"
                                ". . . . . . . 1 \n"
                                ". . . . . . . 1 \n"
                                ". . . . . . . 1 \n"
                                "1 1 1 1 1 1 1 . \n"
                                ". . . . . . . 1 \n"
                                ". . . . . . . 1 \n"
                                ". . . . . . . 1 \n"},
    MoveTestCaseStr{D1, {}, {}, 
                                ". . . 1 . . . . \n"
                                ". . . 1 . . . . \n"
                                ". . . 1 . . . . \n"
                                ". . . 1 . . . . \n"
                                ". . . 1 . . . . \n"
                                ". . . 1 . . . . \n"
                                ". . . 1 . . . . \n"
                                "1 1 1 . 1 1 1 1 \n"},
    MoveTestCaseStr{D4, {}, {}, 
                                ". . . 1 . . . . \n"
                                ". . . 1 . . . . \n"
                                ". . . 1 . . . . \n"
                                ". . . 1 . . . . \n"
                                "1 1 1 . 1 1 1 1 \n"
                                ". . . 1 . . . . \n"
                                ". . . 1 . . . . \n"
                                ". . . 1 . . . . \n"},
    MoveTestCaseStr{D4, {}, {D2}, 
                                ". . . 1 . . . . \n"
                                ". . . 1 . . . . \n"
                                ". . . 1 . . . . \n"
                                ". . . 1 . . . . \n"
                                "1 1 1 . 1 1 1 1 \n"
                                ". . . 1 . . . . \n"
                                ". . . 1 . . . . \n"
                                ". . . . . . . . \n"},
    MoveTestCaseStr{D4, {}, {D2, D7}, 
                                ". . . . . . . . \n"
                                ". . . 1 . . . . \n"
                                ". . . 1 . . . . \n"
                                ". . . 1 . . . . \n"
                                "1 1 1 . 1 1 1 1 \n"
                                ". . . 1 . . . . \n"
                                ". . . 1 . . . . \n"
                                ". . . . . . . . \n"},
    MoveTestCaseStr{D4, {}, {D2, D7, B4}, 
                                ". . . . . . . . \n"
                                ". . . 1 . . . . \n"
                                ". . . 1 . . . . \n"
                                ". . . 1 . . . . \n"
                                ". 1 1 . 1 1 1 1 \n"
                                ". . . 1 . . . . \n"
                                ". . . 1 . . . . \n"
                                ". . . . . . . . \n"},
    MoveTestCaseStr{D4, {}, {D2, D7, B4, G4}, 
                                ". . . . . . . . \n"
                                ". . . 1 . . . . \n"
                                ". . . 1 . . . . \n"
                                ". . . 1 . . . . \n"
                                ". 1 1 . 1 1 1 . \n"
                                ". . . 1 . . . . \n"
                                ". . . 1 . . . . \n"
                                ". . . . . . . . \n"},
    MoveTestCaseStr{D4, {}, {D3, D5, C4, E4}, 
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . 1 . . . . \n"
                                ". . 1 . 1 . . . \n"
                                ". . . 1 . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"},
    MoveTestCaseStr{E5, {E2, E7, B5, G5}, {}, 
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . 1 . . . \n"
                                ". . 1 1 . 1 . . \n"
                                ". . . . 1 . . . \n"
                                ". . . . 1 . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"},
    MoveTestCaseStr{E5, {E4, E6, D5, F5}, {}, 
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"},
    MoveTestCaseStr{E5, {E2}, {E7}, 
                                ". . . . . . . . \n"
                                ". . . . 1 . . . \n"
                                ". . . . 1 . . . \n"
                                "1 1 1 1 . 1 1 1 \n"
                                ". . . . 1 . . . \n"
                                ". . . . 1 . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"}
));

TEST_P(BishopGenerator, bishop_moves) {
    // arrange
    const auto& param = GetParam();
    init_board_empty(&board);
    init_board_empty(&board2);
    Bitboard movesW = 0;
    Bitboard movesB = 0;
    place_piece(&(board.white_bishops), param.pieceSquare);
    place_piece(&(board2.black_bishops), param.pieceSquare);
    for (auto b : param.blockers) {
        place_piece(&(board.white_pawns), b);
        place_piece(&(board2.black_pawns), b);
    }
    for (auto b : param.captures) {
        place_piece(&(board.black_pawns), b);
        place_piece(&(board2.white_pawns), b);
    }
    update_board(&board);
    update_board(&board2);

    // act
    movesW = bishop_moves(playerW, board, param.pieceSquare);
    movesB = bishop_moves(playerB, board2, param.pieceSquare);
    
    // expect
    EXPECT_TRUE(BitboardEq(movesW, param.expectedMoves));
    EXPECT_TRUE(BitboardEq(movesB, param.expectedMoves));
}

INSTANTIATE_TEST_SUITE_P(DefaultCases, BishopGenerator, ::testing::Values(
    // {pawnSquare, blockers, captures, expectedMoves}
    MoveTestCaseStr{A1, {}, {},
                                ". . . . . . . 1 \n"
                                ". . . . . . 1 . \n"
                                ". . . . . 1 . . \n"
                                ". . . . 1 . . . \n"
                                ". . . 1 . . . . \n"
                                ". . 1 . . . . . \n"
                                ". 1 . . . . . . \n"
                                ". . . . . . . . \n"},
    MoveTestCaseStr{H1, {}, {},
                                "1 . . . . . . . \n"
                                ". 1 . . . . . . \n"
                                ". . 1 . . . . . \n"
                                ". . . 1 . . . . \n"
                                ". . . . 1 . . . \n"
                                ". . . . . 1 . . \n"
                                ". . . . . . 1 . \n"
                                ". . . . . . . . \n"},
    MoveTestCaseStr{A8, {}, {},
                                ". . . . . . . . \n"
                                ". 1 . . . . . . \n"
                                ". . 1 . . . . . \n"
                                ". . . 1 . . . . \n"
                                ". . . . 1 . . . \n"
                                ". . . . . 1 . . \n"
                                ". . . . . . 1 . \n"
                                ". . . . . . . 1 \n"},
    MoveTestCaseStr{H8, {}, {},
                                ". . . . . . . . \n"
                                ". . . . . . 1 . \n"
                                ". . . . . 1 . . \n"
                                ". . . . 1 . . . \n"
                                ". . . 1 . . . . \n"
                                ". . 1 . . . . . \n"
                                ". 1 . . . . . . \n"
                                "1 . . . . . . . \n"},
    MoveTestCaseStr{A4, {}, {},
                                ". . . . 1 . . . \n"
                                ". . . 1 . . . . \n"
                                ". . 1 . . . . . \n"
                                ". 1 . . . . . . \n"
                                ". . . . . . . . \n"
                                ". 1 . . . . . . \n"
                                ". . 1 . . . . . \n"
                                ". . . 1 . . . . \n"},
    MoveTestCaseStr{D8, {}, {},
                                ". . . . . . . . \n"
                                ". . 1 . 1 . . . \n"
                                ". 1 . . . 1 . . \n"
                                "1 . . . . . 1 . \n"
                                ". . . . . . . 1 \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"},
    MoveTestCaseStr{H4, {}, {},
                                ". . . 1 . . . . \n"
                                ". . . . 1 . . . \n"
                                ". . . . . 1 . . \n"
                                ". . . . . . 1 . \n"
                                ". . . . . . . . \n"
                                ". . . . . . 1 . \n"
                                ". . . . . 1 . . \n"
                                ". . . . 1 . . . \n"},
    MoveTestCaseStr{D1, {}, {},
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . 1 \n"
                                "1 . . . . . 1 . \n"
                                ". 1 . . . 1 . . \n"
                                ". . 1 . 1 . . . \n"
                                ". . . . . . . . \n"},
    MoveTestCaseStr{D4, {}, {},
                                ". . . . . . . 1 \n"
                                "1 . . . . . 1 . \n"
                                ". 1 . . . 1 . . \n"
                                ". . 1 . 1 . . . \n"
                                ". . . . . . . . \n"
                                ". . 1 . 1 . . . \n"
                                ". 1 . . . 1 . . \n"
                                "1 . . . . . 1 . \n"},
    MoveTestCaseStr{D4, {}, {B2, F2, B6, G7},
                                ". . . . . . . . \n"
                                ". . . . . . 1 . \n"
                                ". 1 . . . 1 . . \n"
                                ". . 1 . 1 . . . \n"
                                ". . . . . . . . \n"
                                ". . 1 . 1 . . . \n"
                                ". 1 . . . 1 . . \n"
                                ". . . . . . . . \n"},
    MoveTestCaseStr{D4, {}, {C3, C5, E3, E5},
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . 1 . 1 . . . \n"
                                ". . . . . . . . \n"
                                ". . 1 . 1 . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"},
    MoveTestCaseStr{D4, {B2, F2, B6, G7}, {},
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . 1 . . \n"
                                ". . 1 . 1 . . . \n"
                                ". . . . . . . . \n"
                                ". . 1 . 1 . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"},
    MoveTestCaseStr{D4, {C3, C5, E3, E5}, {},
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"},
    MoveTestCaseStr{D4, {B2}, {F6},
                                ". . . . . . . . \n"
                                "1 . . . . . . . \n"
                                ". 1 . . . 1 . . \n"
                                ". . 1 . 1 . . . \n"
                                ". . . . . . . . \n"
                                ". . 1 . 1 . . . \n"
                                ". . . . . 1 . . \n"
                                ". . . . . . 1 . \n"}
));

TEST_P(KnightGenerator, knight_moves) {
    // arrange
    const auto& param = GetParam();
    init_board_empty(&board);
    init_board_empty(&board2);
    Bitboard movesW = 0;
    Bitboard movesB = 0;
    place_piece(&(board.white_knights), param.pieceSquare);
    place_piece(&(board2.black_knights), param.pieceSquare);
    for (auto b : param.blockers) {
        place_piece(&(board.white_pawns), b);
        place_piece(&(board2.black_pawns), b);
    }
    for (auto b : param.captures) {
        place_piece(&(board.black_pawns), b);
        place_piece(&(board2.white_pawns), b);
    }
    update_board(&board);
    update_board(&board2);

    // act
    movesW = knight_moves(playerW, board, param.pieceSquare);
    movesB = knight_moves(playerB, board2, param.pieceSquare);
    
    // expect
    EXPECT_TRUE(BitboardEq(movesW, param.expectedMoves));
    EXPECT_TRUE(BitboardEq(movesB, param.expectedMoves));
}

INSTANTIATE_TEST_SUITE_P(DefaultCases, KnightGenerator, ::testing::Values(
    // {pawnSquare, blockers, captures, expectedMoves}
    MoveTestCaseStr{A1, {}, {},
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". 1 . . . . . . \n"
                                ". . 1 . . . . . \n"
                                ". . . . . . . . \n"},
    MoveTestCaseStr{H1, {}, {},
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . 1 . \n"
                                ". . . . . 1 . . \n"
                                ". . . . . . . . \n"},
    MoveTestCaseStr{A8, {}, {},
                                ". . . . . . . . \n"
                                ". . 1 . . . . . \n"
                                ". 1 . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"},
    MoveTestCaseStr{H8, {}, {},
                                ". . . . . . . . \n"
                                ". . . . . 1 . . \n"
                                ". . . . . . 1 . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"},
    MoveTestCaseStr{A4, {}, {},
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". 1 . . . . . . \n"
                                ". . 1 . . . . . \n"
                                ". . . . . . . . \n"
                                ". . 1 . . . . . \n"
                                ". 1 . . . . . . \n"
                                ". . . . . . . . \n"},
    MoveTestCaseStr{D8, {}, {},
                                ". . . . . . . . \n"
                                ". 1 . . . 1 . . \n"
                                ". . 1 . 1 . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"},
    MoveTestCaseStr{H4, {}, {},
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . 1 . \n"
                                ". . . . . 1 . . \n"
                                ". . . . . . . . \n"
                                ". . . . . 1 . . \n"
                                ". . . . . . 1 . \n"
                                ". . . . . . . . \n"},
    MoveTestCaseStr{D1, {}, {},
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . 1 . 1 . . . \n"
                                ". 1 . . . 1 . . \n"
                                ". . . . . . . . \n"},
    MoveTestCaseStr{D4, {}, {},
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . 1 . 1 . . . \n"
                                ". 1 . . . 1 . . \n"
                                ". . . . . . . . \n"
                                ". 1 . . . 1 . . \n"
                                ". . 1 . 1 . . . \n"
                                ". . . . . . . . \n"},
    MoveTestCaseStr{D4, {}, {B3, B5, C2, C6, E2, E6, F3, F5},
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . 1 . 1 . . . \n"
                                ". 1 . . . 1 . . \n"
                                ". . . . . . . . \n"
                                ". 1 . . . 1 . . \n"
                                ". . 1 . 1 . . . \n"
                                ". . . . . . . . \n"},
    MoveTestCaseStr{D4, {B3, B5, C2, C6, E2, E6, F3, F5}, {},
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"},
    MoveTestCaseStr{D4, {B3, B5}, {C2, C6},
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . 1 . 1 . . . \n"
                                ". . . . . 1 . . \n"
                                ". . . . . . . . \n"
                                ". . . . . 1 . . \n"
                                ". . 1 . 1 . . . \n"
                                ". . . . . . . . \n"}
));

TEST_P(QueenGenerator, queen_moves) {
    // arrange
    const auto& param = GetParam();
    init_board_empty(&board);
    init_board_empty(&board2);
    Bitboard movesW = 0;
    Bitboard movesB = 0;
    place_piece(&(board.white_queen), param.pieceSquare);
    place_piece(&(board2.black_queen), param.pieceSquare);
    for (auto b : param.blockers) {
        place_piece(&(board.white_pawns), b);
        place_piece(&(board2.black_pawns), b);
    }
    for (auto b : param.captures) {
        place_piece(&(board.black_pawns), b);
        place_piece(&(board2.white_pawns), b);
    }
    update_board(&board);
    update_board(&board2);

    // act
    movesW = queen_moves(playerW, board, param.pieceSquare);
    movesB = queen_moves(playerB, board2, param.pieceSquare);
    
    // expect
    EXPECT_TRUE(BitboardEq(movesW, param.expectedMoves));
    EXPECT_TRUE(BitboardEq(movesB, param.expectedMoves));
}

INSTANTIATE_TEST_SUITE_P(DefaultCases, QueenGenerator, ::testing::Values(
    // {pawnSquare, blockers, captures, expectedMoves}
    MoveTestCaseStr{A1, {}, {},
                                "1 . . . . . . 1 \n"
                                "1 . . . . . 1 . \n"
                                "1 . . . . 1 . . \n"
                                "1 . . . 1 . . . \n"
                                "1 . . 1 . . . . \n"
                                "1 . 1 . . . . . \n"
                                "1 1 . . . . . . \n"
                                ". 1 1 1 1 1 1 1 \n"},
    MoveTestCaseStr{H1, {}, {},
                                "1 . . . . . . 1 \n"
                                ". 1 . . . . . 1 \n"
                                ". . 1 . . . . 1 \n"
                                ". . . 1 . . . 1 \n"
                                ". . . . 1 . . 1 \n"
                                ". . . . . 1 . 1 \n"
                                ". . . . . . 1 1 \n"
                                "1 1 1 1 1 1 1 . \n"},
    MoveTestCaseStr{A8, {}, {},
                                ". 1 1 1 1 1 1 1 \n"
                                "1 1 . . . . . . \n"
                                "1 . 1 . . . . . \n"
                                "1 . . 1 . . . . \n"
                                "1 . . . 1 . . . \n"
                                "1 . . . . 1 . . \n"
                                "1 . . . . . 1 . \n"
                                "1 . . . . . . 1 \n"},
    MoveTestCaseStr{H8, {}, {},
                                "1 1 1 1 1 1 1 . \n"
                                ". . . . . . 1 1 \n"
                                ". . . . . 1 . 1 \n"
                                ". . . . 1 . . 1 \n"
                                ". . . 1 . . . 1 \n"
                                ". . 1 . . . . 1 \n"
                                ". 1 . . . . . 1 \n"
                                "1 . . . . . . 1 \n"},
    MoveTestCaseStr{A4, {}, {},
                                "1 . . . 1 . . . \n"
                                "1 . . 1 . . . . \n"
                                "1 . 1 . . . . . \n"
                                "1 1 . . . . . . \n"
                                ". 1 1 1 1 1 1 1 \n"
                                "1 1 . . . . . . \n"
                                "1 . 1 . . . . . \n"
                                "1 . . 1 . . . . \n"},
    MoveTestCaseStr{D8, {}, {},
                                "1 1 1 . 1 1 1 1 \n"
                                ". . 1 1 1 . . . \n"
                                ". 1 . 1 . 1 . . \n"
                                "1 . . 1 . . 1 . \n"
                                ". . . 1 . . . 1 \n"
                                ". . . 1 . . . . \n"
                                ". . . 1 . . . . \n"
                                ". . . 1 . . . . \n"},
    MoveTestCaseStr{H4, {}, {},
                                ". . . 1 . . . 1 \n"
                                ". . . . 1 . . 1 \n"
                                ". . . . . 1 . 1 \n"
                                ". . . . . . 1 1 \n"
                                "1 1 1 1 1 1 1 . \n"
                                ". . . . . . 1 1 \n"
                                ". . . . . 1 . 1 \n"
                                ". . . . 1 . . 1 \n"},
    MoveTestCaseStr{D1, {}, {},
                                ". . . 1 . . . . \n"
                                ". . . 1 . . . . \n"
                                ". . . 1 . . . . \n"
                                ". . . 1 . . . 1 \n"
                                "1 . . 1 . . 1 . \n"
                                ". 1 . 1 . 1 . . \n"
                                ". . 1 1 1 . . . \n"
                                "1 1 1 . 1 1 1 1 \n"},
    MoveTestCaseStr{D4, {}, {},
                                ". . . 1 . . . 1 \n"
                                "1 . . 1 . . 1 . \n"
                                ". 1 . 1 . 1 . . \n"
                                ". . 1 1 1 . . . \n"
                                "1 1 1 . 1 1 1 1 \n"
                                ". . 1 1 1 . . . \n"
                                ". 1 . 1 . 1 . . \n"
                                "1 . . 1 . . 1 . \n"},
    MoveTestCaseStr{D4, {}, {B2, F2, B6, G7, D7, D2, G4, B4},
                                ". . . . . . . . \n"
                                ". . . 1 . . 1 . \n"
                                ". 1 . 1 . 1 . . \n"
                                ". . 1 1 1 . . . \n"
                                ". 1 1 . 1 1 1 . \n"
                                ". . 1 1 1 . . . \n"
                                ". 1 . 1 . 1 . . \n"
                                ". . . . . . . . \n"},
    MoveTestCaseStr{D4, {}, {C3, C5, E3, E5, D3, C4, E4, D5},
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . 1 1 1 . . . \n"
                                ". . 1 . 1 . . . \n"
                                ". . 1 1 1 . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"},
    MoveTestCaseStr{D4, {B2, F2, B6, G7, D7, D2, G4, B4}, {},
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . 1 . 1 . . \n"
                                ". . 1 1 1 . . . \n"
                                ". . 1 . 1 1 . . \n"
                                ". . 1 1 1 . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"},
    MoveTestCaseStr{D4, {C3, C5, E3, E5, D3, C4, E4, D5}, {},
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"},
    MoveTestCaseStr{D4, {B2, G4}, {F6, D2},
                                ". . . 1 . . . . \n"
                                "1 . . 1 . . . . \n"
                                ". 1 . 1 . 1 . . \n"
                                ". . 1 1 1 . . . \n"
                                "1 1 1 . 1 1 . . \n"
                                ". . 1 1 1 . . . \n"
                                ". . . 1 . 1 . . \n"
                                ". . . . . . 1 . \n"}
));

TEST_P(KingGenerator, king_moves) {
    // arrange
    const auto& param = GetParam();
    init_board_empty(&board);
    init_board_empty(&board2);
    Bitboard movesW = 0;
    Bitboard movesB = 0;
    place_piece(&(board.white_king), param.pieceSquare);
    place_piece(&(board2.black_king), param.pieceSquare);
    for (auto b : param.blockers) {
        place_piece(&(board.white_pawns), b);
        place_piece(&(board2.black_pawns), b);
    }
    for (auto b : param.captures) {
        place_piece(&(board.black_pawns), b);
        place_piece(&(board2.white_pawns), b);
    }
    update_board(&board);
    update_board(&board2);

    // act
    movesW = king_moves(playerW, board, param.pieceSquare);
    movesB = king_moves(playerB, board2, param.pieceSquare);
    
    // expect
    EXPECT_TRUE(BitboardEq(movesW, param.expectedMoves));
    EXPECT_TRUE(BitboardEq(movesB, param.expectedMoves));
}

INSTANTIATE_TEST_SUITE_P(DefaultCases, KingGenerator, ::testing::Values(
    // {pawnSquare, blockers, captures, expectedMoves}
    MoveTestCaseStr{A1, {}, {},
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                "1 1 . . . . . . \n"
                                ". 1 . . . . . . \n"},
    MoveTestCaseStr{H1, {}, {},
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . 1 1 \n"
                                ". . . . . . 1 . \n"},
    MoveTestCaseStr{A8, {}, {},
                                ". 1 . . . . . . \n"
                                "1 1 . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"},
    MoveTestCaseStr{H8, {}, {},
                                ". . . . . . 1 . \n"
                                ". . . . . . 1 1 \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"},
    MoveTestCaseStr{A4, {}, {},
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                "1 1 . . . . . . \n"
                                ". 1 . . . . . . \n"
                                "1 1 . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"},
    MoveTestCaseStr{D8, {}, {},
                                ". . 1 . 1 . . . \n"
                                ". . 1 1 1 . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"},
    MoveTestCaseStr{H4, {}, {},
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . 1 1 \n"
                                ". . . . . . 1 . \n"
                                ". . . . . . 1 1 \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"},
    MoveTestCaseStr{D1, {}, {},
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . 1 1 1 . . . \n"
                                ". . 1 . 1 . . . \n"},
    MoveTestCaseStr{D4, {}, {},
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . 1 1 1 . . . \n"
                                ". . 1 . 1 . . . \n"
                                ". . 1 1 1 . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"},
    MoveTestCaseStr{D4, {}, {C3, D3, E3, C4, E4, C5, D5, E5},
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . 1 1 1 . . . \n"
                                ". . 1 . 1 . . . \n"
                                ". . 1 1 1 . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"},
    MoveTestCaseStr{D4, {C3, D3, E3, C4, E4, C5, D5, E5}, {},
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"},
    MoveTestCaseStr{D4, {D5, E5}, {C3, D3},
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"
                                ". . 1 . . . . . \n"
                                ". . 1 . 1 . . . \n"
                                ". . 1 1 1 . . . \n"
                                ". . . . . . . . \n"
                                ". . . . . . . . \n"}
));