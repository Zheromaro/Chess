#include <gtest/gtest.h>

extern "C" {
#include "GameLogic/MoveGenerator.h"
#include "GameLogic/Board.h"
#include "jsonReading.h"
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

Bitboard string_to_bitboard(const char* bb_string, const char to_search) {
    Bitboard result = 0;
    printf("Parsing for %c:\n%s\n", to_search, bb_string);

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
    printf("Result for %c:\n %s\n", to_search, bitboard_to_string(result).c_str());

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

// -------- for Test_F ---------
static cJSON* json = get_JSON_data_in((char*)"/home/zhero/projects/Chess/test/GameLogic/MoveGenerator.json");

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
struct MoveTestCase {
    short pieceSquare;
    uint64_t blockers;
    uint64_t captures;
    uint64_t expectedMoves;
};
struct PawnMoveTestCase {
    short pieceSquare;
    uint64_t blockers;
    uint64_t captures;
    uint64_t expectedMoves;
    short enPassantSquare = 0; // default: no en passant
};
struct KingMoveTestCase {
    short pieceSquare;
    uint64_t blockers;
    uint64_t captures;
    uint64_t expectedMoves;
    uint8_t castling_rights = 0; // default: no castling_rights
};

std::vector<MoveTestCase> load_move_testcases_MTC(cJSON* json, const char* object_name) {
    int count = 0;
    char **boards = get_expected_boards(json, object_name, &count);
    std::vector<MoveTestCase> cases;

    for (int i = 0; i < count; i++) {
        short pieceSquare = __builtin_ctzll(string_to_bitboard(boards[i], 'P'));
        uint64_t blockers = string_to_bitboard(boards[i], 'B');
        uint64_t captures = string_to_bitboard(boards[i], 'C');
        uint64_t expected = string_to_bitboard(boards[i], '1');

        MoveTestCase tc;
        tc.pieceSquare = pieceSquare;
        tc.blockers = blockers;
        tc.captures = captures;
        tc.expectedMoves = expected | captures;

        cases.push_back(tc);
    }

    for (int i = 0; i < count; i++) free(boards[i]);
    free(boards);

    return cases;
}

std::vector<PawnMoveTestCase> load_move_testcases_PawnMTC(cJSON* json, const char* object_name) {
    int count = 0;
    char **boards = get_expected_boards(json, object_name, &count);
    std::vector<PawnMoveTestCase> cases;

    for (int i = 0; i < count; i++) {
        int pieceSquare = __builtin_ctzll(string_to_bitboard(boards[i], 'P'));
        uint64_t en_passant = string_to_bitboard(boards[i], 'E');
        uint64_t blockers = string_to_bitboard(boards[i], 'B');
        uint64_t captures = string_to_bitboard(boards[i], 'C');
        uint64_t expected = string_to_bitboard(boards[i], '1');
        
        PawnMoveTestCase tc;
        tc.pieceSquare = (short)pieceSquare;
        tc.blockers = blockers;
        tc.captures = captures;
        tc.expectedMoves = expected | captures;

        if(en_passant)
        {
            int enPassantSquare = __builtin_ctzll(en_passant);
            tc.enPassantSquare = (short)enPassantSquare;
            tc.expectedMoves |= (1ULL << enPassantSquare);
        }

        cases.push_back(tc);
    }

    for (int i = 0; i < count; i++) free(boards[i]);
    free(boards);

    return cases;
}

std::vector<KingMoveTestCase> load_move_testcases_KingMTC(cJSON* json, const char* object_name) {
    int count = 0;
    char **boards = get_expected_boards(json, object_name, &count);
    std::vector<KingMoveTestCase> cases;

    for (int i = 0; i < count; i++) {
        short pieceSquare = __builtin_ctzll(string_to_bitboard(boards[i], 'P'));
        uint64_t blockers = string_to_bitboard(boards[i], 'B');
        uint64_t captures = string_to_bitboard(boards[i], 'C');
        uint64_t expected = string_to_bitboard(boards[i], '1');

        KingMoveTestCase tc;
        tc.pieceSquare = pieceSquare;
        tc.blockers = blockers;
        tc.captures = captures;
        tc.expectedMoves = expected | captures;
        tc.castling_rights = 0;

        cases.push_back(tc);
    }

    for (int i = 0; i < count; i++) free(boards[i]);
    free(boards);

    return cases;
}

class WPawnGenerator : public MoveGenerator, 
                       public ::testing::WithParamInterface<PawnMoveTestCase> {};

class BPawnGenerator : public MoveGenerator, 
                       public ::testing::WithParamInterface<PawnMoveTestCase> {};
                       
class RookGenerator : public MoveGenerator, 
                       public ::testing::WithParamInterface<MoveTestCase> {};
                       
class BishopGenerator : public MoveGenerator, 
                       public ::testing::WithParamInterface<MoveTestCase> {};
                       
class KnightGenerator : public MoveGenerator, 
                       public ::testing::WithParamInterface<MoveTestCase> {};
                       
class QueenGenerator : public MoveGenerator, 
                       public ::testing::WithParamInterface<MoveTestCase> {};
                       
class KingGenerator : public MoveGenerator, 
                       public ::testing::WithParamInterface<KingMoveTestCase> {};
#pragma endregion

TEST_P(WPawnGenerator, wpawn_moves) {
    // arrange
    const auto& param = GetParam();

    place_piece(&(board.pieces[WHITE_PLAYER][PAWN]), param.pieceSquare);
    board.pieces[WHITE_PLAYER][PAWN] = param.blockers;
    board.pieces[BLACK_PLAYER][PAWN] = param.captures;
    board.en_passant_square = param.enPassantSquare;
    update_board(&board);

    // act
    moves = pawn_moves(playerW, board, param.pieceSquare);
    
    // expect
    EXPECT_TRUE(BitboardEq(moves, param.expectedMoves));
}

INSTANTIATE_TEST_SUITE_P(DefaultCases, WPawnGenerator,
    ::testing::ValuesIn(load_move_testcases_PawnMTC(json, "wpawn_tests")));

TEST_P(BPawnGenerator, bpawn_moves) {
    // arrange
    const auto& param = GetParam();
    
    place_piece(&(board.pieces[BLACK_PLAYER][PAWN]), param.pieceSquare);
    board.pieces[BLACK_PLAYER][PAWN] = param.blockers;
    board.pieces[WHITE_PLAYER][PAWN] = param.captures;
    board.en_passant_square = param.enPassantSquare;
    update_board(&board);

    // act
    moves = pawn_moves(playerB, board, param.pieceSquare);
    
    // expect
    EXPECT_TRUE(BitboardEq(moves, param.expectedMoves));
}

INSTANTIATE_TEST_SUITE_P(DefaultCases, BPawnGenerator,
    ::testing::ValuesIn(load_move_testcases_PawnMTC(json, "bpawn_tests")));

TEST_P(RookGenerator, rook_moves) {
    // arrange
    const auto& param = GetParam();
    
    place_piece(&(board.pieces[WHITE_PLAYER][ROOK]), param.pieceSquare);
    place_piece(&(board2.pieces[BLACK_PLAYER][ROOK]), param.pieceSquare);
    board.pieces[WHITE_PLAYER][PAWN] = param.blockers;
    board.pieces[BLACK_PLAYER][PAWN] = param.captures;
    board2.pieces[BLACK_PLAYER][PAWN] = param.blockers;
    board2.pieces[WHITE_PLAYER][PAWN] = param.captures;
    update_board(&board);
    update_board(&board2);

    // act
    movesW = rook_moves(playerW, board, param.pieceSquare);
    movesB = rook_moves(playerB, board2, param.pieceSquare);
    
    // expect
    EXPECT_TRUE(BitboardEq(movesW, param.expectedMoves));
    EXPECT_TRUE(BitboardEq(movesB, param.expectedMoves));
}

INSTANTIATE_TEST_SUITE_P(DefaultCases, RookGenerator,
    ::testing::ValuesIn(load_move_testcases_MTC(json, "rook_tests")));

TEST_P(BishopGenerator, bishop_moves) {
    // arrange
    const auto& param = GetParam();
    
    place_piece(&(board.pieces[WHITE_PLAYER][BISHOP]), param.pieceSquare);
    place_piece(&(board2.pieces[BLACK_PLAYER][BISHOP]), param.pieceSquare);
    board.pieces[WHITE_PLAYER][PAWN] = param.blockers;
    board.pieces[BLACK_PLAYER][PAWN] = param.captures;
    board2.pieces[BLACK_PLAYER][PAWN] = param.blockers;
    board2.pieces[WHITE_PLAYER][PAWN] = param.captures;
    update_board(&board);
    update_board(&board2);

    // act
    movesW = bishop_moves(playerW, board, param.pieceSquare);
    movesB = bishop_moves(playerB, board2, param.pieceSquare);
    
    // expect
    EXPECT_TRUE(BitboardEq(movesW, param.expectedMoves));
    EXPECT_TRUE(BitboardEq(movesB, param.expectedMoves));
}

INSTANTIATE_TEST_SUITE_P(DefaultCases, BishopGenerator,
    ::testing::ValuesIn(load_move_testcases_MTC(json, "bishop_tests")));

TEST_P(KnightGenerator, knight_moves) {
    // arrange
    const auto& param = GetParam();

    place_piece(&(board.pieces[WHITE_PLAYER][KNIGHT]), param.pieceSquare);
    place_piece(&(board2.pieces[BLACK_PLAYER][KNIGHT]), param.pieceSquare);
    board.pieces[WHITE_PLAYER][PAWN] = param.blockers;
    board.pieces[BLACK_PLAYER][PAWN] = param.captures;
    board2.pieces[BLACK_PLAYER][PAWN] = param.blockers;
    board2.pieces[WHITE_PLAYER][PAWN] = param.captures;
    update_board(&board);
    update_board(&board2);

    // act
    movesW = knight_moves(playerW, board, param.pieceSquare);
    movesB = knight_moves(playerB, board2, param.pieceSquare);
    
    // expect
    EXPECT_TRUE(BitboardEq(movesW, param.expectedMoves));
    EXPECT_TRUE(BitboardEq(movesB, param.expectedMoves));
}

INSTANTIATE_TEST_SUITE_P(DefaultCases, KnightGenerator,
    ::testing::ValuesIn(load_move_testcases_MTC(json, "knight_tests")));

TEST_P(QueenGenerator, queen_moves) {
    // arrange
    const auto& param = GetParam();

    place_piece(&(board.pieces[WHITE_PLAYER][QUEEN]), param.pieceSquare);
    place_piece(&(board2.pieces[BLACK_PLAYER][QUEEN]), param.pieceSquare);
    board.pieces[WHITE_PLAYER][PAWN] = param.blockers;
    board.pieces[BLACK_PLAYER][PAWN] = param.captures;
    board2.pieces[BLACK_PLAYER][PAWN] = param.blockers;
    board2.pieces[WHITE_PLAYER][PAWN] = param.captures;
    update_board(&board);
    update_board(&board2);

    // act
    movesW = queen_moves(playerW, board, param.pieceSquare);
    movesB = queen_moves(playerB, board2, param.pieceSquare);
    
    // expect
    EXPECT_TRUE(BitboardEq(movesW, param.expectedMoves));
    EXPECT_TRUE(BitboardEq(movesB, param.expectedMoves));
}

INSTANTIATE_TEST_SUITE_P(DefaultCases, QueenGenerator,
    ::testing::ValuesIn(load_move_testcases_MTC(json, "queen_tests")));
