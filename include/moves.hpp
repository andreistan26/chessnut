#ifndef MOVES_H
#define MOVES_H

#include "bitboard.hpp"
#include <iostream>
#include <vector>

enum class MoveType{
    PAWN_PUSH_SINGLE,
    PAWN_PUSH_PROMO,  
    PAWN_PUSH_DOUBLE,
    PAWN_ATTACK,        
    PAWN_ENP_ATTACK,   
    KNIGHT_MOVE,
    KNIGHT_ATTACK,
    KING_MOVE,
    KING_ATTACK,
    ROOK_MOVE,          
    ROOK_ATTACK,        
    BISHOP_MOVE,        
    BISHOP_ATTACK,      
    QUEEN_MOVE,
    QUEEN_ATTACK,
    CASTLE_RIGHT,
    CASTLE_LEFT
};

struct Move{
    Square from;
    Square to;
    MoveType move_type;
    friend std::ostream& operator<<(std::ostream& os, const Move& move);
};

class Moves{
public:
    std::vector<Move> moves;
    void add_move(Move move);
    friend std::ostream& operator<<(std::ostream& os, const Moves& moves);
};

#endif
