#include "../include/engine.hpp"
Engine::Engine(){
    board = Board::Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    init_all_attack_bitboards();
}

void Engine::update_op_move(Move move, bool is_opponent){
    engine_state = EngineState::PRE_SEARCH;
    if(is_opponent){
        move.piece = board.get_piece(move.from);
        move.move_type = board.get_type(move);
    }
    board.make_move(move);
    board.undo_stack.clear();

    log_stream << "Engine Made move : " << move;
    log_stream << board.debug_info();

    engine_state = EngineState::SEARCH;
}

Move Engine::search_move(){
    assert(engine_state == EngineState::SEARCH);
    time_profile.reset_time_new_move();
    time_profile.set_time_limit(3000);

    log_stream << "Looking for move..." << std::endl;
    Move move = search_move_internal();
    log_stream << "Engine Found move : " << move << std::flush;
    update_op_move(move, false);
    return move;
}

Move Engine::search_move_internal(){
    Move best_move;
    for(int depth = 1; ; depth++){
        if(time_profile.check_is_overtime() || depth == 7){
            log_stream << "Depth reached: " << depth - 1 << std::endl;
            return best_move;
        }
        best_move = search_ab_root(board, depth); 
    }
    return best_move;
}


void TimeProfile::reset_time_new_move(){
    move_started_time = timer::now();
}

TimeProfile::ms TimeProfile::get_time_remaining(){
    return std::chrono::duration_cast<ms>(timer::now() - move_started_time);
}

bool TimeProfile::check_is_overtime(){
    return get_time_remaining() > time_limit;
}

void TimeProfile::set_time_limit(unsigned long long time_limit_){
    time_limit = ms(time_limit_);
}