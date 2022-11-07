#include "../include/engine.hpp"
Engine::Engine(){
    board = Board::Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    init_all_attack_bitboards();
}

void Engine::update_op_move(Move move, bool is_opponent){
    if(is_opponent){
        move.piece = board.get_piece(move.from);
        move.move_type = board.get_type(move);
    }
    board.make_move(move);
    while(!board.undo_stack.empty())
        board.undo_stack.pop();
    log_stream << "Engine Made move : " << move;
    log_stream << board.debug_info();
}

Move Engine::search_move(){
    log_stream << "Looking for move..." << std::endl;
    Move move = search_negamax_root(board, 3);
    log_stream << "Engine Found move : " << move << std::flush;
    update_op_move(move, false);
    return move;
}
