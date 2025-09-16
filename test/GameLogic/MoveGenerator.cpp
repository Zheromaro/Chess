#include <gtest/gtest.h>
#include "GeneratorUtilities.h"

extern "C" {
#include "GameLogic/MoveGenerator.h"
#include "GameLogic/Board.h"
#include "jsonReading.h"
#include <stdio.h>
}

#pragma region Test Setup
// -------- for Test_F ---------
static cJSON* json = get_JSON_data_in((char*)"/home/zhero/projects/Chess/test/GameLogic/Generators_test.json");

class MoveGenerator : public ::testing::Test
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
class WPawn_MoveGenerator : public MoveGenerator, 
                       public ::testing::WithParamInterface<PawnMoveTestCase> {};

class BPawn_MoveGenerator : public MoveGenerator, 
                       public ::testing::WithParamInterface<PawnMoveTestCase> {};
                       
class Rook_MoveGenerator : public MoveGenerator, 
                       public ::testing::WithParamInterface<MoveTestCase> {};
                       
class Bishop_MoveGenerator : public MoveGenerator, 
                       public ::testing::WithParamInterface<MoveTestCase> {};
                       
class Knight_MoveGenerator : public MoveGenerator, 
                       public ::testing::WithParamInterface<MoveTestCase> {};
                       
class Queen_MoveGenerator : public MoveGenerator, 
                       public ::testing::WithParamInterface<MoveTestCase> {};
                       
class King_MoveGenerator : public MoveGenerator, 
                       public ::testing::WithParamInterface<KingMoveTestCase> {};

template <typename TestCaseT, typename F>
std::vector<TestCaseT> load_move_testcases_template(
    cJSON* json,
    const char* object_name,
    F customize_case)
{
    int count = 0;
    char** boards = get_expected_boards(json, object_name, &count);
    std::vector<TestCaseT> cases;
    cases.reserve(count);

    for (int i = 0; i < count; i++) {
        short pieceSquare = __builtin_ctzll(string_to_bitboard(boards[i], 'P'));
        uint64_t blockers = string_to_bitboard(boards[i], 'B');
        uint64_t captures = string_to_bitboard(boards[i], 'C');
        uint64_t expected = string_to_bitboard(boards[i], '1');

        TestCaseT tc{};
        tc.pieceSquare = pieceSquare;
        tc.blockers = blockers;
        tc.captures = captures;
        tc.expectedMoves = expected | captures;

        // Call specialization lambda (for Pawn/King extras)
        customize_case(tc, boards[i], i);
        cases.push_back(tc);
    }

    for (int i = 0; i < count; i++) free(boards[i]);
    free(boards);

    return cases;
}

std::vector<MoveTestCase> load_move_testcases(cJSON* json, const char* object_name) {
    return load_move_testcases_template<MoveTestCase>(json, object_name,
        [](MoveTestCase&, const char*, const int) { /* nothing extra */ });
}
std::vector<PawnMoveTestCase> load_move_testcases_Pawn(cJSON* json, const char* object_name) {
    return load_move_testcases_template<PawnMoveTestCase>(json, object_name,
        [](PawnMoveTestCase& tc, const char* board, const int i) {
            uint64_t en_passant = string_to_bitboard(board, 'E');
            if(en_passant)
            {
                int enPassantSquare = __builtin_ctzll(en_passant);
                tc.enPassantSquare = (short)enPassantSquare;
                tc.expectedMoves |= (1ULL << enPassantSquare);
            }
        });
}
std::vector<KingMoveTestCase> load_move_testcases_King(cJSON* json, const char* object_name) {
    int count = 0;
    char **castling_rights = get_castling_rights(json, object_name, &count);
    
    std::vector<KingMoveTestCase> kingTestCases = 
    load_move_testcases_template<KingMoveTestCase>(json, object_name,
        [castling_rights](KingMoveTestCase& tc, const char* board, const int i) {
            if (castling_rights[i])
            {
                if (strcmp(castling_rights[i], (char*)"WQUEEN_SIDE_CASTLE_RIGHT") == 0)
                    tc.castling_rights = WQUEEN_SIDE_CASTLE_RIGHT;
                else if (strcmp(castling_rights[i], (char*)"WKING_SIDE_CASTLE_RIGHT") == 0)
                    tc.castling_rights = WKING_SIDE_CASTLE_RIGHT;
                else if (strcmp(castling_rights[i], (char*)"BQUEEN_SIDE_CASTLE_RIGHT") == 0)
                    tc.castling_rights = BQUEEN_SIDE_CASTLE_RIGHT;
                else if (strcmp(castling_rights[i], (char*)"BKING_SIDE_CASTLE_RIGHT") == 0)
                    tc.castling_rights = BKING_SIDE_CASTLE_RIGHT;
            }
        });

    for (int j = 0; j < count; j++) free(castling_rights[j]);
    free(castling_rights);

    return kingTestCases;
}

