#include <gtest/gtest.h>
#include "search.hpp"

TEST(TestSearch, InitialPosition){
	init_all_attack_bitboards();
	std::cerr << std::dec;
	std::cerr << "----" << std::endl;
	Board::Board *board = new Board::Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	Move move = search_negamax_root(board, 5);
	delete board;
	std::cerr << move ;
}

TEST(TestSearch, DISABLED_InitialPositionAb){
	std::cerr << std::dec;
	std::cerr << "----" << std::endl;
	Board::Board *board = new Board::Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	// Move move = search_negamax_root_ab(board, 3);
	delete board;
	// std::cerr << move ;
}


TEST(TestSearch, DISABLED_Position1){
	std::cerr << std::dec;
	std::cerr << "----" << std::endl;
	Board::Board *board = new Board::Board("rnbqkbnr/ppp2ppp/4p3/3p4/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 0 1");
	Move move = search_negamax_root(board, 2);
	delete board;
	std::cerr << move;
}

//take to separate test
TEST(TestMakeMove, DISABLED_TakeWithKnight){
	Board::Board board = Board::Board("rnbqkbnr/ppp2ppp/B3p3/3p4/4P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 0 1");
	Moves take_bish_moves = Moves();
	take_bish_moves.push_back(Move(Square::b8, Square::a6, MoveType::Empty, PieceTypes::Knight));
	take_bish_moves.push_back(Move(Square::b7, Square::a6, MoveType::Empty, PieceTypes::Pawn));
	for(Move move : take_bish_moves){
		board.make_move(move);
		std::cerr << "Made move " << move << std::endl;
		std::cerr << board;
		board.unmake_move(move);
		std::cerr << "Unamed move\n";
		std::cerr << board;
		std::cerr << "---------\n";
	}
}