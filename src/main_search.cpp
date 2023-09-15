#include "search.hpp"

int main(){
    init_all_attack_bitboards();
    Board::Board board = Board::Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    Move move = search_negamax_root(board, 5);
    return 0;
}
