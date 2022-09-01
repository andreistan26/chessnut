#include <gtest/gtest.h>
#include "../include/board.hpp"
class BoardTest : public ::testing::Test{
    protected:
        void SetUp() override {
            starting_board = Board::Board(default_fen_string);
        }

        //void TearDown() override {}

        Board::Board starting_board;
        const char* default_fen_string = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
};

TEST(BoardMacroTest, MacroTest){
    EXPECT_EQ(FEN_CHECK_PIECE("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"[0]), 1);
    EXPECT_EQ(FEN_CHECK_PIECE('q'), 1);
    EXPECT_EQ(FEN_CHECK_PIECE('p'), 1);
    EXPECT_EQ(FEN_CHECK_PIECE('R'), 1);
    EXPECT_EQ(FEN_CHECK_PIECE('Q'), 1);
    EXPECT_EQ(FEN_CHECK_EMPTY('1'), 1);
    EXPECT_EQ(FEN_CHECK_EMPTY('9'), 0);
    EXPECT_EQ(FEN_CHECK_EMPTY('a'), 0);
    EXPECT_EQ(FEN_CHECK_EMPTY('0'), 0);
}

TEST_F(BoardTest, ParsingInitial){
    ASSERT_EQ(starting_board.bb_piece_type[0][int(PieceTypes::Pawn)], (Bitboard)0xff00);
}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
