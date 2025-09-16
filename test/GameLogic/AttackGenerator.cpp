#include <gtest/gtest.h>
#include "GeneratorUtilities.h"

extern "C" {
#include "GameLogic/AttackGenerator.h"
#include "GameLogic/Board.h"
#include "jsonReading.h"
#include <stdio.h>
}

#pragma region Test Setup
// -------- for Test_F ---------
static cJSON* json = get_JSON_data_in((char*)"/home/zhero/projects/Chess/test/GameLogic/Generators_test.json");

class AttackGenerator : public ::testing::Test
{
public:
    // is available in all TESTs

    Board board;
    Board board2;
    Board board3;
    Board board4;
    Players playerW = WHITE_PLAYER;
    Players playerB = BLACK_PLAYER;
    Bitboard moves = 0;
    Bitboard movesW = 0;
    Bitboard movesB = 0;
    
    void SetUp() override {
        init_board_empty(&board);
        init_board_empty(&board2);
        init_board_empty(&board3);
        init_board_empty(&board4);
        movesW = 0;
        movesB = 0;
    }
    
    void TearDown() override {
    }
};

// -------- for Test_P ---------

class WPawn_AttackGenerator : public AttackGenerator, 
                       public ::testing::WithParamInterface<MoveTestCase> {};

class BPawn_AttackGenerator : public AttackGenerator, 
                       public ::testing::WithParamInterface<MoveTestCase> {};
                       
class Rook_AttackGenerator : public AttackGenerator, 
                       public ::testing::WithParamInterface<MoveTestCase> {};
                       
class Bishop_AttackGenerator : public AttackGenerator, 
                       public ::testing::WithParamInterface<MoveTestCase> {};
                       
class Knight_AttackGenerator : public AttackGenerator, 
                       public ::testing::WithParamInterface<MoveTestCase> {};
                       
class Queen_AttackGenerator : public AttackGenerator, 
                       public ::testing::WithParamInterface<MoveTestCase> {};
                       
class King_AttackGenerator : public AttackGenerator, 
                       public ::testing::WithParamInterface<MoveTestCase> {};


std::vector<MoveTestCase> load_attack_testcases(cJSON* json, const char* object_name)
{
    int count = 0;
    char** boards = get_expected_boards(json, object_name, &count);
    std::vector<MoveTestCase> cases;
    cases.reserve(count);

    for (int i = 0; i < count; i++) {
        short pieceSquare = __builtin_ctzll(string_to_bitboard(boards[i], 'P'));
        uint64_t blockers = string_to_bitboard(boards[i], 'B');
        uint64_t captures = string_to_bitboard(boards[i], 'C');
        uint64_t expected = string_to_bitboard(boards[i], '1');

        MoveTestCase tc{};
        tc.pieceSquare = pieceSquare;
        tc.blockers = blockers;
        tc.captures = captures;
        tc.expectedMoves = expected | captures | blockers;

        cases.push_back(tc);
    }

    for (int i = 0; i < count; i++) free(boards[i]);
    free(boards);

    return cases;
}

#pragma endregion

TEST_P(WPawn_AttackGenerator, wpawns_attack) {
    // arrange
    const auto& param = GetParam();

    place_piece(&(board.pieces[WHITE_PLAYER][PAWN]), param.pieceSquare);
    setOccupied(&board, param, playerW);

    // act
    moves = pawns_attack(playerW, board);

    // expect
    EXPECT_TRUE(BitboardEq(moves, param.expectedMoves));
}

INSTANTIATE_TEST_SUITE_P(AttackCases, WPawn_AttackGenerator,
    ::testing::ValuesIn(load_attack_testcases(json, "wpawn_attack_tests")));

TEST_P(BPawn_AttackGenerator, wpawns_attack) {
    // arrange
    const auto& param = GetParam();

    place_piece(&(board.pieces[BLACK_PLAYER][PAWN]), param.pieceSquare);
    setOccupied(&board, param, playerB);

    // act
    moves = pawns_attack(playerB, board);

    // expect
    EXPECT_TRUE(BitboardEq(moves, param.expectedMoves));
}

INSTANTIATE_TEST_SUITE_P(AttackCases, BPawn_AttackGenerator,
    ::testing::ValuesIn(load_attack_testcases(json, "bpawn_attack_tests")));


