#include <gtest/gtest.h>

extern "C" {
#include "GameLogic/PieceMoveGenerator.h"
#include "GameLogic/Board.h"
#include <stdio.h>
}

class PieceMoveGenerator : public ::testing::Test
{
public:
    // is available in all TEST_F
    Board board;
    
    // called before every TEST_F
    void SetUp() override {
        // empty board
        board.white_pawns   = 0x0000000000000000ULL;
        board.white_knights = 0x0000000000000000ULL;
        board.white_bishops = 0x0000000000000000ULL;
        board.white_rooks   = 0x0000000000000000ULL;
        board.white_queen   = 0x0000000000000000ULL;
        board.white_king    = 0x0000000000000000ULL;
        board.black_pawns   = 0x0000000000000000ULL;
        board.black_knights = 0x0000000000000000ULL;
        board.black_bishops = 0x0000000000000000ULL;
        board.black_rooks   = 0x0000000000000000ULL;
        board.black_queen   = 0x0000000000000000ULL;
        board.black_king    = 0x0000000000000000ULL;
        board.cliked_square = 0x0000000000000000ULL;
    }
    
    // called after every TEST_F
    void TearDown() override {

    }
};

TEST_F(PieceMoveGenerator, wpawn_moves)
{
    // arrange
    Bitboard wpawn1moves = 0;
    Bitboard wpawn2moves = 0;
    Bitboard wpawn3moves = 0;
    Bitboard wpawn4moves = 0;
    uint64_t wpawn1 = E2;
    uint64_t wpawn2 = D2;
    uint64_t wpawn3 = D3;
    uint64_t wpawn4 = F8;
    uint64_t bpawn1 = E4;
    uint64_t bpawn2 = C4;
    uint64_t bpawn3 = C3;
    place_piece(&(board.white_pawns), wpawn1);
    place_piece(&(board.white_pawns), wpawn2);
    place_piece(&(board.white_pawns), wpawn3);
    place_piece(&(board.white_pawns), wpawn4);
    place_piece(&(board.black_pawns), bpawn1);
    place_piece(&(board.black_pawns), bpawn2);
    place_piece(&(board.black_pawns), bpawn3);
    update_board(&board);
    print_bitboard(wpawn2moves);
    print_bitboard(1ULL << C3);

    // act
    wpawn1moves = wpawn_moves(WHITE_PLAYER, board, wpawn1);
    wpawn2moves = wpawn_moves(WHITE_PLAYER, board, wpawn2);
    wpawn3moves = wpawn_moves(WHITE_PLAYER, board, wpawn3);
    wpawn4moves = wpawn_moves(WHITE_PLAYER, board, wpawn4);

    // expect
    EXPECT_EQ(wpawn1moves, (1ULL << E3));
    EXPECT_EQ(wpawn2moves, (1ULL << C3));
    EXPECT_EQ(wpawn3moves, (1ULL << D4) | (1ULL << C4) | (1ULL << E4));
    EXPECT_EQ(wpawn4moves, 0);
}