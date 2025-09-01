#include <gtest/gtest.h>

extern "C" {
#include "GameLogic/Board.h"
}

class Board_TEST : public ::testing::Test
{
public:
    // is available in all TEST_F
    Board board;
    
    // called before every TEST_F
    void SetUp() override {
        init_board_empty(&board);
    }
    
    // called after every TEST_F
    void TearDown() override {

    }
};

TEST(BoardBasics_TEST, init_board)
{
    // arrange
    Board board;

    // act
    init_board(&board);

    // expect
    EXPECT_EQ(board.pieces[WHITE_PLAYER][PAWN]   ,0x00FF000000000000ULL);
    EXPECT_EQ(board.pieces[WHITE_PLAYER][KNIGHT] ,0x4200000000000000ULL);
    EXPECT_EQ(board.pieces[WHITE_PLAYER][BISHOP] ,0x2400000000000000ULL);
    EXPECT_EQ(board.pieces[WHITE_PLAYER][ROOK]   ,0x8100000000000000ULL);
    EXPECT_EQ(board.pieces[WHITE_PLAYER][QUEEN]  ,0x0800000000000000ULL);
    EXPECT_EQ(board.pieces[WHITE_PLAYER][KING]   ,0x1000000000000000ULL);
    EXPECT_EQ(board.pieces[BLACK_PLAYER][PAWN]   ,0x000000000000FF00ULL);
    EXPECT_EQ(board.pieces[BLACK_PLAYER][KNIGHT] ,0x0000000000000042ULL);
    EXPECT_EQ(board.pieces[BLACK_PLAYER][BISHOP] ,0x0000000000000024ULL);
    EXPECT_EQ(board.pieces[BLACK_PLAYER][ROOK]   ,0x0000000000000081ULL);
    EXPECT_EQ(board.pieces[BLACK_PLAYER][QUEEN]  ,0x0000000000000008ULL);
    EXPECT_EQ(board.pieces[BLACK_PLAYER][KING]   ,0x0000000000000010ULL);
    EXPECT_EQ(board.cliked_square_index , 0);
}

TEST(BoardBasics_TEST, init_board_empty)
{
    // arrange
    Board board;

    // act
    init_board_empty(&board);
    
    // expect
    EXPECT_EQ(board.pieces[WHITE_PLAYER][PAWN]   ,0x0000000000000000ULL);
    EXPECT_EQ(board.pieces[WHITE_PLAYER][KNIGHT] ,0x0000000000000000ULL);
    EXPECT_EQ(board.pieces[WHITE_PLAYER][BISHOP] ,0x0000000000000000ULL);
    EXPECT_EQ(board.pieces[WHITE_PLAYER][ROOK]   ,0x0000000000000000ULL);
    EXPECT_EQ(board.pieces[WHITE_PLAYER][QUEEN]  ,0x0000000000000000ULL);
    EXPECT_EQ(board.pieces[WHITE_PLAYER][KING]   ,0x0000000000000000ULL);
    EXPECT_EQ(board.pieces[BLACK_PLAYER][PAWN]   ,0x0000000000000000ULL);
    EXPECT_EQ(board.pieces[BLACK_PLAYER][KNIGHT] ,0x0000000000000000ULL);
    EXPECT_EQ(board.pieces[BLACK_PLAYER][BISHOP] ,0x0000000000000000ULL);
    EXPECT_EQ(board.pieces[BLACK_PLAYER][ROOK]   ,0x0000000000000000ULL);
    EXPECT_EQ(board.pieces[BLACK_PLAYER][QUEEN]  ,0x0000000000000000ULL);
    EXPECT_EQ(board.pieces[BLACK_PLAYER][KING]   ,0x0000000000000000ULL);
    EXPECT_EQ(board.cliked_square_index , 0);
}

TEST_F(Board_TEST, place_piece)
{
    // arrange
    int square1 = A1;
    int square2 = B1;

    // act
    place_piece(&(board.pieces[BLACK_PLAYER][PAWN]), square1);

    // expect
    EXPECT_EQ(board.pieces[BLACK_PLAYER][PAWN], (1ULL << square1));

    // act
    place_piece(&(board.pieces[BLACK_PLAYER][PAWN]), square2);

    // expect
    EXPECT_EQ(board.pieces[BLACK_PLAYER][PAWN], (1ULL << square1) | (1ULL << square2));
}

TEST_F(Board_TEST, remove_piece)
{
    // arrange
    int square1 = A1;
    int square2 = B1;
    place_piece(&(board.pieces[BLACK_PLAYER][PAWN]), square1);
    place_piece(&(board.pieces[BLACK_PLAYER][PAWN]), square2);

    // act
    remove_piece(&(board.pieces[BLACK_PLAYER][PAWN]), square2);

    // expect
    EXPECT_EQ(board.pieces[BLACK_PLAYER][PAWN], (1ULL << square1));

    // act
    remove_piece(&(board.pieces[BLACK_PLAYER][PAWN]), square1);

    // expect
    EXPECT_EQ(board.pieces[BLACK_PLAYER][PAWN], 0);
}

TEST_F(Board_TEST, is_occupied)
{
    // arrange
    bool result1;
    bool result2;
    bool result3;
    int square1 = A1;
    int square2 = B1;
    int square3 = E4;
    place_piece(&(board.pieces[BLACK_PLAYER][PAWN]), square1);
    place_piece(&(board.pieces[BLACK_PLAYER][PAWN]), square2);

    // act
    result1 = is_occupied(board.pieces[BLACK_PLAYER][PAWN], square1);
    result2 = is_occupied(board.pieces[BLACK_PLAYER][PAWN], square2);
    result3 = is_occupied(board.pieces[BLACK_PLAYER][PAWN], square3);
    
    // expect
    EXPECT_TRUE(result1);
    EXPECT_TRUE(result2);
    EXPECT_FALSE(result3);
    

    // arrange
    remove_piece(&(board.pieces[BLACK_PLAYER][PAWN]), square2);

    // act
    result1 = is_occupied(board.pieces[BLACK_PLAYER][PAWN], square1);
    result2 = is_occupied(board.pieces[BLACK_PLAYER][PAWN], square2);
    result3 = is_occupied(board.pieces[BLACK_PLAYER][PAWN], square3);

    // expect
    EXPECT_TRUE(result1);
    EXPECT_FALSE(result2);
    EXPECT_FALSE(result3);
}

TEST_F(Board_TEST, update_board)
{
    // arrange
    init_board(&board);
    place_piece(&(board.pieces[BLACK_PLAYER][PAWN]), E4);
    place_piece(&(board.pieces[BLACK_PLAYER][PAWN]), D4);
    place_piece(&(board.pieces[WHITE_PLAYER][BISHOP]), F5);

    // act
    update_board(&board);
    
    // expect
    Bitboard WOccupied = 0;
    Bitboard BOccupied = 0;
    Bitboard SOccupied = 0;
    for (int i = 0; i < 6; i++)
    {
        WOccupied |= board.pieces[WHITE_PLAYER][i];
        BOccupied |= board.pieces[BLACK_PLAYER][i];
    }
    SOccupied = WOccupied | BOccupied;

    ASSERT_EQ(board.white_occupied, WOccupied);
    ASSERT_EQ(board.black_occupied, BOccupied);
    EXPECT_EQ(board.occupied_squares, SOccupied);
}