TEST_P(Rook_AttackGenerator, rooks_attack) {
    // arrange
    const auto& param = GetParam();
    
    place_piece(&(board.pieces[WHITE_PLAYER][ROOK]), param.pieceSquare);
    place_piece(&(board2.pieces[BLACK_PLAYER][ROOK]), param.pieceSquare);
    setOccupied(&board, param, playerW);
    setOccupied(&board2, param, playerB);

    // act
    movesW = rooks_attack(playerW, board);
    movesB = rooks_attack(playerB, board2);
    
    // expect
    EXPECT_TRUE(BitboardEq(movesW, param.expectedMoves));
    EXPECT_TRUE(BitboardEq(movesB, param.expectedMoves));
}

INSTANTIATE_TEST_SUITE_P(AttackCases, Rook_AttackGenerator,
    ::testing::ValuesIn(load_attack_testcases(json, "rook_tests")));

TEST_P(Bishop_AttackGenerator, bishops_attack) {
    // arrange
    const auto& param = GetParam();
    
    place_piece(&(board.pieces[WHITE_PLAYER][BISHOP]), param.pieceSquare);
    place_piece(&(board2.pieces[BLACK_PLAYER][BISHOP]), param.pieceSquare);
    setOccupied(&board, param, playerW);
    setOccupied(&board2, param, playerB);

    // act
    movesW = bishops_attack(playerW, board);
    movesB = bishops_attack(playerB, board2);
    
    // expect
    EXPECT_TRUE(BitboardEq(movesW, param.expectedMoves));
    EXPECT_TRUE(BitboardEq(movesB, param.expectedMoves));
}

INSTANTIATE_TEST_SUITE_P(AttackCases, Bishop_AttackGenerator,
    ::testing::ValuesIn(load_attack_testcases(json, "bishop_tests")));

TEST_P(Knight_AttackGenerator, knights_attack) {
    // arrange
    const auto& param = GetParam();
    
    place_piece(&(board.pieces[WHITE_PLAYER][KNIGHT]), param.pieceSquare);
    place_piece(&(board2.pieces[BLACK_PLAYER][KNIGHT]), param.pieceSquare);
    setOccupied(&board, param, playerW);
    setOccupied(&board2, param, playerB);

    // act
    movesW = knights_attack(playerW, board);
    movesB = knights_attack(playerB, board2);
    
    // expect
    EXPECT_TRUE(BitboardEq(movesW, param.expectedMoves));
    EXPECT_TRUE(BitboardEq(movesB, param.expectedMoves));
}

INSTANTIATE_TEST_SUITE_P(AttackCases, Knight_AttackGenerator,
    ::testing::ValuesIn(load_attack_testcases(json, "knight_tests")));

TEST_P(Queen_AttackGenerator, queens_attack) {
    // arrange
    const auto& param = GetParam();

    place_piece(&(board.pieces[WHITE_PLAYER][QUEEN]), param.pieceSquare);
    place_piece(&(board2.pieces[BLACK_PLAYER][QUEEN]), param.pieceSquare);
    setOccupied(&board, param, playerW);
    setOccupied(&board2, param, playerB);

    // act
    movesW = queens_attack(playerW, board);
    movesB = queens_attack(playerB, board2);
    
    // expect
    EXPECT_TRUE(BitboardEq(movesW, param.expectedMoves));
    EXPECT_TRUE(BitboardEq(movesB, param.expectedMoves));
}

INSTANTIATE_TEST_SUITE_P(AttackCases, Queen_AttackGenerator,
    ::testing::ValuesIn(load_attack_testcases(json, "queen_tests")));

TEST_P(King_AttackGenerator, king_attack) {
    // arrange
    const auto& param = GetParam();

    place_piece(&(board.pieces[WHITE_PLAYER][KING]), param.pieceSquare);
    place_piece(&(board2.pieces[BLACK_PLAYER][KING]), param.pieceSquare);
    setOccupied(&board, param, playerW);
    setOccupied(&board2, param, playerB);

    // act
    movesW = king_attack(playerW, board);
    movesB = king_attack(playerB, board2);
    
    // expect
    EXPECT_TRUE(BitboardEq(movesW, param.expectedMoves));
    EXPECT_TRUE(BitboardEq(movesB, param.expectedMoves));
}

INSTANTIATE_TEST_SUITE_P(AttackCases, King_AttackGenerator,
    ::testing::ValuesIn(load_attack_testcases(json, "king_tests")));
