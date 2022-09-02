#include <gtest/gtest.h>
#include "../include/board.hpp"
#include "../include/movegen.hpp"
TEST(MoveGenTest, RookAttacks){
    init_all_attack_bitboards();
    Board::Board board("2R4K/R1P1p1p1/1P1B3n/bB3P2/brppP3/nP1p1P1P/k3ppPp/Nq2Q1rN w - - 0 1");
    Moves moves;
    board.color_to_move = Color::White;
    generate_rook_actions(board.bb_piece_type[(int(board.color_to_move))][int(PieceTypes::Rook)],
            board.bb_occ_rot,
            board.color_to_move,
            board.bb_occ,
            moves
        );
    EXPECT_TRUE(moves.moves.size()!=0);
}

TEST(MoveGenTest, BishopAttacksInternals){
    Bitboard bb = 0x20100004020000ULL;
    Bitboard bb_r = bb_rotate_45_cw(bb);
    EXPECT_EQ(first_diagonal_attacks[26][(bb_r >> diagonal_shift_mine[26]) & 0xff], 0x100800020000);
    Bitboard bb_2 = 0x40201000000ULL;
    Bitboard bb_r_2 = bb_rotate_45_cw(bb_2);
    EXPECT_EQ(first_diagonal_attacks[42][(bb_r_2 >> diagonal_shift_mine[42]) & 0xff], 0x1008000200000000);
}

TEST(MoveGenTest, BishopAttacks){
    Board::Board board("2R4K/R1P1p1p1/1P1B3n/bB3P2/brppP3/nP1p1P1P/k3ppPp/Nq2Q1r1 w - - 0 1");
    Moves moves;
    generate_bishop_actions(board.bb_piece_type[int(board.color_to_move)][int(PieceTypes::Bishop)],
            board.bb_occ_rot_main_diag, board.bb_occ_rot_second_diag,
            board.color_to_move,
            board.bb_occ[int(board.color_to_move)],
            moves
        );
    EXPECT_EQ(moves.moves.size(), 9);
}


TEST(MoveGenTest, QueenAttacks){
    Board::Board board("2R4K/R1P1p1p1/1P1B3n/bB3P2/brppP3/nP1p1P1P/k3ppPp/Nq2Q1r1 w - - 0 1");
    Moves moves;
    generate_queen_actions(board.bb_piece_type[int(board.color_to_move)][int(PieceTypes::Queen)],
            board.bb_occ_rot, board.bb_occ_rot_main_diag, board.bb_occ_rot_second_diag,
            board.color_to_move,
            board.bb_occ,
            moves
        );
    EXPECT_EQ(moves.moves.size(), 10);

}
