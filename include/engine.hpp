#ifndef ENGINE_H
#define ENGINE_H

#include "moves.hpp"
#include "board.hpp"
#include "search.hpp"
#include "logger.hpp"
#include <chrono>

enum class EngineState{
    NOT_INIT, INIT, PRE_SEARCH, SEARCH, AWAITING
};

class TimeProfile{
    typedef std::chrono::steady_clock timer;
    typedef std::chrono::time_point<timer> time;
    typedef std::chrono::milliseconds ms;

    time move_started_time;
    ms time_limit;
    ms get_time_remaining();
public:
    bool check_is_overtime();
    void set_time_limit(unsigned long long time_limit);
    void reset_time_new_move();
};

struct Engine{
    Engine();
    void init();
    void update_op_move(Move move, bool is_opponent);
    Move search_move();
private:
    EngineState engine_state;
    Move last_move;
    Board::Board board;
    TimeProfile time_profile;
    Move search_move_internal();
};


#endif