#pragma endregion

TEST_P(WPawn_MoveGenerator, wpawn_moves) {
    // arrange
    const auto& param = GetParam();

    place_piece(&(board.pieces[WHITE_PLAYER][PAWN]), param.pieceSquare);
    setOccupied(&board, param, playerW);
    board.en_passant_square = param.enPassantSquare;

    // act
    moves = pawn_moves(playerW, board, param.pieceSquare);
    
    // expect
    EXPECT_TRUE(BitboardEq(moves, param.expectedMoves));
}

INSTANTIATE_TEST_SUITE_P(MoveCases, WPawn_MoveGenerator,
    ::testing::ValuesIn(load_move_testcases_Pawn(json, "wpawn_tests")));

TEST_P(BPawn_MoveGenerator, bpawn_moves) {
    // arrange
    const auto& param = GetParam();
    
    place_piece(&(board.pieces[BLACK_PLAYER][PAWN]), param.pieceSquare);
    setOccupied(&board, param, playerB);
    board.en_passant_square = param.enPassantSquare;

    // act
    moves = pawn_moves(playerB, board, param.pieceSquare);
    
    // expect
    EXPECT_TRUE(BitboardEq(moves, param.expectedMoves));
}

INSTANTIATE_TEST_SUITE_P(MoveCases, BPawn_MoveGenerator,
    ::testing::ValuesIn(load_move_testcases_Pawn(json, "bpawn_tests")));

TEST_P(Rook_MoveGenerator, rook_moves) {
    // arrange
    const auto& param = GetParam();
    
    place_piece(&(board.pieces[WHITE_PLAYER][ROOK]), param.pieceSquare);
    place_piece(&(board2.pieces[BLACK_PLAYER][ROOK]), param.pieceSquare);
    setOccupied(&board, param, playerW);
    setOccupied(&board2, param, playerB);

    // act
    movesW = rook_moves(playerW, board, param.pieceSquare);
    movesB = rook_moves(playerB, board2, param.pieceSquare);
    
    // expect
    EXPECT_TRUE(BitboardEq(movesW, param.expectedMoves));
    EXPECT_TRUE(BitboardEq(movesB, param.expectedMoves));
}

INSTANTIATE_TEST_SUITE_P(MoveCases, Rook_MoveGenerator,
    ::testing::ValuesIn(load_move_testcases(json, "rook_tests")));

TEST_P(Bishop_MoveGenerator, bishop_moves) {
    // arrange
    const auto& param = GetParam();
    
    place_piece(&(board.pieces[WHITE_PLAYER][BISHOP]), param.pieceSquare);
    place_piece(&(board2.pieces[BLACK_PLAYER][BISHOP]), param.pieceSquare);
    setOccupied(&board, param, playerW);
    setOccupied(&board2, param, playerB);

    // act
    movesW = bishop_moves(playerW, board, param.pieceSquare);
    movesB = bishop_moves(playerB, board2, param.pieceSquare);
    
    // expect
    EXPECT_TRUE(BitboardEq(movesW, param.expectedMoves));
    EXPECT_TRUE(BitboardEq(movesB, param.expectedMoves));
}

