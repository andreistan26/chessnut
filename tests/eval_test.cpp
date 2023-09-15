#include <gtest/gtest.h>
#include "../include/board.hpp"

std::unordered_map<std::string, int> fen_test = {
    {"r4rk1/ppq1npbp/2n1p1p1/3p4/3P4/2PB1N1P/PP2QPP1/R1B1R1K1 w - - 6 13", 104},
};

TEST(TestEval, Eval1){
    for(auto &a : fen_test){
        Board::Board board(a.first.c_str());
        std::cerr << "engine score " << evaluate(board) << " | stockfish score " <<  a.second << std::endl;
    }
}