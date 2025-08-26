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
    EXPECT_EQ(board.white_pawns   ,0x000000000000FF00ULL);
    EXPECT_EQ(board.white_knights ,0x0000000000000042ULL);
    EXPECT_EQ(board.white_bishops ,0x0000000000000024ULL);
    EXPECT_EQ(board.white_rooks   ,0x0000000000000081ULL);
    EXPECT_EQ(board.white_queen   ,0x0000000000000008ULL);
    EXPECT_EQ(board.white_king    ,0x0000000000000010ULL);
    EXPECT_EQ(board.black_pawns   ,0x00FF000000000000ULL);
    EXPECT_EQ(board.black_knights ,0x4200000000000000ULL);
    EXPECT_EQ(board.black_bishops ,0x2400000000000000ULL);
    EXPECT_EQ(board.black_rooks   ,0x8100000000000000ULL);
    EXPECT_EQ(board.black_queen   ,0x0800000000000000ULL);
    EXPECT_EQ(board.black_king    ,0x1000000000000000ULL);
    EXPECT_EQ(board.cliked_square ,0x0000000000000000ULL);
}

TEST(BoardBasics_TEST, init_board_empty)
{
    // arrange
    Board board;

    // act
    init_board_empty(&board);

    // expect
    EXPECT_EQ(board.white_pawns   ,0x0000000000000000ULL);
    EXPECT_EQ(board.white_knights ,0x0000000000000000ULL);
    EXPECT_EQ(board.white_bishops ,0x0000000000000000ULL);
    EXPECT_EQ(board.white_rooks   ,0x0000000000000000ULL);
    EXPECT_EQ(board.white_queen   ,0x0000000000000000ULL);
    EXPECT_EQ(board.white_king    ,0x0000000000000000ULL);
    EXPECT_EQ(board.black_pawns   ,0x0000000000000000ULL);
    EXPECT_EQ(board.black_knights ,0x0000000000000000ULL);
    EXPECT_EQ(board.black_bishops ,0x0000000000000000ULL);
    EXPECT_EQ(board.black_rooks   ,0x0000000000000000ULL);
    EXPECT_EQ(board.black_queen   ,0x0000000000000000ULL);
    EXPECT_EQ(board.black_king    ,0x0000000000000000ULL);
    EXPECT_EQ(board.cliked_square ,0x0000000000000000ULL);
}

TEST_F(Board_TEST, click_on_square)
{
    // arrange
    int square1 = A1;
    int square2 = B1;

    // act
    click_on_square(&board, square1);

    // expect
    EXPECT_EQ(board.cliked_square, square1);

    // act
    click_on_square(&board, square2);

    // expect
    EXPECT_EQ(board.cliked_square, square2);

    // act
    click_on_square(&board, square2);

    // expect
    EXPECT_EQ(board.cliked_square, 0);
}

TEST_F(Board_TEST, place_piece)
{
    // arrange
    int square1 = A1;
    int square2 = B1;

    // act
    place_piece(&(board.black_pawns), square1);

    // expect
    EXPECT_EQ(board.black_pawns, (1ULL << square1));

    // act
    place_piece(&(board.black_pawns), square2);

    // expect
    EXPECT_EQ(board.black_pawns, (1ULL << square1) | (1ULL << square2));
}

TEST_F(Board_TEST, remove_piece)
{
    // arrange
    int square1 = A1;
    int square2 = B1;
    place_piece(&(board.black_pawns), square1);
    place_piece(&(board.black_pawns), square2);

    // act
    remove_piece(&(board.black_pawns), square2);

    // expect
    EXPECT_EQ(board.black_pawns, (1ULL << square1));

    // act
    remove_piece(&(board.black_pawns), square1);

    // expect
    EXPECT_EQ(board.black_pawns, 0);

    // act
    remove_piece(&(board.black_pawns), square1);

    // expect
    EXPECT_EQ(board.black_pawns, 0);
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
    place_piece(&(board.black_pawns), square1);
    place_piece(&(board.black_pawns), square2);

    // act
    result1 = is_occupied(board.black_pawns, square1);
    result2 = is_occupied(board.black_pawns, square2);
    result3 = is_occupied(board.black_pawns, square3);
    
    // expect
    EXPECT_TRUE(result1);
    EXPECT_TRUE(result2);
    EXPECT_FALSE(result3);
    

    // arrange
    remove_piece(&(board.black_pawns), square2);

    // act
    result1 = is_occupied(board.black_pawns, square1);
    result2 = is_occupied(board.black_pawns, square2);
    result3 = is_occupied(board.black_pawns, square3);

    // expect
    EXPECT_TRUE(result1);
    EXPECT_FALSE(result2);
    EXPECT_FALSE(result3);
}

TEST_F(Board_TEST, update_board)
{
    // arrange
    init_board(&board);
    place_piece(&(board.black_pawns), E4);
    place_piece(&(board.black_pawns), D4);
    place_piece(&(board.white_bishops), F5);

    // act
    update_board(&board);
    
    // expect
    ASSERT_EQ(board.white_occupied, board.white_pawns | board.white_knights |
                                    board.white_bishops | board.white_rooks |
                                    board.white_queen | board.white_king);
    ASSERT_EQ(board.black_occupied, board.black_pawns | board.black_knights |
                                    board.black_bishops | board.black_rooks |
                                    board.black_queen | board.black_king);
    EXPECT_EQ(board.occupied_squares, board.black_occupied | board.white_occupied);
}
