#include "../include/moves.hpp"


void Moves::add_move(Move move){
    moves.push_back(move);
}

std::ostream& Moves::operator<<(std::ostream& os){
    for(const auto move : moves){
        os << move;
    }
    return os;
}

std::ostream& Move::operator<<(std::ostream& os){
    os  << 'a' + int(from) % 8 << int(from) / 8 + 1 << " "
        << 'a' + int(to) % 8 << int(to) / 8 + 1 << '\n';
    return os;
}

