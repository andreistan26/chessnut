#ifndef MOVES_H
#define MOVES_H
//#include "../include/board.hpp"
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



class Moves{
    std::vector<Move> moves;
public:
    void add_move(Move move);
    std::ostream& operator<<(std::ostream& os);
};

struct Move{
    Square from;
    Square to;
    MoveType move_type;
    std::ostream& operator<<(std::ostream& os);
};
#endif
