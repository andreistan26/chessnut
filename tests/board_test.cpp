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
}

TEST_F(BoardTest, ParsingInitial){
}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
