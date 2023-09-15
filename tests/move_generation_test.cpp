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
    EXPECT_TRUE(moves.size()!=0);
}

TEST(MoveGenTest, DISABLED_BishopAttacksInternals){
    Bitboard bb = 0x20100004020000ULL;
    Bitboard bb_r = bb_rotate_45_cw(bb);
    EXPECT_EQ(first_diagonal_attacks[26][(bb_r >> diagonal_shift_mine[26]) & 0xff], 0x100800020000);
    Bitboard bb_2 = 0x40201000000ULL;
    Bitboard bb_r_2 = bb_rotate_45_cw(bb_2);
    EXPECT_EQ(first_diagonal_attacks[42][(bb_r_2 >> diagonal_shift_mine[42]) & 0xff], 0x1008000200000000);
}

TEST(MoveGenTest, DISABLED_BishopAttacks){
    Board::Board board("2q3r1/1pB3P1/5R2/PB4Pp/3Q1PkN/p7/b5p1/1K3N2 w - - 0 1");
    Moves moves;
    Bitboard bb = get_second_diagonal_attack(Square::b1, board.bb_occ_rot_second_diag) & board.bb_piece_type[int(Color::Black)][int(PieceTypes::Bishop)];
    EXPECT_EQ(pop_count(bb), 1);

}


TEST(MoveGenTest, DISABLED_QueenAttacks){
    Board::Board board("2R4K/R1P1p1p1/1P1B3n/bB3P2/brppP3/nP1p1P1P/k3ppPp/Nq2Q1r1 w - - 0 1");
    Moves moves;
    generate_queen_actions(board.bb_piece_type[int(board.color_to_move)][int(PieceTypes::Queen)],
            board.bb_occ_rot, board.bb_occ_rot_main_diag, board.bb_occ_rot_second_diag,
            board.color_to_move,
            board.bb_occ,
            moves
        );
    EXPECT_EQ(moves.size(), 10);

}

TEST(MoveGenTest, QueenAttaks2){
    Board::Board board("rnbqkbnr/pppppppp/8/8/8/7P/PPPPPPP1/RNBQKBNR b KQkq - 0 1");
    Moves moves;
    int from = 59;
    Bitboard attack = ((first_diagonal_attacks[int(from)][(board.bb_occ_rot_main_diag >> diagonal_shift_mine[int(from)]) & 0xff]
    | second_diagonal_attacks[int(from)][(board.bb_occ_rot_second_diag >> diagonal_second_shift_mine[int(from)]) & 0xff]))
    & ~board.bb_occ[int(board.color_to_move)];
    insert_moves(attack, static_cast<Square>(from), MoveType::Move, PieceTypes::Queen, moves);

    std::cerr << moves;
}

TEST(KakashiTest, FriendlyPawnBlockade){
    std::map<std::string, int> test_queen;
    init_all_attack_bitboards();
    test_queen.emplace("8/8/8/2PPP3/2PQP3/2PPP3/8/8 w - - 0 1", 0);
    test_queen.emplace("8/8/8/8/8/2PPP3/2PQP3/2PPP3 w - - 0 1", 0);
    test_queen.emplace("8/8/8/8/2PPP3/2PPP3/2PQP3/8 w - - 0 1", 3);
    test_queen.emplace("8/8/8/2ppp3/1PPQP3/2PPp3/2P1P3/8 w - - 0 1", 4);
    test_queen.emplace("8/8/1p6/2p1p3/1PPQP3/2PPp3/2P1P3/8 w - - 0 1", 7);
    test_queen.emplace("8/8/8/8/8/8/pppppppp/3kqb2 b - - 0 1", 0);
    test_queen.emplace("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1", 0);
    test_queen.emplace("2q3r1/1pB3P1/5R2/PB4Pp/5PkN/p7/b5p1/1Q3N2 w - - 0 1", 14);
    for(auto &[key, value] : test_queen){
        Board::Board board(key.c_str());
        Moves moves;
        generate_queen_actions(board.bb_piece_type[int(board.color_to_move)][int(PieceTypes::Queen)], board.bb_occ_rot, board.bb_occ_rot_main_diag, board.bb_occ_rot_second_diag,
            board.color_to_move, board.bb_occ, moves);
        std::cerr << key << " " << value << " " << moves.size() << std::endl;
        ASSERT_EQ(moves.size(),  value) << "missing moves in fen: " << key << moves << "-----\n";
    }


}

TEST(CastleTest, KingSideCastle){
    Board::Board board("rnbqkbnr/pppp2pp/5p2/4p3/4P3/3B1N2/PPPP1PPP/RNBQK2R w KQkq - 0 1");
    Moves moves = board.generate_pl_moves();
    for(Move move : moves){
        if(move.move_type == MoveType::Castle){
            std::cout << board << std::endl;
            board.make_move(move);
            std::cout << board << std::endl;
            board.unmake_move(move);
            std::cout << board << std::endl;

        }
    }
}

TEST(CastleTest, QueenSideCastle){
    Board::Board board("rnbqkbnr/ppp4p/4ppp1/3p4/3P1B2/2PQ4/PP1NPPPP/R3KBNR w KQkq - 0 1");
    Moves moves = board.generate_pl_moves();
    for(Move move : moves){
        if(move.move_type == MoveType::Castle){
            std::cout << board;
            std::cout << board.irr_data << std::endl;
            board.make_move(move);
            std::cout << board;
            std::cout << board.irr_data << std::endl;
            board.unmake_move(move);
            std::cout << board;
            std::cout << board.irr_data << std::endl;
        }
    }
}