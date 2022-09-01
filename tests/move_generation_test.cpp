#include <gtest/gtest.h>
#include "../include/board.hpp"
#include "../include/movegen.hpp"
TEST(MoveGenTest, RookAttacks){
    Board::Board board("6BR/1P1P3p/P1p1pp1K/Q5pn/2p1p3/8/3k3B/5R2 w KQkq - 0 1");
    Moves moves;
    generate_rook_actions(board.bb_piece_type[(board.color_to_move)][int(PieceTypes::Rook)], board.color_to_move, board.bb_occ, moves);
    std::cout << moves;
}