INSTANTIATE_TEST_SUITE_P(MoveCases, Bishop_MoveGenerator,
    ::testing::ValuesIn(load_move_testcases(json, "bishop_tests")));

TEST_P(Knight_MoveGenerator, knight_moves) {
    // arrange
    const auto& param = GetParam();

    place_piece(&(board.pieces[WHITE_PLAYER][KNIGHT]), param.pieceSquare);
    place_piece(&(board2.pieces[BLACK_PLAYER][KNIGHT]), param.pieceSquare);
    setOccupied(&board, param, playerW);
    setOccupied(&board2, param, playerB);

    // act
    movesW = knight_moves(playerW, board, param.pieceSquare);
    movesB = knight_moves(playerB, board2, param.pieceSquare);
    
    // expect
    EXPECT_TRUE(BitboardEq(movesW, param.expectedMoves));
    EXPECT_TRUE(BitboardEq(movesB, param.expectedMoves));
}

INSTANTIATE_TEST_SUITE_P(MoveCases, Knight_MoveGenerator,
    ::testing::ValuesIn(load_move_testcases(json, "knight_tests")));

TEST_P(Queen_MoveGenerator, queen_moves) {
    // arrange
    const auto& param = GetParam();

    place_piece(&(board.pieces[WHITE_PLAYER][QUEEN]), param.pieceSquare);
    place_piece(&(board2.pieces[BLACK_PLAYER][QUEEN]), param.pieceSquare);
    setOccupied(&board, param, playerW);
    setOccupied(&board2, param, playerB);

    // act
    movesW = queen_moves(playerW, board, param.pieceSquare);
    movesB = queen_moves(playerB, board2, param.pieceSquare);
    
    // expect
    EXPECT_TRUE(BitboardEq(movesW, param.expectedMoves));
    EXPECT_TRUE(BitboardEq(movesB, param.expectedMoves));
}

INSTANTIATE_TEST_SUITE_P(MoveCases, Queen_MoveGenerator,
    ::testing::ValuesIn(load_move_testcases(json, "queen_tests")));

TEST_P(King_MoveGenerator, king_moves) {
    // arrange
    const auto& param = GetParam();

    place_piece(&(board.pieces[WHITE_PLAYER][QUEEN]), param.pieceSquare);
    place_piece(&(board2.pieces[BLACK_PLAYER][QUEEN]), param.pieceSquare);
    setOccupied(&board, param, playerW);
    setOccupied(&board2, param, playerB);
    board.castling_rights = param.castling_rights;
    board2.castling_rights = param.castling_rights;

    // act
    movesW = king_moves(playerW, board, param.pieceSquare);
    movesB = king_moves(playerB, board2, param.pieceSquare);
    
    // expect
    if (!board.castling_rights)
    {
        EXPECT_TRUE(BitboardEq(movesW, param.expectedMoves));
        EXPECT_TRUE(BitboardEq(movesB, param.expectedMoves));
    }
    else if (board.castling_rights & WKING_SIDE_CASTLE_RIGHT ||
             board.castling_rights & WQUEEN_SIDE_CASTLE_RIGHT)
    {
        EXPECT_TRUE(BitboardEq(movesW, param.expectedMoves));
    }
    else if (board.castling_rights & WKING_SIDE_CASTLE_RIGHT ||
             board.castling_rights & WQUEEN_SIDE_CASTLE_RIGHT)
    {
        EXPECT_TRUE(BitboardEq(movesB, param.expectedMoves));
    }
    
}

INSTANTIATE_TEST_SUITE_P(MoveCases, King_MoveGenerator,
    ::testing::ValuesIn(load_move_testcases_King(json, "king_tests")));
