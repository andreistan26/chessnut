#include <gtest/gtest.h>
#include "../include/board.hpp"
#include <string>
class BoardTest : public ::testing::Test{
    protected:
        void SetUp() override {
            init_all_attack_bitboards();
            starting_board = Board::Board(default_fen_string.c_str());
        }

        //void TearDown() override {}
        
        Board::Board starting_board;
        const std::string default_fen_string = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
};

TEST_F(BoardTest, InitialPosMakeUnmake){
    Moves moves = starting_board.generate_pl_moves();
    int c = 1;
    print_bb(starting_board.bb_occ[2]); std::cerr << std::endl;
    print_bb(starting_board.bb_occ_rot_second_diag); std::cerr << std::endl;
    starting_board.make_move(moves.back());
    //Moves new_moves = starting_board.generate_pl_moves();
    //std::cerr << new_moves;
    int from = 59;
    Bitboard attack = //((first_diagonal_attacks[from][(starting_board.bb_occ_rot_main_diag >> diagonal_shift_mine[from]) & 0xff]))
        /*|*/ second_diagonal_attacks[int(from)][(starting_board.bb_occ_rot_second_diag >> diagonal_second_shift_mine[from]) & 0xff]//)
        //| ((rank_attacks[int(from)][(starting_board.bb_occ[2] >> (int(from)/8*8)) & 0xff]) | file_attacks[int(from)][(starting_board.bb_occ_rot >> (from%8*8)) & 0xff]))
    & ~starting_board.bb_occ[int(starting_board.color_to_move)];
    print_bb(starting_board.bb_occ_rot_second_diag); std::cerr << std::endl;
    print_bb(starting_board.bb_occ_rot_second_diag >> diagonal_second_shift_mine[from]); std::cerr << std::endl;
    print_bb(attack);
    /*
    for(Move move : moves){
        std::cerr << "Move " << c << " :" << std::endl;
        starting_board.make_move(move);
        std::cerr << starting_board << std::endl;
        starting_board.unmake_move();
        std::cerr << starting_board << std::endl;
        std::cerr << "--------" << std::endl;
        c++; 
    }
    */
}


TEST(BoardTestStatic, CheckTestBackAttack){
    const std::string fen_check_test[6] = {"2q2br1/1pB3P1/4rR2/PB4Pp/3Q1PkN/8/p5p1/1K3N2 w - - 0 1", //pawn
                "2q2br1/1pB3P1/4rR2/PB4Pp/3Q1PkN/p7/3n2p1/1K3N2 w - - 0 1",                 //knight
                "2q2br1/1pB3P1/5R2/PB4Pp/1r1Q1PkN/p7/6p1/1K3N2 w - - 0 1",                  //file r
                "2q2br1/1pB3P1/5R2/PB4Pp/3Q1PkN/p7/6p1/1K1r1N2 w - - 0 1",                  //rank r
                "2q3r1/1pB3P1/5R2/PB4Pp/3Q1PkN/p2b4/6p1/1K3N2 w - - 0 1",                   //first diag b
                "2q3r1/1pB3P1/5R2/PB4Pp/3Q1PkN/p7/b5p1/1K3N2 w - - 0 1",                    //second diag b
                };
    EXPECT_EQ(Board::Board(fen_check_test[0].c_str()).is_attacking(Square::b1, Color::White, true), 1);
    EXPECT_EQ(Board::Board(fen_check_test[1].c_str()).is_attacking(Square::b1, Color::White, true), 2);
    EXPECT_EQ(Board::Board(fen_check_test[2].c_str()).is_attacking(Square::b1, Color::White, true), 4);
    EXPECT_EQ(Board::Board(fen_check_test[3].c_str()).is_attacking(Square::b1, Color::White, true), 4);
    EXPECT_EQ(Board::Board(fen_check_test[4].c_str()).is_attacking(Square::b1, Color::White, true), 3);
    EXPECT_EQ(Board::Board(fen_check_test[5].c_str()).is_attacking(Square::b1, Color::White, true), 3);
}

TEST(BoardTestStatic, CastleTest){
    const std::string castle_test = "r2qk1nr/pp3pbp/2n1p1p1/3p4/3P4/2PB1N1P/PP3PP1/R1BQK2R w KQkq - 0 10";
    Board::Board board(castle_test.c_str());
    Moves moves;
    board.generate_castle_moves(moves);
    std::cerr << moves;
    std::cerr << std::hex << board.bb_occ[2] << std::endl;
    board.make_move(moves.front());
    std::cerr << board.bb_occ[2] << std::endl;
}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
