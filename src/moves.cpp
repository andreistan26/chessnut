#include "../include/moves.hpp"


void Moves::add_move(Move move){
    moves.push_back(move);
}

std::ostream& operator<<(std::ostream& os, const Move& move){
    os  << (char)('a' + int(move.from) % 8) << int(move.from) / 8 + 1 << ' '
        << (char)('a' + int(move.to) % 8) << int(move.to) / 8 + 1 << '\n';
    return os;
}

std::ostream& operator<<(std::ostream& os, const Moves& moves){
    for(auto move : moves.moves){
        os << move;
    }
    return os;
}


