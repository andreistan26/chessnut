#ifndef MOVES_H
#define MOVES_H

#include "bitboard.hpp"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <stdint.h>
#include <utility>
enum class MoveType : uint8_t{
    Move,
    Capture,
    Ep,
    Castle,
    Promotion,
    Double_Pawn_Push,
    Empty
};

extern std::unordered_map<Square, std::pair<Square, Square>> rook_map;

extern std::unordered_map<Square, std::pair<Square, Square>> castled_rook_by_king;

extern const char *move_type_to_string[7];
struct Move{
    Square from;
    Square to;
    MoveType move_type;
    PieceTypes piece;
    PieceTypes captured;

    Move();
    Move(const Square from_, const Square to_, const MoveType move_type_ , const PieceTypes piece_ = PieceTypes::Empty, const PieceTypes captured_ = PieceTypes::Empty);
    Move(const std::string& move_str);
    
    std::string to_string_uci();
    Move& operator=(const std::string &move_str);
    friend std::ostream& operator<<(std::ostream& os, const Move& move);

    bool empty();
};

class Moves : public std::vector<Move>{
public:
    friend std::ostream& operator<<(std::ostream& os, const Moves& moves);
};

#endif
