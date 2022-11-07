#ifndef ENGINE_H
#define ENGINE_H

#include "moves.hpp"
#include "board.hpp"
#include "search.hpp"
#include "logger.hpp"

enum class EngineState{
    NOT_INIT, INIT, THINKING, AWAITING
};

struct Engine{
    EngineState engine_state;
    Move last_move;
    Board::Board board;
    Engine();
    void init();
    void update_op_move(Move move, bool is_opponent);
    Move search_move();
private:
    
};
#endif
